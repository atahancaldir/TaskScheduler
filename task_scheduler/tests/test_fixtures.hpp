#pragma once

#include <gtest/gtest.h>
#include "task.hpp"
#include "priority_queue.hpp"
#include "scheduler.hpp"
#include "logger.hpp"
#include <chrono>
#include <thread>
#include <filesystem>

// Test fixture for PriorityQueue tests
class PriorityQueueTest : public ::testing::Test {
protected:
    void SetUp() override {
        queue = std::make_unique<PriorityQueue>(10);
    }

    void TearDown() override {
        queue.reset();
    }

    std::unique_ptr<PriorityQueue> queue;
};

// Test fixture for Scheduler tests
class SchedulerTest : public ::testing::Test {
protected:
    void SetUp() override {
        scheduler = std::make_unique<Scheduler>(SchedulingType::priority);
    }

    void TearDown() override {
        if (scheduler) {
            scheduler->stop();
            scheduler.reset();
        }
    }

    std::unique_ptr<Scheduler> scheduler;
};

// Test fixture for Logger tests
class LoggerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Clear any existing log file for clean tests
        if (std::filesystem::exists(logFile)) {
            std::filesystem::remove(logFile);
        }
        std::filesystem::create_directories(logDir);
    }

    void TearDown() override {
        // Clean up log file after tests
        if (std::filesystem::exists(logFile)) {
            std::filesystem::remove(logFile);
        }
    }
}; 