//
// Created by Léo KRYS on 21/04/2025.
//


#include "Observer/TechnicianNotificationObserver.h"
#include <regex>
#include <string>

TechnicianNotificationObserver::TechnicianNotificationObserver(std::shared_ptr<NotificationSystem> notificationSystem)
    : notificationSystem(notificationSystem) {
}

void TechnicianNotificationObserver::notify(const std::string& message) {
    if (!notificationSystem) {
        return; // No notification system available
    }

    // Extract technician ID from the message if present
    // This is a simple approach - in a real system, we might pass structured data instead of strings
    std::regex technicianPattern("Technician: ([A-Z0-9]+)");
    std::smatch match;

    if (std::regex_search(message, match, technicianPattern) && match.size() > 1) {
        std::string technicianId = match[1].str();

        // Notify the technician
        notificationSystem->notifyTechnician(technicianId, message);
    } else {
        // If no specific technician found, broadcast to all
        notificationSystem->broadcastNotification("General notification: " + message);
    }
}