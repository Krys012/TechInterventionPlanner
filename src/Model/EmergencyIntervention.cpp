//
// Created by Léo KRYS on 20/04/2025.
//


#include "Model/EmergencyIntervention.h"
#include <sstream>

EmergencyIntervention::EmergencyIntervention()
    : Intervention(), priority(3) {
}

EmergencyIntervention::EmergencyIntervention(const std::string& location, std::time_t date, int duration, int priority)
    : Intervention(location, date, duration), priority(priority) {
    // Validate priority between 1 and 5
    if (priority < 1) this->priority = 1;
    else if (priority > 5) this->priority = 5;
    else this->priority = priority;
}

std::string EmergencyIntervention::getType() const {
    return "Emergency";
}

int EmergencyIntervention::getPriority() const {
    return priority;
}

void EmergencyIntervention::setPriority(int priority) {
    if (priority < 1) this->priority = 1;
    else if (priority > 5) this->priority = 5;
    else this->priority = priority;
}

std::string EmergencyIntervention::getInfo() const {
    // Get base information
    std::string baseInfo = Intervention::getInfo();

    // Add priority information
    std::ostringstream oss;
    oss << baseInfo << std::endl
        << "Priority: " << priority << "/5";

    return oss.str();
}