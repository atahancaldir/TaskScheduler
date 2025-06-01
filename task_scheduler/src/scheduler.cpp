#include "scheduler.hpp"
#include "table_printer.hpp"
#include "task.hpp"
#include <iostream>
#include <sstream>

Scheduler::Scheduler(SchedulingType schedulerType_) : schedulerType(schedulerType_){
  pq = PriorityQueue();
}

SchedulingType Scheduler::getSchedulingType(){
  return schedulerType;
}

std::string Scheduler::getQueueStatus() {
    std::stringstream ss;
    
    ss << "Scheduler type: ";
    ss << constants::SCHEDULER_TYPE_NAMES.at(schedulerType);
    ss << "\n";

    TablePrinter tp;
    tp.addColumn("Order", 5);
    tp.addColumn("Task Name", 20);
    tp.addColumn("Task ID", 20);
    tp.addColumn("Priority", 10);
    tp.addColumn("Status", 10);
    
    ss << tp.getHeader();

    int order = 0;
    for(const Task& task : pq.getQueue()) {
      if(task.id.empty()) continue;
        
      tp << std::to_string(++order) 
          << task.name 
          << task.id 
          << std::to_string(task.priority)
          << constants::TASK_STATUS_NAMES.at(task.getStatus());
      ss << tp.getCurrentRow();
      tp.clearCurrentRow();
    }

    ss << tp.getFooter();
    return ss.str();
}
