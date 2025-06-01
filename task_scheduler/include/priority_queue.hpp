#pragma once

#include "base_queue.hpp"

class PriorityQueue : public BaseQueue{
  public:
    PriorityQueue(size_t=10);
    void insertTask(const Task&) override;
    Task* getHighestPriority();
    Task extractHighestPriority();
    void deleteTask(const Task&) override;
    
  private:
    void buildHeap(const std::vector<Task>&);
    void heapify(int);
};