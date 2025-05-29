#include "scheduler.hpp"
#include "table_printer.hpp"
#include "task.hpp"
#include <iostream>

Scheduler::Scheduler(SchedulingType schedulerType_) : schedulerType(schedulerType_){
  pq = PriorityQueue();
}

void Scheduler::prettyPrintQueue(){
  TablePrinter tp;
  tp.addColumn("Task Name", 50);
  tp.addColumn("Task ID", 50);
  tp.addColumn("Priority", 20);
  tp.addColumn("Status", 20);
  tp.printHeader();

  for(const Task& task : pq.getQueue()){
    if(task.id.empty()) continue;
    tp << task.name << task.id << std::to_string(task.priority);
    switch (task.getStatus()){
      case pending: tp << "pending"; break;
      case running: tp << "running"; break;
      case finished: tp << "finished"; break;
      case killed: tp << "killed"; break;
      case failed: tp << "failed"; break;
      default: tp << "unknown";
    }
  }

  tp.printFooter();
}
