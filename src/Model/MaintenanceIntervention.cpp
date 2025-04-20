//
// Created by Léo KRYS on 20/04/2025.
//

#include "Model/MaintenanceIntervention.h"

MaintenanceIntervention::MaintenanceIntervention()
    : Intervention() {
}

MaintenanceIntervention::MaintenanceIntervention(const std::string& location, std::time_t date, int duration)
    : Intervention(location, date, duration) {
}

std::string MaintenanceIntervention::getType() const {
    return "Maintenance";
}