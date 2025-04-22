#include <iostream>
#include "Factory/InterventionFactoryRegistry.h"
#include "Factory/MaintenanceFactory.h"
#include "Factory/EmergencyFactory.h"
#include "CLI/CLI.h"

int main() {
    // Initialize the registry with our factories
    auto& registry = InterventionFactoryRegistry::getInstance();

    registry.registerFactory("Maintenance", std::make_unique<MaintenanceFactory>());
    registry.registerFactory("Emergency", std::make_unique<EmergencyFactory>(4)); // Default priority 4

    // Create and start the CLI
    CLI cli;
    cli.start();

    return 0;
}