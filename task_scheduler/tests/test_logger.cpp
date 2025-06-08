#include <gtest/gtest.h>
#include "../include/logger.hpp"
#include "test_fixtures.hpp"
#include <fstream>
#include <sstream>

TEST_F(LoggerTest, LoggingWithTimestamp) {
    std::string testMessage = "Timestamp test message";
    Logger::log(testMessage, true);
    
    std::ifstream logFileStream(logFile);
    std::string line;
    std::getline(logFileStream, line);
    
    // Check if line contains timestamp format (MM/DD/YY HH:MM:SS AM/PM)
    EXPECT_TRUE(line.find("/") != std::string::npos);
    EXPECT_TRUE(line.find(":") != std::string::npos);
    EXPECT_TRUE(line.find(testMessage) != std::string::npos);
}

TEST_F(LoggerTest, LoggingWithoutTimestamp) {
    std::string testMessage = "No timestamp message";
    Logger::log(testMessage, false);
    
    std::ifstream logFileStream(logFile);
    std::string line;
    std::getline(logFileStream, line);
    
    EXPECT_EQ(line, testMessage);
}

TEST_F(LoggerTest, MultipleLogEntries) {
    std::vector<std::string> messages = {
        "First message",
        "Second message",
        "Third message"
    };
    
    for (const auto& msg : messages) {
        Logger::log(msg, false);  // No timestamp for easier testing
    }
    
    std::ifstream logFileStream(logFile);
    std::string line;
    std::vector<std::string> loggedMessages;
    
    while (std::getline(logFileStream, line)) {
        loggedMessages.push_back(line);
    }
    
    EXPECT_EQ(loggedMessages.size(), messages.size());
    for (size_t i = 0; i < messages.size(); i++) {
        EXPECT_EQ(loggedMessages[i], messages[i]);
    }
}

TEST_F(LoggerTest, PrintLastNLines) {
    // Add 10 log entries
    for (int i = 0; i < 10; i++) {
        Logger::log("Message " + std::to_string(i));
    }
    
    // Capture cout output
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    // Print last 3 lines
    Logger::printLogs(3);
    
    // Restore cout
    std::cout.rdbuf(old);
    
    std::string output = buffer.str();
    EXPECT_TRUE(output.find("Message 7") != std::string::npos);
    EXPECT_TRUE(output.find("Message 8") != std::string::npos);
    EXPECT_TRUE(output.find("Message 9") != std::string::npos);
    EXPECT_TRUE(output.find("Message 6") == std::string::npos);
}

TEST_F(LoggerTest, PrintMoreLinesThanAvailable) {
    Logger::log("Single message");
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    Logger::printLogs(5);  // Try to print more lines than available
    
    std::cout.rdbuf(old);
    
    std::string output = buffer.str();
    EXPECT_TRUE(output.find("Single message") != std::string::npos);
}

TEST_F(LoggerTest, LogDirectoryCreation) {
    // Remove log directory if it exists
    std::filesystem::remove_all(logDir);
    
    Logger::log("Test message");
    
    EXPECT_TRUE(std::filesystem::exists(logDir));
    EXPECT_TRUE(std::filesystem::is_directory(logDir));
    EXPECT_TRUE(std::filesystem::exists(logFile));
}

TEST_F(LoggerTest, LogFilePermissions) {
    Logger::log("Test message");
    
    std::filesystem::permissions(logFile, 
        std::filesystem::perms::owner_read | std::filesystem::perms::owner_write);
    
    EXPECT_TRUE(std::filesystem::exists(logFile));
    EXPECT_TRUE(std::filesystem::is_regular_file(logFile));
}