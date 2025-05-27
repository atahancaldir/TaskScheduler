#pragma once

#include <string>
#include <functional>

class Task{
  public:
    std::string id;
    int priority;
    std::function<void()> execute;

    Task(std::function<void()>, int);
    bool operator==(const Task&) const;
};