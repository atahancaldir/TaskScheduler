#pragma once

#include "base_queue.hpp"

class TaskQueue : public BaseQueue{
  public:
    TaskQueue(size_t=10);
    void insertTask(const Task&);
    void deleteTask(Task&);
};