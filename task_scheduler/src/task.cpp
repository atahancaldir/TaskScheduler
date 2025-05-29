#include "task.hpp"
#include "utils.hpp"
#include <string>

Task::Task(std::string name_, std::function<void()> func, int priority_) : 
          name(name_), priority(priority_){
  id = utils::generateUUID();
  execute = func;
  status = pending;
}

TaskStatus Task::getStatus() const { return status; }

bool Task::operator==(const Task& other) const{
  return other.id == id && other.priority == priority;
}