#pragma once

#include <string>
#include <functional>
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

  private:
    TaskStatus status;
};