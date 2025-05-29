#include "cxxopts.hpp"
#include <iostream>
#include <functional>
#include <string>

int main(int argc, char* argv[]){
  cxxopts::Options options("Task Scheduler", "Task scheduler command line interface");

  // add, list, start, stop
  // regular task, timed task (periodic, start time), file-based task

  options.add_options()
  ("a,add", "Add a new task", cxxopts::value<std::string>())
  ("c,clear", "Clear the task queue")
  ("d,delete", "Delete a task", cxxopts::value<std::string>())
  ("h,help", "Print usage")
  ("l,list", "List all tasks")
  ("log", "Show n lines of logs", cxxopts::value<int>())
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
  } catch(const std::exception& e){
    std::cout << "Argument parsing failed: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}