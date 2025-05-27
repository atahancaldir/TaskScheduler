#include <gtest/gtest.h>
#include "../include/task.hpp"
#include <functional>

TEST(TaskTest, CreateTask){
  std::function<void()> f = [](){};
  Task task = Task(f, 4);
  EXPECT_EQ(task.priority, 4);
}