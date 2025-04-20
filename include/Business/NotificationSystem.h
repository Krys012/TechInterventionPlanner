//
// Created by Léo KRYS on 20/04/2025.
//

#ifndef NOTIFICATION_SYSTEM_H
#define NOTIFICATION_SYSTEM_H

#include <string>
#include <vector>
#include <map>

/**
 * @class NotificationSystem
 * @brief Handles sending notifications to different recipients
 */
class NotificationSystem {
public:
    /**
     * @brief Enumeration of notification channels
     */
    enum class Channel {
        CONSOLE,
        LOG,
        EMAIL,
        MOBILE
    };

private:
    std::map<std::string, std::vector<Channel>> userPreferences; // Maps user ID to preferred channels
    bool consoleEnabled;
    bool logEnabled;
    std::string logFile;

public:
    /**
     * @brief Constructor
     * @param enableConsole Whether to enable console notifications
     * @param enableLog Whether to enable log notifications
     * @param logFile Path to the log file (if logging is enabled)
     */
    NotificationSystem(bool enableConsole = true, bool enableLog = false,
                     const std::string& logFile = "notifications.log");

    /**
     * @brief Set user notification preferences
     * @param userId ID of the user
     * @param channels Vector of preferred notification channels
     */
    void setUserPreferences(const std::string& userId, const std::vector<Channel>& channels);

    /**
     * @brief Send a notification to a specific user
     * @param userId ID of the recipient
     * @param message Notification message
     * @return True if notification was sent successfully
     */
    bool notifyUser(const std::string& userId, const std::string& message);

    /**
     * @brief Send a notification to a specific technician
     * @param technicianId ID of the technician
     * @param message Notification message
     * @return True if notification was sent successfully
     */
    bool notifyTechnician(const std::string& technicianId, const std::string& message);

    /**
     * @brief Send a broadcast notification to all users
     * @param message Notification message
     * @return Number of recipients that received the notification
     */
    int broadcastNotification(const std::string& message);

    /**
     * @brief Enable or disable console notifications
     * @param enabled Whether to enable console notifications
     */
    void setConsoleEnabled(bool enabled);

    /**
     * @brief Enable or disable log notifications
     * @param enabled Whether to enable log notifications
     * @param logFile Path to the log file (if enabled)
     */
    void setLogEnabled(bool enabled, const std::string& logFile = "");

private:
    /**
     * @brief Send notification to console
     * @param message Notification message
     */
    void sendToConsole(const std::string& message);

    /**
     * @brief Send notification to log
     * @param message Notification message
     */
    void sendToLog(const std::string& message);

    /**
     * @brief Send notification via email (simulation)
     * @param recipient Email address
     * @param message Notification message
     */
    void sendToEmail(const std::string& recipient, const std::string& message);

    /**
     * @brief Send notification to mobile device (simulation)
     * @param recipient Mobile identifier
     * @param message Notification message
     */
    void sendToMobile(const std::string& recipient, const std::string& message);
};

#endif // NOTIFICATION_SYSTEM_H