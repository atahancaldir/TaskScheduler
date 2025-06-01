#pragma once

#include "priority_queue.hpp"
#include "constants.hpp"
#include <queue>

class Scheduler{
  public:
    Scheduler(SchedulingType);
    SchedulingType getSchedulingType();
    std::string getQueueStatus();

  private:
    PriorityQueue pq;
    // std::queue<Task&> taskQueue;
    SchedulingType schedulerType;
};