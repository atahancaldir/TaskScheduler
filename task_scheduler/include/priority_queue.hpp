#pragma once

#include "base_queue.hpp"

class PriorityQueue : public BaseQueue{
  public:
    PriorityQueue(size_t=10);
    void insertTask(const Task&);
    Task* getHighestPriority();
    Task extractHighestPriority();
    void deleteTask(Task&);
    
  private:
    void buildHeap(const std::vector<Task>&);
    void heapify(int);
};