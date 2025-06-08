#pragma once

#include "base_queue.hpp"
#include "constants.hpp"
#include <atomic>
#include <mutex>

class Scheduler{
  public:
    Scheduler(SchedulingType);
    SchedulingType getSchedulingType();
    std::string getQueueStatus();
    bool addTask(std::string);
    bool deleteTask(std::string);
    SchedlerStatus getStatus();
    void clearQueue();
    void run();
    void pause();
    void stop();

  private:
    std::unique_ptr<BaseQueue> queue;
    SchedulingType schedulerType;
    std::atomic<SchedlerStatus> status;
    std::mutex tasksMutex;

    void CreateTaskProcesses(Task&);

    void ScheduleFCFS();
    void SchedulePriority();
    void ScheduleRoundRobin();
};