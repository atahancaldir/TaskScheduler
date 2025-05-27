#include "task.hpp"
#include "../include/utils.hpp"

Task::Task(std::function<void()> func, int priority_) : priority(priority_){
  id = utils::generateUUID();
  execute = func;
}

bool Task::operator==(const Task& other) const{
  return other.id == id && other.priority == priority;
}