#include "task.hpp"

bool Task::operator==(const Task& other) const{
  return other.id == id && other.priority == priority;
}