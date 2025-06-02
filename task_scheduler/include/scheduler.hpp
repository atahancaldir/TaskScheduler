#pragma once

#include "base_queue.hpp"
#include "constants.hpp"
#include <atomic>

class Scheduler{
  public:
    Scheduler(SchedulingType);
    ~Scheduler();
    SchedulingType getSchedulingType();
    std::string getQueueStatus();
    bool addTask(std::string);
    bool deleteTask(std::string);
    void clearQueue();
    void run();
    void pause();
    void stop();

  private:
    BaseQueue* queue;
    SchedulingType schedulerType;
    std::atomic<SchedlerStatus> status;
    std::mutex tasksMutex;

    void RoundRobin();
};