#include "priority_queue.hpp"
#include <iostream>

PriorityQueue::PriorityQueue(size_t capacity_) : BaseQueue(capacity_) {}

// Restore heap order at index i
void PriorityQueue::heapify(int i){
  int root = i;
  int left = 2 * i + 1;
  int right = 2 * i + 2;

  if (left < size && array[left].priority > array[root].priority){
    root = left;
  }

  if (right < size && array[right].priority > array[root].priority){
    root = right;
  }

  if (root != i){
    std::swap(array[i], array[root]);
    heapify(root);
  }
}

// Create a priority queue from a given array
void PriorityQueue::buildHeap(const std::vector<Task>& arr){
  capacity = arr.size();
  size = capacity;
  array = arr;

  // Heapify (n-1)/2 nodes
  for(int i=(size-1)/2; i>=0; i--){
    heapify(i);
  }
}

// Insert a new task into priority queue
void PriorityQueue::insertTask(const Task& t){
  if (size == capacity){
    capacity *= 2;
    array.resize(capacity);
  }

  size++;
  int i = size-1;
  array[i] = t;

  while(i != 0 && array[(i-1)/2].priority < array[i].priority){
    std::swap(array[i], array[(i-1)/2]);
    i = (i-1)/2;
  }
}

// Return highest priority task
Task* PriorityQueue::getHighestPriority(){
  if(size <= 0) return nullptr;
  return &array.front();
}

// Remove and return highest priority task
Task PriorityQueue::extractHighestPriority(){
  if(size <= 0) throw std::runtime_error("Empty queue");

  Task task = array[0];

  if(size > 1){
    array[0] = array[size-1];
    size--;
    heapify(0);
  } else{
    size--;
  }
  return task;
}

// Delete task from priority queue
void PriorityQueue::deleteTask(const std::string& taskID){
  for(size_t i=0; i<size; i++){
    if(array[i].id == taskID){
      array.erase(array.begin() + i);
      size--;

      if (size < capacity / 4){
        capacity /= 2;
        array.resize(capacity);
      }

      for (int j=(size-1)/2; j>=0; j--){
        heapify(j);
      }
      return;
    }
  }
}