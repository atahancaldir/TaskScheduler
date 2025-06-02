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
#include <chrono>
#include <thread>

Scheduler::Scheduler(SchedulingType schedulerType_) : schedulerType(schedulerType_){
  status = SchedlerStatus::stopped;
  if (schedulerType == SchedulingType::fcfs ||
      schedulerType == SchedulingType::roundRobin){
    queue = new TaskQueue();
  } else {
    queue = new PriorityQueue();
  }
}

Scheduler::~Scheduler(){
  delete queue;
}

SchedulingType Scheduler::getSchedulingType(){
  return schedulerType;
}

bool Scheduler::addTask(std::string taskString){
  Task task(taskString);
  queue->insertTask(task);
  Logger::log("Task is added to queue: " + task.id);
  return true;
}

bool Scheduler::deleteTask(std::string taskID){
  queue->deleteTask(taskID);
  Logger::log("Task is deleted from queue: " + taskID);
  return true;
}

void Scheduler::RoundRobin(){
  Logger::log("Round Robin started");
  while(true){
    {
      std::lock_guard<std::mutex> lock(tasksMutex);
      if (queue->getQueue().empty()) {
        if (status.load() == SchedlerStatus::stopped) {
          Logger::log("Round Robin stopped");
          return;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        continue;
      }
    }

    // Check if scheduler is paused/stopped
    if (status.load() == SchedlerStatus::paused){
      Logger::log("Round Robin paused");
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      continue;
    } else if (status.load() == SchedlerStatus::stopped){
      Logger::log("Round Robin stopped");
      std::lock_guard<std::mutex> lock(tasksMutex);
      for(Task& task : queue->getQueue()){
        if (task.id.empty()) continue;
        kill(task.pid, SIGTERM); // Terminate tasks
      }
      return;
    }

    // Run tasks one by one in quantum time slice
    {
      std::lock_guard<std::mutex> lock(tasksMutex);
      for (size_t i=0; i<queue->getQueue().size(); i++){
        Task& task = queue->getQueue()[i];
        if (task.id.empty()) continue;
        if (task.getStatus() == TaskStatus::finished ||
            task.getStatus() == TaskStatus::killed){
              continue;
        }

        // Check if process has exited
        int processStatus;
        pid_t result = waitpid(task.pid, &processStatus, WNOHANG);
        if (result == task.pid){
          task.setStatus(TaskStatus::finished);
        } else{
          kill(task.pid, SIGCONT); // Continue task
          std::this_thread::sleep_for(std::chrono::milliseconds(constants::ROUND_ROBIN_QUANTUM)); // Wait quantum time slice
          kill(task.pid, SIGSTOP); // Pause task
        }
      }

      // Remove finished tasks
      queue->getQueue().erase(
        std::remove_if(queue->getQueue().begin(), queue->getQueue().end(), 
                      [](const Task& t){
                        return t.id.empty() || 
                        t.getStatus() == TaskStatus::finished || 
                        t.getStatus() == TaskStatus::killed;
        }),
        queue->getQueue().end()
      );
    }
  }
  Logger::log("Round Robin finished");
}

void Scheduler::run(){
  if (status.load() == SchedlerStatus::paused){
    status = SchedlerStatus::running;
    Logger::log("Scheduler is resuming");
    return;
  }
  status = SchedlerStatus::running;
  Logger::log("Scheduler is running");

  // Initialize task executions and pause
  {
    std::lock_guard<std::mutex> lock(tasksMutex);
    for (Task& task : queue->getQueue()){
      if (task.id.empty()) continue;
      pid_t pid = fork();
      if (pid == 0){
        execl("/bin/sh", "sh", "-c", task.command.c_str(), (char *)nullptr);
        exit(1);
      } else {
        kill(pid, SIGSTOP); // Pause task
        task.pid = pid;
        task.setStatus(TaskStatus::running);
      }
    }
  }

  Logger::log("starting thread");
  // Round-Robin scheduling
  if (schedulerType == SchedulingType::roundRobin){
    std::thread([this](){RoundRobin();}).detach();
  }

  Logger::log("Thread started");
}

void Scheduler::pause(){
  status = SchedlerStatus::paused;
  Logger::log("Scheduler is paused");
}

void Scheduler::stop(){
  status = SchedlerStatus::stopped;
  Logger::log("Scheduler is stopped");
}

void Scheduler::clearQueue(){
  queue->clear();
}

std::string Scheduler::getQueueStatus() {
    std::stringstream ss;
    
    ss << "Scheduler type: ";
    ss << constants::SCHEDULER_TYPE_PRETTY_NAMES.at(schedulerType);
    ss << "\n";

    TablePrinter tp;
    tp.addColumn("Order", 5);
    tp.addColumn("Task ID", 40);
    tp.addColumn("Task Command", 50);
    tp.addColumn("Priority", 10);
    tp.addColumn("Status", 10);
    tp.addColumn("Task PID", 10);
    
    ss << tp.getHeader();

    int order = 0;
    for(const Task& task : queue->getQueue()) {
      if(task.id.empty()) continue;

      std::string formattedTaskCommand = task.command;
      if (formattedTaskCommand.length() > 48){
        formattedTaskCommand = formattedTaskCommand.substr(0, 45) + "...";
      }
        
      tp << std::to_string(++order) 
          << task.id
          << formattedTaskCommand
          << std::to_string(task.priority)
          << constants::TASK_STATUS_NAMES.at(task.getStatus())
          << std::to_string(task.pid);
      ss << tp.getCurrentRow();
      tp.clearCurrentRow();
    }

    ss << tp.getFooter();
    return ss.str();
}
