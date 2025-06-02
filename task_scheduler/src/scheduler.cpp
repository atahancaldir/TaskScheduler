#include "scheduler.hpp"
#include "table_printer.hpp"
#include "priority_queue.hpp"
#include "task_queue.hpp"
#include "task.hpp"
#include "logger.hpp"
#include <iostream>
#include <sstream>

Scheduler::Scheduler(SchedulingType schedulerType_) : schedulerType(schedulerType_){
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

bool Scheduler::run(){

}

bool Scheduler::pause(){

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
          << constants::TASK_STATUS_NAMES.at(task.getStatus());
      ss << tp.getCurrentRow();
      tp.clearCurrentRow();
    }

    ss << tp.getFooter();
    return ss.str();
}
