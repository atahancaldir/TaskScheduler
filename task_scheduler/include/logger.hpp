#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <ctime>
#include <filesystem>
#include <deque>

const std::filesystem::path exePath = std::filesystem::current_path();
const std::filesystem::path logDir = exePath / "logs";
const std::filesystem::path logFile = logDir / "log.txt";

class Logger
{
public:
  static void log(std::string message, bool includeTimestamp=true){
    std::filesystem::create_directories(logDir);

    if(includeTimestamp){
      time_t timestamp = time(NULL);
      struct tm datetime = *localtime(&timestamp);
      char output[50];
      strftime(output, 50, "%m/%d/%y %I:%M:%S %p", &datetime);
      message = std::string(output) + " - " + message;
    }

    std::ofstream logFileStream(logFile, std::ios::app);
    if(!logFileStream.is_open()){
      std::cerr << "Failed to open log file: " << logFile << std::endl;
      return;
    }

    logFileStream << message << "\n";
    logFileStream.close();
  }

  static void printLogs(int lineCount){
    std::ifstream logFileStream(logFile);

    if(!logFileStream.is_open()){
      std::cerr << "Failed to open log file: " << logFile << std::endl;
      return;
    }

    std::string line;
    std::deque<std::string> dq;

    while(std::getline(logFileStream, line)){
      dq.push_front(line);
      if(dq.size() > lineCount) 
        dq.pop_back();
    }
    logFileStream.close();

    for(const auto& l : dq){
      std::cout << l << std::endl;
    }
  }
};