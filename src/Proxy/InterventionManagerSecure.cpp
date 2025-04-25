//
// Created by Léo KRYS on 21/04/2025.
//

#include "Proxy/InterventionManagerSecure.h"
#include <iostream>
#include <fstream>
#include <ctime>

InterventionManagerSecure::InterventionManagerSecure(const std::string& currentUser)
    : realManager(std::make_unique<InterventionManager>()), currentUser(currentUser) {

    // Initialize default access levels (in a real app, this would come from a database)
    userAccessLevels["admin"] = AccessLevel::FULL;
    userAccessLevels["manager"] = AccessLevel::MODIFY;
    userAccessLevels["technician"] = AccessLevel::READ;
    userAccessLevels["guest"] = AccessLevel::READ;

    // If the user doesn't exist, set their access level to NONE
    if (userAccessLevels.find(currentUser) == userAccessLevels.end()) {
        userAccessLevels[currentUser] = AccessLevel::NONE;
    }
}

void InterventionManagerSecure::setUserAccessLevel(const std::string& userId, AccessLevel level) {
    // Only admin can change access levels
    if (getCurrentUserAccessLevel() != AccessLevel::FULL) {
        logAccessAttempt("setUserAccessLevel", false);
        return;
    }

    userAccessLevels[userId] = level;
    logAccessAttempt("setUserAccessLevel", true);
}

InterventionManagerSecure::AccessLevel InterventionManagerSecure::getCurrentUserAccessLevel() const {
    auto it = userAccessLevels.find(currentUser);
    if (it != userAccessLevels.end()) {
        return it->second;
    }
    return AccessLevel::NONE;
}

bool InterventionManagerSecure::checkAccess(AccessLevel requiredLevel) const {
    AccessLevel userLevel = getCurrentUserAccessLevel();

    // Convert enum to int for comparison
    return static_cast<int>(userLevel) >= static_cast<int>(requiredLevel);
}

void InterventionManagerSecure::logAccessAttempt(const std::string& operation, bool granted) const {
    // Generate timestamp
    std::time_t now = std::time(nullptr);
    char timestamp[25];
    std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

    // Log to console
    std::cout << "log [ACCESS] [" << timestamp << "] User '" << currentUser
              << "' attempted '" << operation << "' - "
              << (granted ? "GRANTED" : "DENIED") << std::endl;

    // In a real application, this could also log to a file or database
    std::ofstream logFile("access_log.txt", std::ios::app);
    if (logFile.is_open()) {
        logFile << "[" << timestamp << "] User '" << currentUser
                << "' attempted '" << operation << "' - "
                << (granted ? "GRANTED" : "DENIED") << std::endl;
        logFile.close();
    }
}

// IInterventionManager interface implementation

int InterventionManagerSecure::createIntervention(const std::string& type,
                                               const std::string& location,
                                               std::time_t date,
                                               int duration) {
    if (!checkAccess(AccessLevel::FULL)) {
        logAccessAttempt("createIntervention", false);
        return -1;
    }

    logAccessAttempt("createIntervention", true);
    return realManager->createIntervention(type, location, date, duration);
}

bool InterventionManagerSecure::deleteIntervention(int interventionId) {
    if (!checkAccess(AccessLevel::FULL)) {
        logAccessAttempt("deleteIntervention", false);
        return false;
    }

    logAccessAttempt("deleteIntervention", true);
    return realManager->deleteIntervention(interventionId);
}

bool InterventionManagerSecure::modifyIntervention(int interventionId,
                                                const std::string& location,
                                                std::time_t date,
                                                int duration) {
    if (!checkAccess(AccessLevel::MODIFY)) {
        logAccessAttempt("modifyIntervention", false);
        return false;
    }

    logAccessAttempt("modifyIntervention", true);
    return realManager->modifyIntervention(interventionId, location, date, duration);
}

bool InterventionManagerSecure::changeInterventionStatus(int interventionId,
                                                      const std::string& status,
                                                      const std::string& comments) {
    if (!checkAccess(AccessLevel::MODIFY)) {
        logAccessAttempt("changeInterventionStatus", false);
        return false;
    }

    logAccessAttempt("changeInterventionStatus", true);
    return realManager->changeInterventionStatus(interventionId, status, comments);
}

bool InterventionManagerSecure::assignTechnician(int interventionId,
                                              const std::string& technicianId) {
    if (!checkAccess(AccessLevel::MODIFY)) {
        logAccessAttempt("assignTechnician", false);
        return false;
    }

    logAccessAttempt("assignTechnician", true);
    return realManager->assignTechnician(interventionId, technicianId);
}

const Intervention* InterventionManagerSecure::getIntervention(int interventionId) const {
    if (!checkAccess(AccessLevel::READ)) {
        logAccessAttempt("getIntervention", false);
        return nullptr;
    }

    logAccessAttempt("getIntervention", true);
    return realManager->getIntervention(interventionId);
}

std::vector<const Intervention*> InterventionManagerSecure::getInterventionsForDay(std::time_t date) const {
    if (!checkAccess(AccessLevel::READ)) {
        logAccessAttempt("getInterventionsForDay", false);
        return std::vector<const Intervention*>();
    }

    logAccessAttempt("getInterventionsForDay", true);
    return realManager->getInterventionsForDay(date);
}

std::map<int, int> InterventionManagerSecure::getInterventionCountsForMonth(int month, int year) const {
    if (!checkAccess(AccessLevel::READ)) {
        logAccessAttempt("getInterventionCountsForMonth", false);
        return std::map<int, int>();
    }

    logAccessAttempt("getInterventionCountsForMonth", true);
    return realManager->getInterventionCountsForMonth(month, year);
}

InterventionManager* InterventionManagerSecure::getRealManager() const {
    // Optionally, you could add a security check here to ensure only admin users
    // can access the real manager directly
    if (getCurrentUserAccessLevel() != AccessLevel::FULL) {
        logAccessAttempt("getRealManager", false);
        return nullptr;
    }

    logAccessAttempt("getRealManager", true);
    return realManager.get();
}

bool InterventionManagerSecure::decorateWithGPS(int interventionId) {
    if (!checkAccess(AccessLevel::MODIFY)) {
        logAccessAttempt("decorateWithGPS", false);
        return false;
    }

    logAccessAttempt("decorateWithGPS", true);
    return realManager->decorateWithGPS(interventionId);
}

bool InterventionManagerSecure::decorateWithAttachments(int interventionId) {
    if (!checkAccess(AccessLevel::MODIFY)) {
        logAccessAttempt("decorateWithAttachments", false);
        return false;
    }

    logAccessAttempt("decorateWithAttachments", true);
    return realManager->decorateWithAttachments(interventionId);
}

bool InterventionManagerSecure::addGPSCoordinate(int interventionId, double latitude, double longitude) {
    if (!checkAccess(AccessLevel::MODIFY)) {
        logAccessAttempt("addGPSCoordinate", false);
        return false;
    }

    logAccessAttempt("addGPSCoordinate", true);
    return realManager->addGPSCoordinate(interventionId, latitude, longitude);
}

bool InterventionManagerSecure::addAttachment(int interventionId, const std::string& filename, const std::string& description) {
    if (!checkAccess(AccessLevel::MODIFY)) {
        logAccessAttempt("addAttachment", false);
        return false;
    }

    logAccessAttempt("addAttachment", true);
    return realManager->addAttachment(interventionId, filename, description);
}