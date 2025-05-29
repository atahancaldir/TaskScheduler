#pragma once

#include <string>
#include <functional>

enum TaskStatus{
  pending,
  running,
  finished,
  killed,
  failed,
};

class Task{
  public:
    std::string id;
    std::string name;
    int priority;
    std::function<void()> execute;

    Task() = default;
    Task(std::string, std::function<void()>, int);
    TaskStatus getStatus() const;
    void start();
    bool operator==(const Task&) const;

  private:
    TaskStatus status;
};