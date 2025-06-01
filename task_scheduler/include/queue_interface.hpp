#pragma once

#include <vector>
#include "task.hpp"

class ITaskQueue{
  public:
    virtual ~ITaskQueue() = default;

    virtual void insertTask(const Task&) = 0;
    virtual void deleteTask(Task&) = 0;
    virtual const std::vector<Task>& getQueue() const = 0;

  protected:
    std::vector<Task> array;
    int size;
    int capacity;
};