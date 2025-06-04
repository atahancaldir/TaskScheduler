#include "base_queue.hpp"
#include <cmath>

BaseQueue::BaseQueue(size_t capacity_){
  size = 0;
  capacity = std::min(capacity_, constants::MAX_QUEUE_CAPACITY);
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