#pragma once

#include "priority_queue.hpp"

enum SchedulingType{
  roundRobin, // Round Robin
  fcfs // First-Come First-Served
};

class Scheduler{
  public:
    Scheduler(SchedulingType);
    void prettyPrintQueue();

  private:
    PriorityQueue pq;
    SchedulingType schedulerType;
};