#include "task.hpp"
#include "utils.hpp"
#include <string>
#include <vector>

Task::Task(std::string taskString){
  std::vector<std::string> taskBuilder = utils::splitString(taskString, "::");
  command = taskBuilder.at(0);
  if (taskBuilder.size() > 1)
    priority = stoi(taskBuilder.at(1));
  else
    priority = -1;
  id = utils::generateUUID();
  status = TaskStatus::pending;
}

Task::Task(std::string command_, int priority_=0) : 
          command(command_), priority(priority_){
  id = utils::generateUUID();
  status = TaskStatus::pending;
}

TaskStatus Task::getStatus() const { return status; }

bool Task::operator==(const Task& other) const{
  return other.id == id && other.priority == priority;
}