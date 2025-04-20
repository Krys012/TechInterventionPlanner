//
// Created by Léo KRYS on 20/04/2025.
//

#include "Business/NotificationSystem.h"
#include <iostream>
#include <fstream>
#include <ctime>

NotificationSystem::NotificationSystem(bool enableConsole, bool enableLog, const std::string& logFile)
    : consoleEnabled(enableConsole), logEnabled(enableLog), logFile(logFile) {
}

void NotificationSystem::setUserPreferences(const std::string& userId,
                                         const std::vector<Channel>& channels) {
    userPreferences[userId] = channels;
}

bool NotificationSystem::notifyUser(const std::string& userId, const std::string& message) {
    // Check if user has preferences
    auto it = userPreferences.find(userId);
    if (it == userPreferences.end()) {
        // Default to console notification if no preferences
        if (consoleEnabled) {
            sendToConsole("[User: " + userId + "] " + message);
            return true;
        }
        return false;
    }

    // Send to each preferred channel
    const auto& channels = it->second;
    bool notified = false;

    for (const auto& channel : channels) {
        switch (channel) {
            case Channel::CONSOLE:
                if (consoleEnabled) {
                    sendToConsole("[User: " + userId + "] " + message);
                    notified = true;
                }
                break;
            case Channel::LOG:
                if (logEnabled) {
                    sendToLog("[User: " + userId + "] " + message);
                    notified = true;
                }
                break;
            case Channel::EMAIL:
                sendToEmail(userId + "@company.com", message);
                notified = true;
                break;
            case Channel::MOBILE:
                sendToMobile(userId, message);
                notified = true;
                break;
        }
    }

    return notified;
}

bool NotificationSystem::notifyTechnician(const std::string& technicianId, const std::string& message) {
    // Similar to notifyUser, but specific for technicians
    // For simplicity, we'll use the same mechanism
    return notifyUser(technicianId, message);
}

int NotificationSystem::broadcastNotification(const std::string& message) {
    int notifiedCount = 0;

    // Notify all users with preferences
    for (const auto& userPref : userPreferences) {
        if (notifyUser(userPref.first, message)) {
            notifiedCount++;
        }
    }

    // If no specific users, but console is enabled, send there
    if (notifiedCount == 0 && consoleEnabled) {
        sendToConsole("[BROADCAST] " + message);
        notifiedCount = 1;
    }

    return notifiedCount;
}

void NotificationSystem::setConsoleEnabled(bool enabled) {
    consoleEnabled = enabled;
}

void NotificationSystem::setLogEnabled(bool enabled, const std::string& newLogFile) {
    logEnabled = enabled;
    if (!newLogFile.empty()) {
        logFile = newLogFile;
    }
}

void NotificationSystem::sendToConsole(const std::string& message) {
    std::cout << "NOTIFICATION: " << message << std::endl;
}

void NotificationSystem::sendToLog(const std::string& message) {
    std::ofstream log(logFile, std::ios::app);
    if (log.is_open()) {
        // Get current time
        std::time_t now = std::time(nullptr);
        struct tm* timeinfo = std::localtime(&now);
        char timestamp[20];
        std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);

        log << "[" << timestamp << "] " << message << std::endl;
        log.close();
    }
}

void NotificationSystem::sendToEmail(const std::string& recipient, const std::string& message) {
    // This is a simulation - in a real app, it would connect to an email service
    std::cout << "Simulating email to " << recipient << ": " << message << std::endl;

    // For a real implementation, you might want to use a library like libcurl or a mail API
}

void NotificationSystem::sendToMobile(const std::string& recipient, const std::string& message) {
    // This is a simulation - in a real app, it would connect to a push notification service
    std::cout << "Simulating mobile push to " << recipient << ": " << message << std::endl;

    // For a real implementation, you might want to use Firebase Cloud Messaging or similar
}