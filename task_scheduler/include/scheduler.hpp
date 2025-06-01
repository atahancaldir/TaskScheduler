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
    bool deleteTask(std::string);
    void clearQueue();
    bool run();
    bool pause();

  private:
    BaseQueue* queue;
    SchedulingType schedulerType;
};