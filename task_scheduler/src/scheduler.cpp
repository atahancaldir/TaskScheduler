#include "scheduler.hpp"
#include "table_printer.hpp"
#include "task.hpp"
#include <iostream>
#include <sstream>

Scheduler::Scheduler(SchedulingType schedulerType_) : schedulerType(schedulerType_){
  pq = PriorityQueue();
}

std::string Scheduler::getQueueStatus() {
    std::stringstream ss;
    
    ss << "Scheduler type: ";
    switch(schedulerType) {
        case roundRobin: ss << "Round Robin"; break;
        case fcfs: ss << "First-Come First-Served"; break;
        default: ss << "Undefined";
    }
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
           << std::to_string(task.priority);
        
        std::string status;
        switch (task.getStatus()) {
            case pending: status = "pending"; break;
            case running: status = "running"; break;
            case finished: status = "finished"; break;
            case killed: status = "killed"; break;
            case failed: status = "failed"; break;
            default: status = "unknown";
        }
        tp << status;
        
        ss << tp.getCurrentRow();
        tp.clearCurrentRow();
    }

    ss << tp.getFooter();
    return ss.str();
}
