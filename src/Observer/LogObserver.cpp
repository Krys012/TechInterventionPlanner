//
// Created by Léo KRYS on 21/04/2025.
//

#include "Observer/LogObserver.h"
#include <ctime>
#include <iostream>

LogObserver::LogObserver(const std::string& logFilePath)
    : logFilePath(logFilePath) {
}

void LogObserver::notify(const std::string& message) {
    try {
        // Open log file in append mode
        std::ofstream logFile(logFilePath, std::ios::app);

        if (logFile.is_open()) {
            // Get current timestamp
            std::time_t now = std::time(nullptr);
            char timestamp[25];
            std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

            // Write timestamped message to log file
            logFile << "[" << timestamp << "] " << message << std::endl;
            logFile.close();
        } else {
            // Handle error - could not open log file
            std::cerr << "Error: Could not open log file: " << logFilePath << std::endl;
        }
    } catch (const std::exception& e) {
        // Handle any exceptions during logging
        std::cerr << "Error during logging: " << e.what() << std::endl;
    }
}