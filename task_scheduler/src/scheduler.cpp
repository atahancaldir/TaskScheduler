#include "scheduler.hpp"
#include "table_printer.hpp"
#include "priority_queue.hpp"
#include "task_queue.hpp"
#include "task.hpp"
#include "logger.hpp"
#include "constants.hpp"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <chrono>
#include <thread>

Scheduler::Scheduler(SchedulingType schedulerType_) : schedulerType(schedulerType_){
  status = SchedlerStatus::stopped;
  if (schedulerType == SchedulingType::fcfs ||
      schedulerType == SchedulingType::roundRobin){
    queue = std::make_unique<TaskQueue>();
  } else if (schedulerType == SchedulingType::priority) {
    queue = std::make_unique<PriorityQueue>();
  }
}

SchedulingType Scheduler::getSchedulingType(){
  return schedulerType;
}

SchedlerStatus Scheduler::getStatus(){
  return status.load();
}

bool Scheduler::addTask(std::string taskString){
  Task task(taskString);

  if (status.load() == SchedlerStatus::running ||
      status.load() == SchedlerStatus::paused){
    {
      std::lock_guard<std::mutex> lock(tasksMutex);
      CreateTaskProcesses(task);
    }
  }

  queue->insertTask(task);
  Logger::log("Task is added to queue: " + task.id);
  return true;
}

bool Scheduler::deleteTask(std::string taskID){
  queue->deleteTask(taskID);
  Logger::log("Task is deleted from queue: " + taskID);
  return true;
}

// Initialize task executions and pause
void Scheduler::CreateTaskProcesses(Task& task){
  if (task.id.empty() || task.pid != -1) return;
  pid_t pid = fork();
  if (pid == 0){
    execl("/bin/sh", "sh", "-c", task.command.c_str(), (char *)nullptr);
    exit(1);
  } else {
    kill(pid, SIGSTOP); // Pause task
    task.pid = pid;
  }
}

void Scheduler::run(){
  if (status.load() == SchedlerStatus::running){
    return;
  } else if (status.load() == SchedlerStatus::paused){
    status = SchedlerStatus::running;
    Logger::log("Scheduler is resuming");
    return;
  }
  status = SchedlerStatus::running;
  Logger::log("Scheduler is running");

  {
    std::lock_guard<std::mutex> lock(tasksMutex);
    for (Task& task : queue->getQueue()){
      CreateTaskProcesses(task);
    }
  }

  std::thread([this](){
    Logger::log("Scheduler thread started");
    while(true){
      if (status.load() == SchedlerStatus::stopped) {
        std::lock_guard<std::mutex> lock(tasksMutex);
        for (Task& task : queue->getQueue()){
          if (!task.id.empty() && task.pid > 0){
            kill(task.pid, SIGTERM);
            waitpid(task.pid, nullptr, 0);
          }
        }
        queue->clear();
        return;
      }

      // Wait for a task until scheduler stops
      {
        std::lock_guard<std::mutex> lock(tasksMutex);
        if (queue->getQueue().empty()) {
          if (status.load() == SchedlerStatus::stopped) {
            return;
          }
          std::this_thread::sleep_for(std::chrono::milliseconds(100));
          continue;
        }
      }

      // Check if scheduler is paused/stopped
      if (status.load() == SchedlerStatus::paused){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        continue;
      }

      // Execute scheduling algorithm
      if (schedulerType == SchedulingType::roundRobin){
        ScheduleRoundRobin();
      } else if (schedulerType == SchedulingType::fcfs){
        ScheduleFCFS();
      } else if (schedulerType == SchedulingType::priority) {
        SchedulePriority();
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    Logger::log("Scheduler thread finished");
  }).detach();
}

void Scheduler::pause(){
  status = SchedlerStatus::paused;
  std::lock_guard<std::mutex> lock(tasksMutex);
  for(Task& task : queue->getQueue()){
    if (task.getStatus() == TaskStatus::running){
      kill(task.pid, SIGSTOP);
      task.setStatus(TaskStatus::paused);
    }
  }
  Logger::log("Scheduler is paused");
}

void Scheduler::stop(){
  status = SchedlerStatus::stopped;
  Logger::log("Scheduler is stopped");
}

void Scheduler::clearQueue(){
  queue->clear();
}

void Scheduler::ScheduleFCFS(){
  // Run tasks one by one in order
  std::lock_guard<std::mutex> lock(tasksMutex);
  std::vector<Task>& tasks = queue->getQueue();
  if (!tasks.empty()) {
    Task& task = tasks[0];
    if (!task.id.empty() && task.pid > 0) {
      if (task.getStatus() == TaskStatus::finished ||
          task.getStatus() == TaskStatus::killed){
        queue->deleteTask(task.id);
      } else {
        if (task.getStatus() != TaskStatus::running)
          task.setStatus(TaskStatus::running);

        // Check if process has exited
        int processStatus;
        pid_t result = waitpid(task.pid, &processStatus, WNOHANG);
        if (result == task.pid){
          task.setStatus(TaskStatus::finished);
          queue->deleteTask(task.id);
        } else if (result == -1 && errno == ESRCH) {
          task.setStatus(TaskStatus::killed);
          queue->deleteTask(task.id);
        } else {
          if (kill(task.pid, SIGCONT) == -1 && errno == ESRCH) {
            task.setStatus(TaskStatus::killed);
            queue->deleteTask(task.id);
          }
        }
      }
    }
  }
}

void Scheduler::SchedulePriority(){
  // Run tasks one by one in order
  std::lock_guard<std::mutex> lock(tasksMutex);
  
  // Get highest priority task
  Task* task = dynamic_cast<PriorityQueue*>(queue.get())->getHighestPriority();
  if (!task || task->id.empty() || task->pid <= 0) return;

  // Delete task when it is finished or killed externally
  if (task->getStatus() == TaskStatus::finished ||
      task->getStatus() == TaskStatus::killed){
    queue->deleteTask(task->id);
  } else {
    if (task->getStatus() != TaskStatus::running)
      task->setStatus(TaskStatus::running);

    // Check if process has exited
    int processStatus;
    pid_t result = waitpid(task->pid, &processStatus, WNOHANG);
    if (result == task->pid){
      task->setStatus(TaskStatus::finished);
      queue->deleteTask(task->id);
    } else if (result == -1 && errno == ESRCH) {
      task->setStatus(TaskStatus::killed);
      queue->deleteTask(task->id);
    } else {
      if (kill(task->pid, SIGCONT) == -1 && errno == ESRCH) {
        task->setStatus(TaskStatus::killed);
        queue->deleteTask(task->id);
      }
    }
  }
}

void Scheduler::ScheduleRoundRobin(){
  // Run tasks in circular order and execute each by quantum time slice
  std::lock_guard<std::mutex> lock(tasksMutex);
  std::vector<Task>& tasks = queue->getQueue();
  for (size_t i = 0; i < tasks.size(); i++){
    Task& task = tasks[i];
    if (task.id.empty() || task.pid <= 0) continue;

    // Delete task when it is finished or killed externally
    if (task.getStatus() == TaskStatus::finished ||
        task.getStatus() == TaskStatus::killed){
      queue->deleteTask(task.id);
      i--;  // Adjust index since we removed an element
      continue;
    }

    if (task.getStatus() != TaskStatus::running)
      task.setStatus(TaskStatus::running);

    // Check if process has exited
    int processStatus;
    pid_t result = waitpid(task.pid, &processStatus, WNOHANG);
    if (result == task.pid){
      task.setStatus(TaskStatus::finished);
      queue->deleteTask(task.id);
      i--;  // Adjust index since we removed an element
    } else if (result == -1 && errno == ESRCH) {
      // Process doesn't exist
      task.setStatus(TaskStatus::killed);
      queue->deleteTask(task.id);
      i--;  // Adjust index since we removed an element
    } else {
      // Process is still running
      if (kill(task.pid, SIGCONT) == -1 && errno == ESRCH) {
        task.setStatus(TaskStatus::killed);
        queue->deleteTask(task.id);
        i--;  // Adjust index since we removed an element
      } else {
        std::this_thread::sleep_for(std::chrono::milliseconds(constants::ROUND_ROBIN_QUANTUM));
        if (kill(task.pid, SIGSTOP) == -1 && errno == ESRCH) {
          task.setStatus(TaskStatus::killed);
          queue->deleteTask(task.id);
          i--;  // Adjust index since we removed an element
        }
      }
    }
  }
}

std::string Scheduler::getQueueStatus() {
    std::stringstream ss;
    
    ss << "Scheduler type: ";
    ss << constants::SCHEDULER_TYPE_PRETTY_NAMES.at(schedulerType);
    ss << "\n";

    TablePrinter tp;
    tp.addColumn("Order", 7);
    tp.addColumn("Task ID", 40);
    tp.addColumn("Task Command", 50);
    if (schedulerType == SchedulingType::priority) 
      tp.addColumn("Priority", 8);
    tp.addColumn("Status", 10);
    tp.addColumn("Duration", 10);
    tp.addColumn("PID", 8);
    
    ss << tp.getHeader();

    {
      std::lock_guard<std::mutex> lock(tasksMutex);
      int order = 0;
      for(const Task& task : queue->getQueue()) {
        if(task.id.empty()) continue;

        std::string formattedTaskCommand = task.command;
        if (formattedTaskCommand.length() > 48){
          formattedTaskCommand = formattedTaskCommand.substr(0, 45) + "...";
        }
          
        tp << std::to_string(++order) 
            << task.id
            << formattedTaskCommand;
        if (schedulerType == SchedulingType::priority)
          tp << std::to_string(task.priority);
        tp << constants::TASK_STATUS_NAMES.at(task.getStatus())
            << task.getDuration()
            << std::to_string(task.pid);
        ss << tp.getCurrentRow();
        tp.clearCurrentRow();
      }
    }

    ss << tp.getFooter();
    return ss.str();
}
