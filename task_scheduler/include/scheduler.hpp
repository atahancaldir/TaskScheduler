#pragma once

#include "priority_queue.hpp"

enum SchedulingType{
  roundRobin, // Round Robin
  fcfs // First-Come First-Served
};

class Scheduler{
  public:
    Scheduler(SchedulingType);
    std::string getQueueStatus();

  private:
    PriorityQueue pq;
    SchedulingType schedulerType;
};