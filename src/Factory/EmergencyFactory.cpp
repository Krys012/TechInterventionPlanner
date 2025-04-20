//
// Created by Léo KRYS on 20/04/2025.
//

#include "Factory/EmergencyFactory.h"

EmergencyFactory::EmergencyFactory(int defaultPriority)
    : defaultPriority(defaultPriority) {
    // Validate priority range
    if (this->defaultPriority < 1) this->defaultPriority = 1;
    else if (this->defaultPriority > 5) this->defaultPriority = 5;
}

std::unique_ptr<Intervention> EmergencyFactory::createIntervention() const {
    return std::make_unique<EmergencyIntervention>();
}

std::unique_ptr<Intervention> EmergencyFactory::createIntervention(
    const std::string& location,
    std::time_t date,
    int duration) const {

    return std::make_unique<EmergencyIntervention>(location, date, duration, defaultPriority);
}

std::unique_ptr<Intervention> EmergencyFactory::createEmergencyIntervention(
    const std::string& location,
    std::time_t date,
    int duration,
    int priority) const {

    return std::make_unique<EmergencyIntervention>(location, date, duration, priority);
}