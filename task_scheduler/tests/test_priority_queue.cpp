#include <queue>
#include "priority_queue.hpp"
#include <gtest/gtest.h>
#include "test_fixtures.hpp"
#include <vector>
#include <algorithm>

TEST_F(PriorityQueueTest, CreateEmptyQueue) {
    EXPECT_TRUE(queue->isEmpty());
    EXPECT_EQ(queue->getSize(), 0);
}

TEST_F(PriorityQueueTest, InsertAndExtractSingleTask) {
    Task task("echo test", 5);
    queue->insertTask(task);
    
    EXPECT_FALSE(queue->isEmpty());
    EXPECT_EQ(queue->getSize(), 1);
    
    Task* highestPriority = queue->getHighestPriority();
    EXPECT_EQ(highestPriority->priority, 5);
    EXPECT_EQ(highestPriority->command, "echo test");
}

TEST_F(PriorityQueueTest, MaintainHeapProperty) {
    std::vector<int> priorities = {3, 1, 4, 2, 5};
    for (int priority : priorities) {
        queue->insertTask(Task("echo test " + std::to_string(priority), priority));
    }
    
    std::vector<int> extractedPriorities;
    while (!queue->isEmpty()) {
        Task task = queue->extractHighestPriority();
        extractedPriorities.push_back(task.priority);
    }
    
    std::vector<int> expectedOrder = {5, 4, 3, 2, 1};
    EXPECT_EQ(extractedPriorities, expectedOrder);
}

TEST_F(PriorityQueueTest, DeleteTask) {
    Task task1("echo test 1", 3);
    Task task2("echo test 2", 2);
    Task task3("echo test 3", 1);
    
    queue->insertTask(task1);
    queue->insertTask(task2);
    queue->insertTask(task3);
    
    queue->deleteTask(task1.id);
    EXPECT_EQ(queue->getSize(), 2);
    
    Task* highest = queue->getHighestPriority();
    EXPECT_EQ(highest->command, "echo test 2");
    EXPECT_EQ(highest->priority, 2);
}

TEST_F(PriorityQueueTest, QueueResizing) {
    for (int i = 0; i < 15; i++) {
        queue->insertTask(Task("echo test " + std::to_string(i), i));
    }
    
    EXPECT_EQ(queue->getSize(), 15);
    Task* highest = queue->getHighestPriority();
    EXPECT_EQ(highest->priority, 14);
}

TEST_F(PriorityQueueTest, ExtractFromEmptyQueue) {
    EXPECT_THROW(queue->extractHighestPriority(), std::runtime_error);
}

TEST_F(PriorityQueueTest, DeleteNonExistentTask) {
    queue->insertTask(Task("echo test 1", 1));
    queue->deleteTask("non_existent");
    EXPECT_EQ(queue->getSize(), 1);
}

TEST_F(PriorityQueueTest, MultipleTasksSamePriority) {
    queue->insertTask(Task("echo test 1", 1));
    queue->insertTask(Task("echo test 2", 1));
    queue->insertTask(Task("echo test 3", 1));
    
    EXPECT_EQ(queue->getSize(), 3);
    Task* highest = queue->getHighestPriority();
    EXPECT_EQ(highest->priority, 1);
}

TEST_F(PriorityQueueTest, ClearQueue) {
    queue->insertTask(Task("echo test 1", 1));
    queue->insertTask(Task("echo test 2", 2));
    queue->insertTask(Task("echo test 3", 3));
    
    queue->clear();
    EXPECT_TRUE(queue->isEmpty());
    EXPECT_EQ(queue->getSize(), 0);
}