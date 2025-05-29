#include <queue>
#include "priority_queue.hpp"
#include <gtest/gtest.h>

TEST(PriorityQueueTest, CreatingQueue){
  PriorityQueue p(23);
  p.insertTask(Task([](){}, 3));
  p.insertTask(Task([](){}, 2));
  p.insertTask(Task([](){}, 1));

  EXPECT_EQ(p.getHighestPriority()->priority, 3);
}