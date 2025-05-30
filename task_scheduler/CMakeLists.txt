cmake_minimum_required(VERSION 3.14)
project(TaskScheduler)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directory
include_directories(
  include
  external/cxxopts
)

# Add executable
add_executable(task_scheduler
    src/main.cpp
    src/daemon.cpp
    src/scheduler.cpp
    src/priority_queue.cpp
    src/task.cpp
)

# Add GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

# Enable CTest
enable_testing()

# Add test executables
add_executable(test_task tests/test_task.cpp src/task.cpp)
add_executable(test_priority_queue tests/test_priority_queue.cpp src/task.cpp src/priority_queue.cpp )

# Link with GTest
target_link_libraries(test_task GTest::gtest_main)
target_link_libraries(test_priority_queue GTest::gtest_main)

# Add tests to CTest
add_test(NAME test_task COMMAND test_task)
add_test(NAME test_priority_queue COMMAND test_task)