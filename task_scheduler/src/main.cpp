#include "cxxopts.hpp"
#include "logger.hpp"
#include "daemon.hpp"
#include "constants.hpp"
#include <iostream>
#include <functional>
#include <string>
#include <thread>
#include <chrono>

std::string sendCommand(std::string command){
  if (!command.empty()){
    std::string response = Daemon::getInstance().sendCommand(command);
    if (response.empty()){
      std::cout << "Failed to send command to task scheduler" << std::endl;
      exit(EXIT_SUCCESS);
    }
    return response;
  }
  return std::string();
}

int main(int argc, char* argv[]){
  cxxopts::Options options("Task Scheduler", "Task scheduler command line interface");

  options.add_options()
  ("a,add", "Add a new task")
  ("c,clear", "Clear the task queue")
  ("d,delete", "Delete a task", cxxopts::value<std::string>())
  ("h,help", "Print usage")
  ("l,list", "List all tasks")
  ("m,monitor", "Monitor tasks in real-time")
  ("r,run", "Run/resume task scheduler")
  ("p,pause", "Pause task scheduler")
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
        std::cout << "Invalid scheduling algorithm. Options:" << std::endl;
        for(auto& [key, value] : constants::SCHEDULER_TYPE_VALUES){
          std::cout << "> " << key << std::endl;
        }
        return 0;
      }

      if (!daemon.start(schedulingType)){
        std::cout << "Failed to start task scheduler" << std::endl;
        return 1;
      }
      std::cout << "Task scheduler started successfully" << std::endl;
    } else if (result.count("stop")){
      if (!daemon.isRunning()) 
        return 0;
      std::cout << sendCommand("stop") << std::endl;
    }
    else{
      if (!daemon.isRunning()){
        std::cout << "Task scheduler is not running. Start is first with --start" << std::endl;
        return 1;
      }

      std::string command;

      if (result.count("add")){
        command = "add";

        std::string taskCommand;
        std::string taskPriority = std::string();

        SchedulingType schedulingType = constants::SCHEDULER_TYPE_VALUES.at(sendCommand("algorithm"));
        
        std::cout << "Command: ";
        getline(std::cin, taskCommand);
        
        if (schedulingType == SchedulingType::priority){
          std::cout << "Priority #: ";
          getline(std::cin, taskPriority);
        }

        std::stringstream ss;
        ss << command << " " << taskCommand;
        ss << constants::TASK_BUILDER_DELIMITER;
        ss << taskPriority;
        std::cout << sendCommand(ss.str()) << std::endl;
      } else if (result.count("clear")){
        command = "clear";
        std::cout << sendCommand(command) << std::endl;
      } else if (result.count("delete")){
        command = "delete " + result["delete"].as<std::string>();
        std::cout << sendCommand(command) << std::endl;
      } else if (result.count("run")){
        command = "run";
        std::cout << sendCommand(command) << std::endl;
      } else if (result.count("pause")){
        command = "pause";
        std::cout << sendCommand(command) << std::endl;
      } else if (result.count("list")){
        Logger::log("Sending list request");
        command = "list";
        std::cout << sendCommand(command) << std::endl;
      } else if (result.count("monitor")){        
        std::cout << "Monitoring tasks in real-time. Press Ctrl+C to exit." << std::endl;
        while (true) {
          std::cout << "\033[2J\033[1;1H"; // Clear screen and move cursor to top-left
          std::cout << "----------------------------------------" << std::endl;
          std::cout << sendCommand("list") << std::endl;
          std::this_thread::sleep_for(std::chrono::seconds(1));
        }
      } else if (result.count("log")){
        int nLines = result["log"].as<int>();
        Logger::printLogs(nLines);
        return 0;
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