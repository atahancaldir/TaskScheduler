#include "daemon.hpp"
#include "logger.hpp"
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

Daemon& Daemon::getInstance(){
  static Daemon instance;
  return instance;
}

Daemon::~Daemon(){
  if (running_){
    stop();
  }
}

bool Daemon::start(SchedulingType schedulingType){
  if(isRunning()){
    Logger::log("Daemon is already running");
    return false;
  }

  daemonize();
  setupSignalHandlers();

  if(!setupSocket()){
    Logger::log("Failed to setup IPC socket");
    cleanupSocket();
    return false;
  }

  scheduler_ = std::make_unique<Scheduler>(schedulingType);
  running_ = true;

  // Main daemon loop
  while(running_){
    int client_fd = accept(server_fd_, nullptr, nullptr);
    if (client_fd < 0) continue;

    handleClientConnection(client_fd);
    close(client_fd);
  }
  
  return true;
}

void Daemon::stop(){
  if(!isRunning()) return;

  running_ = false;
  cleanupSocket();
  removePidFile();

  Logger::log("Daemon is stopped");
}

bool Daemon::isRunning() const {
  std::ifstream pidFile(pid_file_);
  if(!pidFile) return false;

  pid_t pid;
  pidFile >> pid;
  return kill(pid, 0) == 0; // Check if pid is running
}

void Daemon::daemonize(){
  pid_t pid = fork();
  if (pid < 0) exit(EXIT_FAILURE); // Fork is failed
  if (pid > 0) exit(EXIT_SUCCESS); // Fork is sucessful, parent process exists. Child will remain.

  // Set file creation mask to 0. This will allow daemon to create files with any permissions
  umask(0);

  if (setsid() < 0) exit(EXIT_FAILURE); // Create a new session and detach from terminal
  if (chdir("/") < 0) exit(EXIT_FAILURE); // Change to root directory

  // Close stdin, stdout, and stderr to prevent daemon from reading/writing terminal
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  writePidFile();
}

void Daemon::writePidFile(){
  std::ofstream pidFile(pid_file_);
  pidFile << getpid();

  Logger::log("Created PID file");
}

void Daemon::removePidFile(){
  fs::remove(pid_file_);

  Logger::log("Removed PID file");
}

void Daemon::setupSignalHandlers(){
  signal(SIGTERM, signalHandler); // Termination request
  signal(SIGINT, signalHandler); // Keyboard interrupt (Ctrl+C)
}

void Daemon::signalHandler(int signum){
  if (signum == SIGTERM || signum == SIGINT){
    Daemon::getInstance().stop();
  }
}

bool Daemon::setupSocket(){
  server_fd_ = socket(AF_UNIX, SOCK_STREAM, 0); // Socket for IPC
  if (server_fd_ < 0) return false;

  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, socket_path_.c_str(), sizeof(addr.sun_path) - 1);

  if (bind(server_fd_, (struct sockaddr*)&addr, sizeof(addr)) < 0){
    close(server_fd_);
    return false;
  }

  if (listen(server_fd_, 5) < 0){
    close(server_fd_);
    return false;
  }

  return true;
}

void Daemon::cleanupSocket(){
  if (server_fd_ >= 0){
    close(server_fd_);
  }
  fs::remove(socket_path_);
  Logger::log("Socket cleaned up");
}

void Daemon::handleClientConnection(int client_fd){
  char buffer[constants::SOCKET_BUFFER_SIZE];
  ssize_t bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
  if (bytes > 0){
    buffer[bytes] = '\0';
    handleCommand(std::string(buffer), client_fd);
  } else if (bytes < 0){
    Logger::log("Error receiving command from client");
  }
}


// Handle command coming from CLI in the background
void Daemon::handleCommand(const std::string& command, int client_fd){
  std::string cmd = command.substr(0, command.find(" "));
  std::string response;

  Logger::log("Background received command: " + command);

  if (cmd == "add"){
    std::string taskString = command.substr(command.find(" ")+1);
    if (scheduler_->addTask(taskString)){
      response = "Task is added to queue";
    } else{
      response = "Task could not be added to queue";
    }
  } else if (cmd == "clear"){
    scheduler_->clearQueue();
    response = "Task queue is cleared";
  } else if (cmd == "delete"){
    std::string taskID = utils::splitString(command, " ")[1];
    if (scheduler_->deleteTask(taskID)){
      response = "Task is deleted";
    } else{
      response = "Task could not be deleted";
    }
  } else if (cmd == "run"){
    scheduler_->run();
    response = "Task queue is running";
  } else if (cmd == "pause"){
    scheduler_->pause();
    response = "Task queue is paused";
  } else if (cmd == "list"){
    response = scheduler_->getQueueStatus();
  } else if (cmd == "algorithm"){
    response = constants::SCHEDULER_TYPE_NAMES.at(scheduler_->getSchedulingType());
  }else if (cmd == "stop"){
    scheduler_->stop();
    stop();
    response = "Task scheduler stopped";
  }

  send(client_fd, response.c_str(), response.length(), 0);
}

// Send command from CLI to background
std::string Daemon::sendCommand(const std::string& command){
  int sock = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sock < 0){
    Logger::log("Failed to create a client socket");
    return std::string();
  }

  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, socket_path_.c_str(), sizeof(addr.sun_path) - 1);

  if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0){
    Logger::log("Failed to connect to daemon");
    close(sock);
    return std::string();
  }

  if (send(sock, command.c_str(), command.length(), 0) < 0) {
    Logger::log("Failed to send command");
    close(sock);
    return std::string();
  }

  char buffer[constants::SOCKET_BUFFER_SIZE];
  ssize_t bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
  std::string response;
  if (bytes > 0) {
    buffer[bytes] = '\0';
    response = std::string(buffer);
  }

  close(sock);
  return response;
}