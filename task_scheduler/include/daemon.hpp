#pragma once

#include <string>
#include <memory>
#include <functional>
#include "scheduler.hpp"
#include "constants.hpp"
#include "utils.hpp"

class Daemon{
  public:
    static Daemon& getInstance();

    Daemon(const Daemon&) = delete;
    Daemon& operator=(const Daemon&) = delete;

    bool start(SchedulingType schedulingType = SchedulingType::fcfs);
    void stop();
    bool isRunning() const;

    std::string sendCommand(const std::string& command);
    void handleCommand(const std::string& command, int client_fd);
  
  private:
    Daemon() = default;
    ~Daemon();

    // Daemon process management
    void daemonize();
    void writePidFile();
    void removePidFile();
    void setupSignalHandlers();

    // IPC setup
    bool setupSocket();
    void cleanupSocket();
    void handleClientConnection(int client_fd);

    // Member variables
    std::string pid_file_ = "/tmp/task_scheduler.pid";
    std::string socket_path_ = "/tmp/task_scheduler.sock";
    int server_fd_ = -1;
    std::unique_ptr<Scheduler> scheduler_;
    bool running_ = false;

    // Static signal handler
    static void signalHandler(int signum);
};