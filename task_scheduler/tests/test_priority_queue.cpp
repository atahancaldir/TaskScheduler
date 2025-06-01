#include <queue>
#include "priority_queue.hpp"
#include <gtest/gtest.h>

TEST(PriorityQueueTest, CreatingQueue){
  PriorityQueue pq(23);
  pq.insertTask(Task("task 1", [](){}, 3));
  pq.insertTask(Task("task 2", [](){}, 2));
  pq.insertTask(Task("task 3", [](){}, 1));

  EXPECT_EQ(pq.getHighestPriority()->priority, 3);
}