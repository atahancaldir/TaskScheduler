#pragma once

#include "base_queue.hpp"
#include "constants.hpp"

class Scheduler{
  public:
    Scheduler(SchedulingType);
    ~Scheduler();
    SchedulingType getSchedulingType();
    std::string getQueueStatus();
    bool addTask(std::string);

  private:
    BaseQueue* queue;
    SchedulingType schedulerType;
};