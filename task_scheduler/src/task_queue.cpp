#include "task_queue.hpp"
#include <iostream>

TaskQueue::TaskQueue(size_t capacity_) : BaseQueue(capacity_) {}

void TaskQueue::insertTask(const Task& t){
  if (size == capacity){
    capacity *= 2;
    array.resize(capacity);
  }

  array[size++] = t;
}

void TaskQueue::deleteTask(Task& task){
  bool shifting = false;
  for (size_t i = 0; i < size-1; i++){
    if (array[i] == task) 
      shifting = true;
    if (shifting){
      array[i] = array[i+1];
    }
  }
  size--;

  if (size < capacity / 4) {
    capacity /= 2;
    array.resize(capacity);
  }
}