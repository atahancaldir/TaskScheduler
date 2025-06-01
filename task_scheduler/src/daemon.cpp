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
  char buffer[1024];
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
  std::istringstream iss(command);
  std::string cmd;
  iss >> cmd;

  std::string response;

  if (cmd == "add"){
    // TODO: handle command
  } else if (cmd == "clear"){
    // TODO: handle command
  } else if (cmd == "status"){
    // TODO: handle command
  }else if (cmd == "delete"){
    // TODO: handle command
  } else if (cmd == "list"){
    response = scheduler_->getQueueStatus();
  } else if (cmd == "algorithm"){
    response = scheduler_->getQueueStatus();
  }else if (cmd == "stop"){
    stop();
  }

  send(client_fd, response.c_str(), response.length(), 0);
}

// Send command from CLI to background
bool Daemon::sendCommand(const std::string& command){
  int sock = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sock < 0){
    Logger::log("Failed to create a client socket");
    return false;
  }

  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, socket_path_.c_str(), sizeof(addr.sun_path) - 1);

  if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0){
    Logger::log("Failed to connect to daemon");
    close(sock);
    return false;
  }

  if (send(sock, command.c_str(), command.length(), 0) < 0) {
    Logger::log("Failed to send command");
    close(sock);
    return false;
  }

  char buffer[1024];
  ssize_t bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
  if (bytes > 0) {
    buffer[bytes] = '\0';
    std::cout << buffer << std::endl;
  }

  close(sock);
  return true;
}