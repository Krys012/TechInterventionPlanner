//
// Created by Léo KRYS on 21/04/2025.
//

#ifndef TECHNICIAN_NOTIFICATION_OBSERVER_H
#define TECHNICIAN_NOTIFICATION_OBSERVER_H

#include "Observer/InterventionObserver.h"
#include "Business/NotificationSystem.h"
#include <memory>

/**
 * @class TechnicianNotificationObserver
 * @brief Observer that sends notifications to technicians
 */
class TechnicianNotificationObserver : public InterventionObserver {
private:
    std::shared_ptr<NotificationSystem> notificationSystem;

public:
    /**
     * @brief Constructor
     * @param notificationSystem Shared pointer to the notification system
     */
    explicit TechnicianNotificationObserver(std::shared_ptr<NotificationSystem> notificationSystem);

    /**
     * @brief Sends notifications to relevant technicians
     * @param message Message describing the event
     */
    void notify(const std::string& message) override;
};

#endif // TECHNICIAN_NOTIFICATION_OBSERVER_H
