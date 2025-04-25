//
// Created by Léo KRYS on 21/04/2025.
//

#include "Facade/InterventionManager.h"
#include "Model/EmergencyIntervention.h"
#include "Model/MaintenanceIntervention.h"
#include "Decorator/GPSTrackingDecorator.h"
#include "Decorator/AttachmentsDecorator.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>

#include "AttachmentsDecorator.h"
#include "GPSTrackingDecorator.h"

InterventionManager::InterventionManager()
    : technicianManager(),
      notificationSystem(std::make_shared<NotificationSystem>(true, true, "notifications.log")),
      nextInterventionId(1) {
}

void InterventionManager::addObserver(std::shared_ptr<InterventionObserver> observer) {
    observers.push_back(observer);
}

void InterventionManager::removeObserver(InterventionObserver* observer) {
    observers.erase(
        std::remove_if(observers.begin(), observers.end(),
            [observer](const std::shared_ptr<InterventionObserver>& ptr) {
                return ptr.get() == observer;
            }),
        observers.end());
}

bool InterventionManager::addTechnician(const Technician& technician) {
    bool result = technicianManager.addTechnician(technician);

    if (result) {
        notifyObservers("technician_added",
                       "Technician " + technician.getId() + " (" + technician.getName() + ") added");
    }

    return result;
}

const std::map<std::string, Technician>& InterventionManager::getAllTechnicians() const {
    return technicianManager.getAllTechnicians();
}

std::vector<std::string> InterventionManager::findAvailableTechnicians(
    std::time_t date, int duration, const std::string& specialty) const {
    return technicianManager.findAvailableTechnicians(date, duration, specialty);
}

void InterventionManager::notifyObservers(const std::string& eventType, const std::string& message) {
    for (const auto& observer : observers) {
        if (observer->isInterestedIn(eventType)) {
            observer->notify(message);
        }
    }
}

bool InterventionManager::interventionExists(int interventionId) const {
    return interventions.find(interventionId) != interventions.end();
}

std::string InterventionManager::generateInterventionSummary(int interventionId) const {
    auto it = interventions.find(interventionId);
    if (it == interventions.end()) {
        return "Intervention #" + std::to_string(interventionId) + " not found";
    }

    const auto& intervention = it->second;

    // Format the date
    char dateBuffer[30];
    std::time_t date = intervention->getDate();
    std::strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d %H:%M", std::localtime(&date));

    // Build summary
    std::ostringstream summary;
    summary << "Intervention #" << interventionId << " (" << intervention->getType() << ")" << std::endl
            << "  Location: " << intervention->getLocation() << std::endl
            << "  Date: " << dateBuffer << std::endl
            << "  Duration: " << intervention->getDuration() << " minutes" << std::endl
            << "  Status: " << intervention->getStatus();

    // Add technician info if assigned
    std::string techId = intervention->getTechnicianId();
    if (!techId.empty()) {
        const Technician* tech = technicianManager.getTechnician(techId);
        if (tech) {
            summary << std::endl << "  Technician: " << tech->getName()
                    << " (" << techId << ") - " << tech->getSpecialty();
        } else {
            summary << std::endl << "  Technician: " << techId;
        }
    }

    // Add comments if any
    std::string comments = intervention->getComments();
    if (!comments.empty()) {
        summary << std::endl << "  Comments: " << comments;
    }

    return summary.str();
}

std::string InterventionManager::generateSystemSummary() const {
    std::ostringstream summary;

    // Count interventions by type and status
    std::map<std::string, int> typeCount;
    std::map<std::string, int> statusCount;

    for (const auto& pair : interventions) {
        const auto& intervention = pair.second;
        typeCount[intervention->getType()]++;
        statusCount[intervention->getStatus()]++;
    }

    // Generate summary
    summary << "=== System Summary ===" << std::endl
            << "Total interventions: " << interventions.size() << std::endl
            << "Total technicians: " << technicianManager.getAllTechnicians().size() << std::endl;

    // Interventions by type
    summary << std::endl << "Interventions by type:" << std::endl;
    for (const auto& pair : typeCount) {
        summary << "  " << pair.first << ": " << pair.second << std::endl;
    }

    // Interventions by status
    summary << std::endl << "Interventions by status:" << std::endl;
    for (const auto& pair : statusCount) {
        summary << "  " << pair.first << ": " << pair.second << std::endl;
    }

    return summary.str();
}

// IInterventionManager interface implementation

int InterventionManager::createIntervention(const std::string& type,
                                         const std::string& location,
                                         std::time_t date,
                                         int duration) {
    // Get the appropriate factory from the registry
    auto& registry = InterventionFactoryRegistry::getInstance();

    // Create the intervention
    auto intervention = registry.createIntervention(type, location, date, duration);
    if (!intervention) {
        return -1; // Factory returned nullptr (unknown type)
    }

    // Set the ID
    int id = nextInterventionId++;
    intervention->setId(id);

    // Schedule the intervention
    if (!planner.scheduleIntervention(id, date, duration)) {
        return -1; // Scheduling failed
    }

    // Store the intervention
    interventions[id] = std::move(intervention);

    // Notify observers
    notifyObservers("creation", "Intervention " + std::to_string(id) + " created (" + type + ")");

    return id;
}

bool InterventionManager::deleteIntervention(int interventionId) {
    // Check if the intervention exists
    auto it = interventions.find(interventionId);
    if (it == interventions.end()) {
        return false;
    }

    // Get date and duration for planner
    std::time_t date = it->second->getDate();

    // Remove from planner
    if (!planner.removeIntervention(interventionId, date)) {
        return false;
    }

    // Remove from technician if assigned
    std::string techId = it->second->getTechnicianId();
    if (!techId.empty()) {
        technicianManager.unassignTechnician(techId, date);
    }

    // Get type for notification
    std::string type = it->second->getType();

    // Remove from interventions map
    interventions.erase(it);

    // Notify observers
    notifyObservers("deletion", "Intervention " + std::to_string(interventionId) + " deleted (" + type + ")");

    return true;
}

bool InterventionManager::modifyIntervention(int interventionId,
                                          const std::string& location,
                                          std::time_t date,
                                          int duration) {
    // Check if the intervention exists
    auto it = interventions.find(interventionId);
    if (it == interventions.end()) {
        return false;
    }

    // Store current values for rescheduling
    std::time_t oldDate = it->second->getDate();
    int oldDuration = it->second->getDuration();

    // Update location if provided
    if (!location.empty()) {
        it->second->setLocation(location);
    }

    // Update date and duration if provided
    bool needsRescheduling = false;
    if (date != 0 && date != oldDate) {
        needsRescheduling = true;
    }
    if (duration != 0 && duration != oldDuration) {
        needsRescheduling = true;
        it->second->setDuration(duration);
    }

    // Reschedule if needed
    if (needsRescheduling) {
        std::time_t newDate = (date != 0) ? date : oldDate;
        int newDuration = (duration != 0) ? duration : oldDuration;

        if (!planner.rescheduleIntervention(interventionId, oldDate, newDate, newDuration)) {
            return false;
        }

        if (date != 0) {
            it->second->setDate(date);
        }

        // Update technician assignment if assigned
        std::string techId = it->second->getTechnicianId();
        if (!techId.empty()) {
            technicianManager.unassignTechnician(techId, oldDate);
            technicianManager.assignTechnician(techId, newDate, newDuration);
        }
    }

    // Notify observers
    notifyObservers("modification", "Intervention " + std::to_string(interventionId) + " modified");

    return true;
}

bool InterventionManager::changeInterventionStatus(int interventionId,
                                                const std::string& status,
                                                const std::string& comments) {
    // Check if the intervention exists
    auto it = interventions.find(interventionId);
    if (it == interventions.end()) {
        return false;
    }

    // Update status
    std::string oldStatus = it->second->getStatus();
    it->second->setStatus(status);

    // Update comments if provided
    if (!comments.empty()) {
        it->second->setComments(comments);
    }

    // Notify observers
    notifyObservers("status_change",
                   "Intervention " + std::to_string(interventionId) +
                   " status changed from " + oldStatus + " to " + status);

    // Notify technician if assigned
    std::string techId = it->second->getTechnicianId();
    if (!techId.empty()) {
        notificationSystem->notifyTechnician(techId,
            "Status of intervention #" + std::to_string(interventionId) +
            " changed to " + status);
    }

    return true;
}

bool InterventionManager::assignTechnician(int interventionId,
                                        const std::string& technicianId) {
    // Check if the intervention exists
    auto it = interventions.find(interventionId);
    if (it == interventions.end()) {
        return false;
    }

    // Check if the technician exists
    if (technicianManager.getTechnician(technicianId) == nullptr) {
        return false;
    }

    // Check if the technician is available
    std::time_t date = it->second->getDate();
    int duration = it->second->getDuration();
    if (!technicianManager.isTechnicianAvailable(technicianId, date, duration)) {
        return false;
    }

    // Unassign current technician if any
    std::string currentTechId = it->second->getTechnicianId();
    if (!currentTechId.empty()) {
        technicianManager.unassignTechnician(currentTechId, date);

        // Notify the previous technician
        notificationSystem->notifyTechnician(currentTechId,
            "You have been unassigned from intervention #" + std::to_string(interventionId));
    }

    // Assign new technician
    if (!technicianManager.assignTechnician(technicianId, date, duration)) {
        return false;
    }

    // Update intervention
    it->second->setTechnicianId(technicianId);

    // Notify observers
    notifyObservers("technician_assignment",
                   "Technician " + technicianId + " assigned to intervention " +
                   std::to_string(interventionId));

    // Notify technician
    notificationSystem->notifyTechnician(technicianId,
        "You have been assigned to intervention #" + std::to_string(interventionId) +
        " at " + it->second->getLocation());

    return true;
}

const Intervention* InterventionManager::getIntervention(int interventionId) const {
    auto it = interventions.find(interventionId);
    if (it != interventions.end()) {
        return it->second.get();
    }
    return nullptr;
}

std::vector<const Intervention*> InterventionManager::getInterventionsForDay(std::time_t date) const {
    std::vector<const Intervention*> result;

    // Get intervention IDs for the day
    std::vector<int> ids = planner.getInterventionsForDay(date);

    // Collect intervention pointers
    for (int id : ids) {
        auto it = interventions.find(id);
        if (it != interventions.end()) {
            result.push_back(it->second.get());
        }
    }

    return result;
}

std::map<int, int> InterventionManager::getInterventionCountsForMonth(int month, int year) const {
    return planner.countInterventionsPerDay(month, year);
}

bool InterventionManager::initializeWithSampleData() {
    // Create some sample interventions
    std::time_t now = std::time(nullptr);

    // Create a maintenance intervention
    createIntervention("Emergency", "Data Center B", now + 1800, 60);

    // Create another maintenance intervention for tomorrow
    std::time_t tomorrow = now + 86400; // 24 hours in seconds
    createIntervention("Maintenance", "Retail Store C", tomorrow, 90);

    // Create some sample technicians
    technicianManager.addTechnician(Technician("TECH001", "John Smith", "Electrical", "john@example.com"));
    technicianManager.addTechnician(Technician("TECH002", "Jane Doe", "Plumbing", "jane@example.com"));
    technicianManager.addTechnician(Technician("TECH003", "Bob Johnson", "HVAC", "bob@example.com"));

    notifyObservers("initialization", "System initialized with sample data");

    return true;
}

bool InterventionManager::optimizeSchedule(std::time_t date) {
    // Optimize the schedule for a specific day
    bool success = planner.optimizeSchedule(date);

    if (success) {
        // Get all interventions for the day to update them
        std::vector<int> ids = planner.getInterventionsForDay(date);

        // In a real implementation, the optimizeSchedule method would return
        // the new schedule and we would update the interventions accordingly

        notifyObservers("optimization",
                      "Schedule optimized for " + formatDate(date));
    }

    return success;
}

bool InterventionManager::exportSchedule(const std::string& format,
                                      const std::string& outputFile) const {
    try {
        std::ofstream file(outputFile);
        if (!file.is_open()) {
            return false;
        }

        if (format == "csv") {
            // CSV header
            file << "ID,Type,Location,Date,Duration,Technician,Status,Comments\n";

            // Export each intervention
            for (const auto& pair : interventions) {
                const auto& id = pair.first;
                const auto& intervention = pair.second;

                // Format date
                char dateBuffer[30];
                std::time_t date = intervention->getDate();
                std::strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d %H:%M",
                             std::localtime(&date));

                file << id << ","
                     << "\"" << intervention->getType() << "\","
                     << "\"" << intervention->getLocation() << "\","
                     << "\"" << dateBuffer << "\","
                     << intervention->getDuration() << ","
                     << "\"" << intervention->getTechnicianId() << "\","
                     << "\"" << intervention->getStatus() << "\","
                     << "\"" << intervention->getComments() << "\"\n";
            }
        } else if (format == "json") {
            // Create a simple JSON output manually
            file << "{\n  \"interventions\": [\n";

            bool first = true;
            for (const auto& pair : interventions) {
                const auto& id = pair.first;
                const auto& intervention = pair.second;

                if (!first) {
                    file << ",\n";
                }
                first = false;

                // Format date
                char dateBuffer[30];
                std::time_t date = intervention->getDate();
                std::strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d %H:%M",
                             std::localtime(&date));

                file << "    {\n"
                     << "      \"id\": " << id << ",\n"
                     << "      \"type\": \"" << intervention->getType() << "\",\n"
                     << "      \"location\": \"" << intervention->getLocation() << "\",\n"
                     << "      \"date\": \"" << dateBuffer << "\",\n"
                     << "      \"duration\": " << intervention->getDuration() << ",\n"
                     << "      \"technicianId\": \"" << intervention->getTechnicianId() << "\",\n"
                     << "      \"status\": \"" << intervention->getStatus() << "\",\n"
                     << "      \"comments\": \"" << intervention->getComments() << "\"\n"
                     << "    }";
            }

            file << "\n  ]\n}";
        } else {
            // Default to text format
            file << "=== Intervention Schedule ===\n\n";

            for (const auto& pair : interventions) {
                const auto& id = pair.first;
                file << generateInterventionSummary(id) << "\n\n";
            }
        }

        file.close();
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Error exporting schedule: " << e.what() << std::endl;
        return false;
    }
}

// Helper function to format date
std::string InterventionManager::formatDate(std::time_t date) const {
    char buffer[30];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", std::localtime(&date));
    return std::string(buffer);
}

bool InterventionManager::decorateWithGPS(int interventionId) {
    // Find the intervention
    auto it = interventions.find(interventionId);
    if (it == interventions.end()) {
        return false;
    }

    // Apply GPS decorator
    try {
        auto decorated = std::make_unique<GPSTrackingDecorator>(std::move(it->second), 15); // 15-minute update frequency
        it->second = std::move(decorated);

        // Notify observers
        notifyObservers("decoration",
                        "Intervention " + std::to_string(interventionId) +
                        " decorated with GPS tracking capability");

        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error applying GPS decorator: " << e.what() << std::endl;
        return false;
    }
}

bool InterventionManager::decorateWithAttachments(int interventionId) {
    // Find the intervention
    auto it = interventions.find(interventionId);
    if (it == interventions.end()) {
        return false;
    }

    // Apply Attachments decorator
    try {
        auto decorated = std::make_unique<AttachmentsDecorator>(std::move(it->second));
        it->second = std::move(decorated);

        // Notify observers
        notifyObservers("decoration",
                        "Intervention " + std::to_string(interventionId) +
                        " decorated with attachments capability");

        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error applying Attachments decorator: " << e.what() << std::endl;
        return false;
    }
}

bool InterventionManager::addGPSCoordinate(int interventionId, double latitude, double longitude) {
    // Find the intervention
    auto it = interventions.find(interventionId);
    if (it == interventions.end()) {
        return false;
    }

    // Check if it has GPS decorator
    GPSTrackingDecorator* gpsDecorator = dynamic_cast<GPSTrackingDecorator*>(it->second.get());
    if (!gpsDecorator) {
        return false;
    }

    // Add coordinate
    gpsDecorator->addCoordinate(latitude, longitude);

    // Notify observers
    notifyObservers("gps_update",
                   "GPS coordinate added to intervention " + std::to_string(interventionId) +
                   " (" + std::to_string(latitude) + ", " + std::to_string(longitude) + ")");

    return true;
}

bool InterventionManager::addAttachment(int interventionId, const std::string& filename, const std::string& description) {
    // Find the intervention
    auto it = interventions.find(interventionId);
    if (it == interventions.end()) {
        return false;
    }

    // Check if it has Attachments decorator
    AttachmentsDecorator* attachmentsDecorator = dynamic_cast<AttachmentsDecorator*>(it->second.get());
    if (!attachmentsDecorator) {
        return false;
    }

    // Add attachment
    bool success = attachmentsDecorator->addAttachment(filename, description);

    if (success) {
        // Notify observers
        notifyObservers("attachment_added",
                       "Attachment added to intervention " + std::to_string(interventionId) +
                       " (" + filename + ")");
    }

    return success;
}