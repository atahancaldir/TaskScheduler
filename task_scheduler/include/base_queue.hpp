#pragma once

#include <vector>
#include "task.hpp"

class BaseQueue{
  public:
    BaseQueue(size_t=10);
    virtual ~BaseQueue() = default;

    virtual void insertTask(const Task&) = 0;
    virtual void deleteTask(const std::string&) = 0;

    std::vector<Task>& getQueue();
    bool isEmpty() const { return size == 0; }
    size_t getSize() const { return size; }
    void clear();

  protected:
    std::vector<Task> array;
    int size;
    size_t capacity;
};