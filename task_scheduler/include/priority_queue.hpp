#pragma once

#include <vector>
#include "task.hpp"

class PriorityQueue{
  public:
    PriorityQueue(int);
    void buildHeap(const std::vector<Task>&);
    void insertTask(const Task&);
    Task* getHighestPriority();
    Task extractHighestPriority();
    void deleteTask(Task&);
    void printQueue();
    
  private:
    std::vector<Task> array;
    int size;
    int capacity;

    void heapify(int);
};