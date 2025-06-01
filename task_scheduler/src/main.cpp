#include "cxxopts.hpp"
#include "logger.hpp"
#include "daemon.hpp"
#include "constants.hpp"
#include <iostream>
#include <functional>
#include <string>

int main(int argc, char* argv[]){
  cxxopts::Options options("Task Scheduler", "Task scheduler command line interface");

  // add, list, start, stop
  // regular task, timed task (periodic, start time), file-based task

  options.add_options()
  ("a,add", "Add a new task")
  ("c,clear", "Clear the task queue")
  ("d,delete", "Delete a task", cxxopts::value<std::string>())
  ("h,help", "Print usage")
  ("l,list", "List all tasks")
  ("status", "Status of task scheduler")
  ("log", "Show n lines of logs", cxxopts::value<int>()->default_value("10")->implicit_value("10"))
  ("s,start", "Start the task scheduler", cxxopts::value<std::string>()->default_value("fcfs"))
  ("x,stop", "Stop the task scheduler")
  ;

  try{
    auto result = options.parse(argc, argv);

    if (argc <= 1 || result.count("help"))
    {
      std::cout << options.help() << std::endl;
      return 0;
    }

    Daemon& daemon = Daemon::getInstance();

    if (result.count("start")){
      if(daemon.isRunning()){
        std::cout << "Task scheduler is already running, to see details use -l flag" << std::endl;
        return 0;
      }

      std::string algorithm = result["start"].as<std::string>();
      SchedulingType schedulingType;

      if(constants::SCHEDULER_TYPE_VALUES.find(algorithm) != constants::SCHEDULER_TYPE_VALUES.end()){
        schedulingType = constants::SCHEDULER_TYPE_VALUES.at(algorithm);
      } else{
        std::cout << "Invalid scheduling algorithm. Options:";
        for(auto& [key, value] : constants::SCHEDULER_TYPE_VALUES){
          std::cout << key << " |";
        }
        std::cout << std::endl;
        return 1;
      }

      if (!daemon.start(schedulingType)){
        std::cout << "Failed to start task scheduler" << std::endl;
        return 1;
      }
      std::cout << "Task scheduler started successfully" << std::endl;
    } else if (result.count("stop")){
      if (!daemon.isRunning()) 
        return 0;
      daemon.stop();
      std::cout << "Task scheduler stopped" << std::endl;
    }
    else{
      if (!daemon.isRunning()){
        std::cout << "Task scheduler is not running. Start is first with --start" << std::endl;
        return 1;
      }

      std::string command;

      if (result.count("add")){
        command = "add ";
      } else if (result.count("clear")){
        command = "clear";
      } else if (result.count("delete")){
        command = "delete " + result["delete"].as<std::string>();
      } else if (result.count("status")){
        command = "status";
      }else if (result.count("list")){
        command = "list";
      } else if (result.count("log")){
        int nLines = result["log"].as<int>();
        Logger::printLogs(nLines);
        return 0;
      }

      if (!command.empty()){
        if (!daemon.sendCommand(command)){
          std::cout << "Failed to send command to task scheduler" << std::endl;
          return 1;
        }
      }
    }
  } catch(const std::exception& e){
    std::string message = "Argument parsing failed: " + std::string(e.what());
    std::cout << message << std::endl;
    Logger::log(message);
    return 1;
  }

  return 0;
}