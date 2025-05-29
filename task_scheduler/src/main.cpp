#include "cxxopts.hpp"
#include "logger.hpp"
#include <iostream>
#include <functional>
#include <string>

int main(int argc, char* argv[]){
  cxxopts::Options options("Task Scheduler", "Task scheduler command line interface");

  // add, list, start, stop
  // regular task, timed task (periodic, start time), file-based task

  options.add_options()
  ("a,add", "Add a new task (type command in quotation marks)", cxxopts::value<std::string>())
  ("c,clear", "Clear the task queue")
  ("d,delete", "Delete a task", cxxopts::value<std::string>())
  ("h,help", "Print usage")
  ("l,list", "List all tasks")
  ("log", "Show n lines of logs", cxxopts::value<int>()->default_value("10")->implicit_value("10"))
  ("s,start", "Start the task scheduler")
  ("x,stop", "Stop the task scheduler")
  ;

  try{
    auto result = options.parse(argc, argv);

    if (argc <= 1 || result.count("help"))
    {
      std::cout << options.help() << std::endl;
      return 0;
    }

    else if (result.count("add")){
      std::string command = result["add"].as<std::string>();
    }

    else if (result.count("clear")){}
    
    else if (result.count("delete")){
      std::string taskId = result["delete"].as<std::string>();
    }

    else if (result.count("list")){}

    else if (result.count("log")){
      int nLines = result["log"].as<int>();
      Logger::printLogs(nLines);
    }
    
    else if (result.count("start")){}
    
    else if (result.count("stop")){}

  } catch(const std::exception& e){
    std::string message = "Argument parsing failed: " + std::string(e.what());
    std::cout << message << std::endl;
    Logger::log(message);
    return 1;
  }

  return 0;
}