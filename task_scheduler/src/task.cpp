#include "task.hpp"
#include "utils.hpp"
#include "logger.hpp"
#include <string>
#include <vector>

Task::Task(std::string taskString){
  std::vector<std::string> taskBuilder = utils::splitString(taskString);
  command = taskBuilder.at(0);
  if (taskBuilder.size() > 1)
    priority = stoi(taskBuilder.at(1));
  else
    priority = -1;
  id = utils::generateUUID();
  pid = -1;
  status = TaskStatus::pending;
  duration = std::chrono::seconds(0);
}

Task::Task(std::string command_, int priority_=0) : 
          command(command_), priority(priority_){
  id = utils::generateUUID();
  pid = -1;
  status = TaskStatus::pending;
  duration = std::chrono::seconds(0);
}

TaskStatus Task::getStatus() const { return status; }

void Task::setStatus(TaskStatus status_){
  auto now = std::chrono::steady_clock::now();
  if (status_ == TaskStatus::running && status != TaskStatus::running){
    startTime = now;
  } else if (status == TaskStatus::running && status_ != TaskStatus::running){
    duration += std::chrono::duration_cast<std::chrono::seconds>(now - startTime);
  }
  status = status_;
  Logger::log("Task status changed to '" + constants::TASK_STATUS_NAMES.at(status) + "': " + id + ", PID: " + std::to_string(pid));
}

std::string Task::getDuration() const {
  std::chrono::seconds currentDuration;
  if (status == TaskStatus::running) {
    // For running tasks, include current session duration
    auto now = std::chrono::steady_clock::now();
    currentDuration = duration + std::chrono::duration_cast<std::chrono::seconds>(now - startTime);
  } else {
    // For non-running tasks, just show accumulated duration
    currentDuration = duration;
  }
  int totalSeconds = currentDuration.count();
  int hours = totalSeconds / 3600;
  int minutes = (totalSeconds % 3600) / 60;
  int seconds = totalSeconds % 60;
  
  char buffer[9];
  snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", hours, minutes, seconds);
  return std::string(buffer);
}

bool Task::operator==(const Task& other) const{
  return other.id == id && other.priority == priority;
}