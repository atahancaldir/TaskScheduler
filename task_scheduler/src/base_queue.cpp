#include "base_queue.hpp"

BaseQueue::BaseQueue(size_t capacity_){
  size = 0;
  capacity = capacity_;
  array.resize(capacity_);
}

// Return the priority queue
std::vector<Task>& BaseQueue::getQueue() {
  return array;
}

void BaseQueue::clear(){
  array.clear();
  size = 0;
}