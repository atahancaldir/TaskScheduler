#pragma once

#include <string>
#include <chrono>
#include "constants.hpp"

class Task{
  public:
    std::string id;
    pid_t pid;
    std::string command;
    int priority;
    
    Task() = default;
    Task(std::string);
    Task(std::string, int);
    TaskStatus getStatus() const;
    void setStatus(TaskStatus);
    bool operator==(const Task&) const;
    std::string getDuration() const;

  private:
    TaskStatus status;
    std::chrono::steady_clock::time_point startTime;
    std::chrono::seconds duration;
};