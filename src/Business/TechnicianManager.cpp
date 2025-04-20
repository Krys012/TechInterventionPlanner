//
// Created by Léo KRYS on 20/04/2025.
//

#include "Business/TechnicianManager.h"
#include <algorithm>

TechnicianManager::TechnicianManager() {
    // Initialize if needed
}

bool TechnicianManager::addTechnician(const Technician& technician) {
    // Check if technician already exists
    if (technicians.find(technician.getId()) != technicians.end()) {
        return false;
    }

    // Add the technician
    technicians[technician.getId()] = technician;
    return true;
}

bool TechnicianManager::removeTechnician(const std::string& technicianId) {
    // Check if technician exists
    auto it = technicians.find(technicianId);
    if (it == technicians.end()) {
        return false;
    }

    // Remove the technician
    technicians.erase(it);

    // Remove any assignments for this technician
    assignments.erase(technicianId);

    return true;
}

const Technician* TechnicianManager::getTechnician(const std::string& technicianId) const {
    auto it = technicians.find(technicianId);
    if (it == technicians.end()) {
        return nullptr;
    }

    return &(it->second);
}

const std::map<std::string, Technician>& TechnicianManager::getAllTechnicians() const {
    return technicians;
}

bool TechnicianManager::isTechnicianAvailable(const std::string& technicianId,
                                           std::time_t date, int duration) const {
    // Check if technician exists
    if (technicians.find(technicianId) == technicians.end()) {
        return false;
    }

    // Check if technician has assignments
    auto assignIt = assignments.find(technicianId);
    if (assignIt == assignments.end()) {
        return true; // No assignments, so available
    }

    // Check each assignment for overlap
    const auto& techAssignments = assignIt->second;
    for (const auto& assignment : techAssignments) {
        std::time_t assignedDate = assignment.first;
        int assignedDuration = assignment.second;

        // Check for overlap
        std::time_t assignedEnd = assignedDate + assignedDuration * 60; // Convert minutes to seconds
        std::time_t requestedEnd = date + duration * 60;

        if ((date >= assignedDate && date < assignedEnd) ||
            (assignedDate >= date && assignedDate < requestedEnd)) {
            return false; // Overlap found
        }
    }

    return true; // No overlap, technician is available
}

bool TechnicianManager::assignTechnician(const std::string& technicianId,
                                      std::time_t date, int duration) {
    // Check if technician is available
    if (!isTechnicianAvailable(technicianId, date, duration)) {
        return false;
    }

    // Assign the technician
    assignments[technicianId][date] = duration;
    return true;
}

bool TechnicianManager::unassignTechnician(const std::string& technicianId, std::time_t date) {
    // Check if technician has assignments
    auto assignIt = assignments.find(technicianId);
    if (assignIt == assignments.end()) {
        return false;
    }

    // Check if the specific assignment exists
    auto& techAssignments = assignIt->second;
    auto dateIt = techAssignments.find(date);
    if (dateIt == techAssignments.end()) {
        return false;
    }

    // Remove the assignment
    techAssignments.erase(dateIt);

    // If technician has no more assignments, remove from assignments map
    if (techAssignments.empty()) {
        assignments.erase(assignIt);
    }

    return true;
}

std::vector<std::string> TechnicianManager::findAvailableTechnicians(std::time_t date, int duration,
                                                                  const std::string& specialty) const {
    std::vector<std::string> availableTechnicians;

    // Check each technician
    for (const auto& techPair : technicians) {
        const std::string& techId = techPair.first;
        const Technician& tech = techPair.second;

        // Check specialty if specified
        if (!specialty.empty() && tech.getSpecialty() != specialty) {
            continue;
        }

        // Check availability
        if (isTechnicianAvailable(techId, date, duration)) {
            availableTechnicians.push_back(techId);
        }
    }

    return availableTechnicians;
}