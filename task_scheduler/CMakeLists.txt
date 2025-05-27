cmake_minimum_required(VERSION 3.14)
project(TaskScheduler)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directory
include_directories(include)

# Add executable
add_executable(task_scheduler
    src/main.cpp
    src/task.cpp
    src/utils.cpp
)