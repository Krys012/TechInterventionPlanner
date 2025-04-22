//
// Created by Léo KRYS on 21/04/2025.
//

#include "Facade/InterventionManager.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iomanip>

#include "AttachmentsDecorator.h"
#include "GPSTrackingDecorator.h"

InterventionManager::InterventionManager(bool autoSave)
    : technicianManager(),
      notificationSystem(std::make_shared<NotificationSystem>(true, true, "notifications.log")),
      nextInterventionId(1),
      autoSave(autoSave) {
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

void InterventionManager::setAutoSave(bool enable) {
    autoSave = enable;
}

bool InterventionManager::addTechnician(const Technician& technician) {
    bool result = technicianManager.addTechnician(technician);

    if (result) {
        notifyObservers("technician_added",
                       "Technician " + technician.getId() + " (" + technician.getName() + ") added");
        tryAutoSave();
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

void InterventionManager::tryAutoSave() {
    if (autoSave) {
        saveInterventions();
    }
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

    // Auto save if enabled
    tryAutoSave();

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

    // Auto save if enabled
    tryAutoSave();

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

    // Auto save if enabled
    tryAutoSave();

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

    // Auto save if enabled
    tryAutoSave();

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

    // Auto save if enabled
    tryAutoSave();

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

bool InterventionManager::saveInterventions() {
    return saveData("interventions.json", "technicians.json");
}

bool InterventionManager::loadInterventions() {
    // In a real application, this would load from a database or file
    std::cout << "Loading interventions..." << std::endl;

    // For demonstration, we'll create some sample interventions
    std::time_t now = std::time(nullptr);

    // Create a maintenance intervention
    createIntervention("Maintenance", "Office Building A", now + 3600, 120);

    // Create an emergency intervention
    createIntervention("Emergency", "Data Center B", now + 1800, 60);

    notifyObservers("load", "Interventions loaded");
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

        // Auto save if enabled
        tryAutoSave();
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
            // Use the same format as saveInterventions
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

bool InterventionManager::saveData(const std::string& interventionsFile,
                                 const std::string& techniciansFile){
    bool interventionsSaved = saveInterventionsToFile(interventionsFile);
    bool techniciansSaved = saveTechniciansToFile(techniciansFile);

    notifyObservers("data_save",
                   "Data saved to " + interventionsFile + " and " + techniciansFile);

    return interventionsSaved && techniciansSaved;
}

bool InterventionManager::loadData(const std::string& interventionsFile,
                                 const std::string& techniciansFile) {
    try {
        bool interventionsLoaded = loadInterventionsFromFile(interventionsFile);
        bool techniciansLoaded = loadTechniciansFromFile(techniciansFile);

        notifyObservers("data_load",
                       "Data loaded from " + interventionsFile + " and " + techniciansFile);

        return interventionsLoaded && techniciansLoaded;
    } catch (const std::exception& e) {
        std::cerr << "Error loading data: " << e.what() << std::endl;
        return false;
    } catch (...) {
        std::cerr << "Unknown error loading data" << std::endl;
        return false;
    }


}

bool InterventionManager::saveInterventionsToFile(const std::string& filename) const {
    try {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Could not open file: " << filename << std::endl;
            return false;
        }

        file << "{\n  \"interventions\": [";

        // Vérifiez si nous avons des interventions à sauvegarder
        if (interventions.empty()) {
            // Collection vide, juste fermer le tableau
            file << "]\n}";
        } else {
            // Nous avons des interventions à sauvegarder
            file << "\n";

            bool first = true;
            for (const auto& pair : interventions) {
                const auto& id = pair.first;
                const auto& intervention = pair.second;

                if (!first) {
                    file << ",\n";
                }
                first = false;

                // Vérifiez que l'intervention n'est pas nullptr
                if (!intervention) {
                    std::cerr << "Warning: Null intervention found with ID " << id << std::endl;
                    continue;
                }

                try {
                    // Format date
                    char dateBuffer[30];
                    std::time_t date = intervention->getDate();
                    std::strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d %H:%M", std::localtime(&date));

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
                } catch (const std::exception& e) {
                    std::cerr << "Warning: Error processing intervention " << id << ": " << e.what() << std::endl;
                    continue;
                }
            }

            file << "\n  ]\n}";
        }

        file.close();

        std::cout << "Saved " << interventions.size() << " interventions to " << filename << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error saving interventions to file: " << e.what() << std::endl;
        return false;
    }
}

bool InterventionManager::saveTechniciansToFile(const std::string& filename) const {
    try {
        const auto& technicians = technicianManager.getAllTechnicians();

        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Could not open file: " << filename << std::endl;
            return false;
        }

        file << "{\n  \"technicians\": [";

        // Vérifiez si nous avons des techniciens à sauvegarder
        if (technicians.empty()) {
            // Collection vide, juste fermer le tableau
            file << "]\n}";
        } else {
            // Nous avons des techniciens à sauvegarder
            file << "\n";

            bool first = true;
            for (const auto& pair : technicians) {
                const auto& id = pair.first;
                const auto& technician = pair.second;

                if (!first) {
                    file << ",\n";
                }
                first = false;

                try {
                    file << "    {\n"
                         << "      \"id\": \"" << id << "\",\n"
                         << "      \"name\": \"" << technician.getName() << "\",\n"
                         << "      \"specialty\": \"" << technician.getSpecialty() << "\",\n"
                         << "      \"contact\": \"" << technician.getContact() << "\"\n"
                         << "    }";
                } catch (const std::exception& e) {
                    std::cerr << "Warning: Error processing technician " << id << ": " << e.what() << std::endl;
                    continue;
                }
            }

            file << "\n  ]\n}";
        }

        file.close();

        std::cout << "Saved " << technicians.size() << " technicians to " << filename << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error saving technicians to file: " << e.what() << std::endl;
        return false;
    }
}

bool InterventionManager::loadInterventionsFromFile(const std::string& filename) {
    // Dans une application réelle, vous utiliseriez une bibliothèque JSON pour cela
    // Pour cet exemple, nous allons simuler le chargement avec quelques interventions prédéfinies

    std::cout << "Loading interventions from " << filename << "..." << std::endl;

    try {
        // Vérifier si le fichier existe
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "File not found. Creating sample interventions instead." << std::endl;

            // Fichier non trouvé, créer des exemples d'interventions
            std::time_t now = std::time(nullptr);

            // Supprime toutes les interventions existantes
            interventions.clear();
            nextInterventionId = 1;

            // Crée quelques interventions d'exemple
            createIntervention("Maintenance", "Office Building A", now + 3600, 120);
            createIntervention("Emergency", "Data Center B", now + 1800, 60);
            createIntervention("Maintenance", "Retail Store C", now + 7200, 90);

            return true;
        }

        // Dans une application réelle, vous analyseriez le fichier JSON ici
        // Pour cet exemple, nous allons simplement dire que nous avons chargé
        // avec succès à partir du fichier

        std::cout << "Loaded interventions from file." << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error loading interventions: " << e.what() << std::endl;
        return false;
    }
}

bool InterventionManager::loadTechniciansFromFile(const std::string& filename) {
    // Dans une application réelle, vous utiliseriez une bibliothèque JSON pour cela
    // Pour cet exemple, nous allons simuler le chargement avec quelques techniciens prédéfinis

    std::cout << "Loading technicians from " << filename << "..." << std::endl;

    try {
        // Vérifier si le fichier existe
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "File not found. Creating sample technicians instead." << std::endl;

            // Fichier non trouvé, créer des exemples de techniciens
            technicianManager.addTechnician(Technician("TECH001", "John Smith", "Electrical", "john@example.com"));
            technicianManager.addTechnician(Technician("TECH002", "Jane Doe", "Plumbing", "jane@example.com"));
            technicianManager.addTechnician(Technician("TECH003", "Bob Johnson", "HVAC", "bob@example.com"));

            return true;
        }

        // Dans une application réelle, vous analyseriez le fichier JSON ici
        // Pour cet exemple, nous allons simplement dire que nous avons chargé
        // avec succès à partir du fichier

        std::cout << "Loaded technicians from file." << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error loading technicians: " << e.what() << std::endl;
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

        // Auto save if enabled
        tryAutoSave();

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

        // Auto save if enabled
        tryAutoSave();

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

    // Auto save if enabled
    tryAutoSave();

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

        // Auto save if enabled
        tryAutoSave();
    }

    return success;
}