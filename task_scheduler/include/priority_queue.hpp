#pragma once

#include <vector>
#include "task.hpp"

class PriorityQueue{
  public:
    PriorityQueue(size_t=10);
    void buildHeap(const std::vector<Task>&);
    void insertTask(const Task&);
    Task* getHighestPriority();
    Task extractHighestPriority();
    void deleteTask(Task&);
    const std::vector<Task>& getQueue() const;
    
  private:
    std::vector<Task> array;
    int size;
    int capacity;

    void heapify(int);
};