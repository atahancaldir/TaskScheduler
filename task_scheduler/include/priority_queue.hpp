#pragma once

#include <vector>
#include "task.hpp"
#include "queue_interface.hpp"

class PriorityQueue : public ITaskQueue{
  public:
    PriorityQueue(size_t=10);
    void insertTask(const Task&);
    Task* getHighestPriority();
    Task extractHighestPriority();
    void deleteTask(Task&);
    const std::vector<Task>& getQueue() const;
    
  private:
    std::vector<Task> array;
    int size;
    int capacity;

    void buildHeap(const std::vector<Task>&);
    void heapify(int);
};