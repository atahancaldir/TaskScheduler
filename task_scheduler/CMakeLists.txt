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
    src/task_queue.cpp
    src/base_queue.cpp
    src/task.cpp
    src/utils.cpp
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

# Common source files for tests
set(COMMON_TEST_SOURCES
    src/task.cpp
    src/utils.cpp
    src/base_queue.cpp
    src/priority_queue.cpp
    src/task_queue.cpp
    src/scheduler.cpp
)

# Add test executables
add_executable(test_logger 
    tests/test_logger.cpp
    ${COMMON_TEST_SOURCES}
)

add_executable(test_task 
    tests/test_task.cpp
    ${COMMON_TEST_SOURCES}
)

add_executable(test_priority_queue 
    tests/test_priority_queue.cpp
    ${COMMON_TEST_SOURCES}
)

# Link with GTest
target_link_libraries(test_logger 
    PRIVATE 
    GTest::gtest_main
)

target_link_libraries(test_task 
    PRIVATE 
    GTest::gtest_main
)

target_link_libraries(test_priority_queue 
    PRIVATE 
    GTest::gtest_main
)

# Add tests to CTest
add_test(NAME test_logger COMMAND test_logger)
add_test(NAME test_task COMMAND test_task)
add_test(NAME test_priority_queue COMMAND test_priority_queue)

# Set test properties
set_tests_properties(test_logger test_task test_priority_queue
    PROPERTIES
    ENVIRONMENT "GTEST_COLOR=1"
    TIMEOUT 30
)