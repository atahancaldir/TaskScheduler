#include "task_queue.hpp"
#include <iostream>

TaskQueue::TaskQueue(size_t capacity_) : BaseQueue(capacity_) {}

void TaskQueue::insertTask(const Task& task){
  if (size == capacity){
    capacity *= 2;
    array.resize(capacity);
  }

  array[size++] = task;
}

void TaskQueue::deleteTask(const std::string& taskID){
  for (size_t i = 0; i < size-1; i++){
    if (array[i].id == taskID){
      array.erase(array.begin() + i);
    }
  }
  size--;

  if (size < capacity / 4) {
    capacity /= 2;
    array.resize(capacity);
  }
}