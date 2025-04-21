//
// Created by Léo KRYS on 20/04/2025.
//

#include <iostream>
#include <memory>
#include <vector>
#include "Factory/InterventionFactoryRegistry.h"
#include "Factory/MaintenanceFactory.h"
#include "Factory/EmergencyFactory.h"
#include "Model/Intervention.h"
#include "Observer/ConsoleObserver.h"
#include "Observer/LogObserver.h"
#include "Observer/TechnicianNotificationObserver.h"
#include "Business/NotificationSystem.h"

int main() {
    // Initialize the registry with our factories
    auto& registry = InterventionFactoryRegistry::getInstance();

    registry.registerFactory("Maintenance", std::make_unique<MaintenanceFactory>());
    registry.registerFactory("Emergency", std::make_unique<EmergencyFactory>(4)); // Default priority 4

    // Setup observers
    std::vector<std::shared_ptr<InterventionObserver>> observers;

    // Console observer interested in creations and modifications
    observers.push_back(std::make_shared<ConsoleObserver>(
        std::vector<std::string>{"creation", "modification"}
    ));

    // Log observer for all events
    observers.push_back(std::make_shared<LogObserver>("interventions.log"));

    // Setup notification system and technician observer
    auto notificationSystem = std::make_shared<NotificationSystem>(true, true, "notifications.log");
    observers.push_back(std::make_shared<TechnicianNotificationObserver>(notificationSystem));

    // Create interventions
    std::time_t now = std::time(nullptr);

    auto maintenance = registry.createIntervention("Maintenance", "Office Building A", now + 3600, 120);

    // Manually assign a technician ID for demonstration
    if (maintenance) {
        maintenance->setId(1);
        maintenance->setTechnicianId("TECH001");

        // Notify observers about the creation
        for (const auto& observer : observers) {
            if (observer->isInterestedIn("creation")) {
                observer->notify("Intervention created: " + maintenance->getInfo());
            }
        }
    }

    auto emergency = registry.createIntervention("Emergency", "Data Center B", now + 1800, 60);

    if (emergency) {
        emergency->setId(2);
        emergency->setTechnicianId("TECH002");

        // Notify observers about the creation
        for (const auto& observer : observers) {
            if (observer->isInterestedIn("creation")) {
                observer->notify("Intervention created: " + emergency->getInfo());
            }
        }

        // Modify the emergency intervention
        emergency->setStatus("In Progress");

        // Notify observers about the modification
        for (const auto& observer : observers) {
            if (observer->isInterestedIn("modification")) {
                observer->notify("Intervention modified: " + emergency->getInfo());
            }
        }
    }

    return 0;
}