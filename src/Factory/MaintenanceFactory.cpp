//
// Created by Léo KRYS on 20/04/2025.
//

#include "Factory/MaintenanceFactory.h"

std::unique_ptr<Intervention> MaintenanceFactory::createIntervention() const {
    return std::make_unique<MaintenanceIntervention>();
}

std::unique_ptr<Intervention> MaintenanceFactory::createIntervention(
    const std::string& location,
    std::time_t date,
    int duration) const {

    return std::make_unique<MaintenanceIntervention>(location, date, duration);
}