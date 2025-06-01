#include "scheduler.hpp"
#include "table_printer.hpp"
#include "priority_queue.hpp"
#include "task_queue.hpp"
#include "task.hpp"
#include <iostream>
#include <sstream>

Scheduler::Scheduler(SchedulingType schedulerType_) : schedulerType(schedulerType_){
  if (schedulerType == SchedulingType::fcfs){
    queue = new TaskQueue();
  } else if (schedulerType == SchedulingType::roundRobin){
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
  Task* task = new Task(taskString);
  return true;
}

std::string Scheduler::getQueueStatus() {
    std::stringstream ss;
    
    ss << "Scheduler type: ";
    ss << constants::SCHEDULER_TYPE_PRETTY_NAMES.at(schedulerType);
    ss << "\n";

    TablePrinter tp;
    tp.addColumn("Order", 5);
    tp.addColumn("Task ID", 20);
    tp.addColumn("Task Command", 50);
    tp.addColumn("Priority", 10);
    tp.addColumn("Status", 10);
    
    ss << tp.getHeader();

    int order = 0;
    for(const Task& task : queue->getQueue()) {
      if(task.id.empty()) continue;
        
      tp << std::to_string(++order) 
          << task.id
          << task.command 
          << std::to_string(task.priority)
          << constants::TASK_STATUS_NAMES.at(task.getStatus());
      ss << tp.getCurrentRow();
      tp.clearCurrentRow();
    }

    ss << tp.getFooter();
    return ss.str();
}
