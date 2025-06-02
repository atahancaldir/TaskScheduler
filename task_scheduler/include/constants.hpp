#pragma once

#include <string>
#include <unordered_map>

// Enums
enum class SchedulingType{
  fcfs,
  roundRobin
};

enum class SchedlerStatus{
  stopped,
  running,
  paused
};

enum class TaskStatus {
    pending,
    running,
    paused,
    finished,
    killed
};

// Conversion maps
namespace constants {
  const std::unordered_map<SchedulingType, std::string> SCHEDULER_TYPE_NAMES = {
      {SchedulingType::fcfs, "fcfs"},
      {SchedulingType::roundRobin, "round-robin"}
  };

  const std::unordered_map<SchedulingType, std::string> SCHEDULER_TYPE_PRETTY_NAMES = {
      {SchedulingType::fcfs, "fcfs"},
      {SchedulingType::roundRobin, "round-robin"}
  };

  const std::unordered_map<std::string, SchedulingType> SCHEDULER_TYPE_VALUES = {
    {"fcfs", SchedulingType::fcfs},
    {"round-robin", SchedulingType::roundRobin}
  };

  const std::unordered_map<TaskStatus, std::string> TASK_STATUS_NAMES = {
      {TaskStatus::pending, "pending"},
      {TaskStatus::running, "running"},
      {TaskStatus::paused, "paused"},
      {TaskStatus::finished, "finished"},
      {TaskStatus::killed, "killed"}
  };

  const std::string TASK_BUILDER_DELIMITER = "::";
  const int ROUND_ROBIN_QUANTUM = 3; // ms
}