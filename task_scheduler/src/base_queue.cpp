#include "base_queue.hpp"

BaseQueue::BaseQueue(size_t capacity_){
  size = 0;
  capacity = capacity_;
  array.resize(capacity_);
}

// Return the priority queue
const std::vector<Task>& BaseQueue::getQueue() const {
  return array;
}