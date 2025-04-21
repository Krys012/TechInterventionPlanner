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
#include "Decorator/GPSTrackingDecorator.h"
#include "Decorator/AttachmentsDecorator.h"

int main() {
    // Initialize the registry with our factories
    auto& registry = InterventionFactoryRegistry::getInstance();

    registry.registerFactory("Maintenance", std::make_unique<MaintenanceFactory>());
    registry.registerFactory("Emergency", std::make_unique<EmergencyFactory>(4)); // Default priority 4

    // Setup observers
    std::vector<std::shared_ptr<InterventionObserver>> observers;
    observers.push_back(std::make_shared<ConsoleObserver>());
    observers.push_back(std::make_shared<LogObserver>("interventions.log"));

    // Create interventions
    std::time_t now = std::time(nullptr);

    // Create a basic maintenance intervention
    auto maintenance = registry.createIntervention("Maintenance", "Office Building A", now + 3600, 120);
    maintenance->setId(1);
    maintenance->setTechnicianId("TECH001");

    // Notify observers about the basic intervention
    for (const auto& observer : observers) {
        observer->notify("Basic intervention created: " + maintenance->getInfo());
    }

    // Add GPS tracking to the maintenance intervention
    auto maintenanceWithGPS = std::make_unique<GPSTrackingDecorator>(std::move(maintenance), 10);

    // Add some tracking points
    maintenanceWithGPS->addCoordinate(40.7128, -74.0060); // New York
    maintenanceWithGPS->addCoordinate(40.7129, -74.0062); // Slight movement

    // Notify observers about the enhanced intervention
    for (const auto& observer : observers) {
        observer->notify("Added GPS tracking: " + maintenanceWithGPS->getInfo());
    }

    // Create an emergency intervention with both GPS tracking and attachments
    auto emergency = registry.createIntervention("Emergency", "Data Center B", now + 1800, 60);
    emergency->setId(2);
    emergency->setTechnicianId("TECH002");

    // First, add GPS tracking
    auto emergencyWithGPS = std::make_unique<GPSTrackingDecorator>(std::move(emergency), 5);
    emergencyWithGPS->addCoordinate(37.7749, -122.4194); // San Francisco

    // Then, add attachments capability
    auto emergencyWithGPSAndAttachments =
        std::make_unique<AttachmentsDecorator>(std::move(emergencyWithGPS));

    // Add some attachments
    emergencyWithGPSAndAttachments->addAttachment("server_diagram.pdf", "Data center layout");
    emergencyWithGPSAndAttachments->addAttachment("error_logs.txt", "Server error logs");

    // Notify observers about the fully decorated intervention
    for (const auto& observer : observers) {
        observer->notify("Created emergency with GPS and attachments: " +
                        emergencyWithGPSAndAttachments->getInfo());
    }

    return 0;
}