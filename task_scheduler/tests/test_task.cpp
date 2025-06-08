#include <gtest/gtest.h>
#include "task.hpp"
#include "constants.hpp"

TEST(TaskTest, CreateTaskWithDefaultConstructor) {
    Task task;
    EXPECT_TRUE(task.id.empty());
}

TEST(TaskTest, CreateTaskWithCommand) {
    std::string command = "echo test";
    Task task(command);
    
    EXPECT_EQ(task.command, command);
    EXPECT_EQ(task.priority, -1);  // Default priority when not specified
    EXPECT_EQ(task.getStatus(), TaskStatus::pending);
    EXPECT_FALSE(task.id.empty());  // UUID should be generated
    EXPECT_EQ(task.pid, -1);
}

TEST(TaskTest, CreateTaskWithCommandAndPriority) {
    std::string command = "echo test";
    int priority = 5;
    Task task(command, priority);
    
    EXPECT_EQ(task.command, command);
    EXPECT_EQ(task.priority, priority);
    EXPECT_EQ(task.getStatus(), TaskStatus::pending);
    EXPECT_FALSE(task.id.empty());  // UUID should be generated
    EXPECT_EQ(task.pid, -1);
}

TEST(TaskTest, CreateTaskWithTaskString) {
    std::string taskString = "echo hello" + 
                            constants::TASK_BUILDER_DELIMITER + 
                            "3";  // command::priority format
    Task task(taskString);
    
    EXPECT_EQ(task.command, "echo hello");
    EXPECT_EQ(task.priority, 3);
    EXPECT_EQ(task.getStatus(), TaskStatus::pending);
    EXPECT_FALSE(task.id.empty());
    EXPECT_EQ(task.pid, -1);
}

TEST(TaskTest, CreateTaskWithTaskStringNoPriority) {
    std::string taskString = "echo test";  // command without priority
    Task task(taskString);
    
    EXPECT_EQ(task.command, "echo test");
    EXPECT_EQ(task.priority, -1);  // Default when no priority specified
    EXPECT_EQ(task.getStatus(), TaskStatus::pending);
    EXPECT_FALSE(task.id.empty());
    EXPECT_EQ(task.pid, -1);
}

TEST(TaskTest, TaskStatusTransitions) {
    Task task("test_command", 1);
    
    EXPECT_EQ(task.getStatus(), TaskStatus::pending);
    
    task.setStatus(TaskStatus::running);
    EXPECT_EQ(task.getStatus(), TaskStatus::running);
    
    task.setStatus(TaskStatus::finished);
    EXPECT_EQ(task.getStatus(), TaskStatus::finished);
    
    task.setStatus(TaskStatus::killed);
    EXPECT_EQ(task.getStatus(), TaskStatus::killed);
    
    task.setStatus(TaskStatus::paused);
    EXPECT_EQ(task.getStatus(), TaskStatus::paused);
}

TEST(TaskTest, TaskEquality) {
    Task task1("test_command", 1);
    Task task2("test_command", 1);
    
    // Tasks with different IDs should not be equal (UUIDs are unique)
    EXPECT_FALSE(task1 == task2);
    
    // Same task should be equal to itself
    EXPECT_TRUE(task1 == task1);
}

TEST(TaskTest, TaskPriorityComparison) {
    Task highPriority("command", 10);
    Task lowPriority("command", 1);
    
    EXPECT_TRUE(highPriority.priority > lowPriority.priority);
    EXPECT_FALSE(lowPriority.priority > highPriority.priority);
}

TEST(TaskTest, TaskFieldAccess) {
    Task task("ls -la", 8);
    
    // Test direct field access
    EXPECT_EQ(task.command, "ls -la");
    EXPECT_EQ(task.priority, 8);
    EXPECT_EQ(task.pid, -1);
    EXPECT_FALSE(task.id.empty());
    
    // Test that we can modify fields
    task.pid = 1234;
    EXPECT_EQ(task.pid, 1234);
}

TEST(TaskTest, TaskStringParsingWithComplexCommand) {
    std::string taskString = "grep -r 'pattern' /path/to/dir" + 
                            constants::TASK_BUILDER_DELIMITER + 
                            "5";
    Task task(taskString);
    
    EXPECT_EQ(task.command, "grep -r 'pattern' /path/to/dir");
    EXPECT_EQ(task.priority, 5);
}