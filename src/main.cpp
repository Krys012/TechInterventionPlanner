//
// Created by Léo KRYS on 20/04/2025.
//

#include <iostream>
#include <memory>
#include "Factory/InterventionFactoryRegistry.h"
#include "Factory/MaintenanceFactory.h"
#include "Factory/EmergencyFactory.h"
#include "Model/Intervention.h"

int main() {
    // Initialize the registry with our factories
    auto& registry = InterventionFactoryRegistry::getInstance();

    registry.registerFactory("Maintenance", std::make_unique<MaintenanceFactory>());
    registry.registerFactory("Emergency", std::make_unique<EmergencyFactory>(4)); // Default priority 4

    // Use the registry to create interventions
    std::time_t now = std::time(nullptr);

    auto maintenance = registry.createIntervention("Maintenance", "Office Building A", now + 3600, 120);
    auto emergency = registry.createIntervention("Emergency", "Data Center B", now + 1800, 60);

    // Display information about the created interventions
    if (maintenance) {
        std::cout << "Created: " << maintenance->getInfo() << std::endl << std::endl;
    }

    if (emergency) {
        std::cout << "Created: " << emergency->getInfo() << std::endl;
    }

    return 0;
}