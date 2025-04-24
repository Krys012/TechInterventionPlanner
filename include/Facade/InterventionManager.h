//
// Created by Léo KRYS on 21/04/2025.
//

#ifndef INTERVENTION_MANAGER_H
#define INTERVENTION_MANAGER_H

#include "Proxy/IInterventionManager.h"
#include "Business/InterventionPlanner.h"
#include "Business/TechnicianManager.h"
#include "Business/NotificationSystem.h"
#include "Factory/InterventionFactoryRegistry.h"
#include "Observer/InterventionObserver.h"
#include <map>
#include <vector>
#include <memory>
#include <string>

/**
 * @class InterventionManager
 * @brief Facade that centralizes and coordinates all intervention management operations
 *
 * This class implements the IInterventionManager interface and serves
 * as a facade for the underlying subsystems.
 */
class InterventionManager : public IInterventionManager {
private:
    InterventionPlanner planner;
    TechnicianManager technicianManager;
    std::shared_ptr<NotificationSystem> notificationSystem;
    std::map<int, std::unique_ptr<Intervention>> interventions;
    std::vector<std::shared_ptr<InterventionObserver>> observers;
    int nextInterventionId;
    bool autoSave;

    /**
     * @brief Notify all observers about an event
     * @param eventType Type of event (e.g., "creation", "modification")
     * @param message Message describing the event
     */
    void notifyObservers(const std::string& eventType, const std::string& message);

    /**
     * @brief Check if an intervention exists by ID
     * @param interventionId ID of the intervention
     * @return True if the intervention exists
     */
    bool interventionExists(int interventionId) const;

    /**
     * @brief Generate a human-readable summary of an intervention
     * @param interventionId ID of the intervention
     * @return Formatted summary string
     */
    std::string generateInterventionSummary(int interventionId) const;

    /**
     * @brief Auto save if enabled
     */
    void tryAutoSave();

    /**
     * @brief Format a date as a string
     * @param date Date to format
     * @return Formatted date string
     */
    std::string formatDate(std::time_t date) const;

    /**
     * @brief Save interventions to a JSON file
     * @param filename Path to the output file
     * @return True if saving was successful
     */
    bool saveInterventionsToFile(const std::string& filename) const;

    /**
     * @brief Save technicians to a JSON file
     * @param filename Path to the output file
     * @return True if saving was successful
     */
    bool saveTechniciansToFile(const std::string& filename) const;

    /**
     * @brief Load interventions from a JSON file
     * @param filename Path to the input file
     * @return True if loading was successful
     */
    bool loadInterventionsFromFile(const std::string& filename);

    /**
     * @brief Load technicians from a JSON file
     * @param filename Path to the input file
     * @return True if loading was successful
     */
    bool loadTechniciansFromFile(const std::string& filename);

public:
    /**
     * @brief Constructor
     * @param autoSave Whether to automatically save after changes
     */
    explicit InterventionManager(bool autoSave = false);

    /**
     * @brief Add an observer
     * @param observer Shared pointer to the observer
     */
    void addObserver(std::shared_ptr<InterventionObserver> observer);

    /**
     * @brief Remove an observer
     * @param observer Observer to remove
     */
    void removeObserver(InterventionObserver* observer);

    /**
     * @brief Enable or disable auto-save
     * @param enable Whether to enable auto-save
     */
    void setAutoSave(bool enable);

    /**
     * @brief Add a technician to the system
     * @param technician The technician to add
     * @return True if technician was added successfully
     */
    bool addTechnician(const Technician& technician);

    /**
     * @brief Get all technicians
     * @return Map of technician IDs to technicians
     */
    const std::map<std::string, Technician>& getAllTechnicians() const;

    /**
     * @brief Find available technicians for a time slot
     * @param date Date and time of the intervention
     * @param duration Duration in minutes
     * @param specialty Optional specialty requirement
     * @return Vector of available technician IDs
     */
    std::vector<std::string> findAvailableTechnicians(std::time_t date, int duration,
                                                    const std::string& specialty = "") const;

    /**
     * @brief Get a summary of all interventions
     * @return Formatted summary string
     */
    std::string generateSystemSummary() const;

    // IInterventionManager interface implementation
    int createIntervention(const std::string& type,
                          const std::string& location,
                          std::time_t date,
                          int duration) override;

    bool deleteIntervention(int interventionId) override;

    bool modifyIntervention(int interventionId,
                          const std::string& location,
                          std::time_t date,
                          int duration) override;

    bool changeInterventionStatus(int interventionId,
                                const std::string& status,
                                const std::string& comments = "") override;

    bool assignTechnician(int interventionId,
                        const std::string& technicianId) override;

    const Intervention* getIntervention(int interventionId) const override;

    std::vector<const Intervention*> getInterventionsForDay(std::time_t date) const override;

    std::map<int, int> getInterventionCountsForMonth(int month, int year) const override;

    bool saveInterventions() override;

    /**
     * @brief Load interventions from persistent storage
     * @return True if loading was successful
     */
    bool loadInterventions();

    /**
     * @brief Optimize the schedule for a specific day
     * @param date The day to optimize
     * @return True if optimization was successful
     */
    bool optimizeSchedule(std::time_t date);

    /**
     * @brief Export the schedule to a human-readable format
     * @param format Format to export ("text", "csv", "json")
     * @param outputFile Path to the output file
     * @return True if export was successful
     */
    bool exportSchedule(const std::string& format, const std::string& outputFile) const;

    /**
     * @brief Save interventions and technicians to files
     * @param interventionsFile Path to the interventions file
     * @param techniciansFile Path to the technicians file
     * @return True if saving was successful
     */
    bool saveData(const std::string& interventionsFile = "./interventions.json",
                  const std::string& techniciansFile = "./technicians.json");

    /**
     * @brief Load interventions and technicians from files
     * @param interventionsFile Path to the interventions file
     * @param techniciansFile Path to the technicians file
     * @return True if loading was successful
     */
    bool loadData(const std::string& interventionsFile = "./interventions.json",
                  const std::string& techniciansFile = "./technicians.json");

    /**
     * @brief Decorate an intervention with GPS tracking capability
     * @param interventionId ID of the intervention
     * @return True if decoration was successful
     */
    bool decorateWithGPS(int interventionId) override;

    /**
     * @brief Decorate an intervention with attachments capability
     * @param interventionId ID of the intervention
     * @return True if decoration was successful
     */
    bool decorateWithAttachments(int interventionId) override;

    /**
     * @brief Add a GPS coordinate to a GPS-decorated intervention
     * @param interventionId ID of the intervention
     * @param latitude Latitude coordinate
     * @param longitude Longitude coordinate
     * @return True if coordinate was added successfully
     */
    bool addGPSCoordinate(int interventionId, double latitude, double longitude) override;

    /**
     * @brief Add an attachment to an attachment-decorated intervention
     * @param interventionId ID of the intervention
     * @param filename Name of the file
     * @param description Optional description of the file
     * @return True if attachment was added successfully
     */
    bool addAttachment(int interventionId, const std::string& filename, const std::string& description = "") override;

  };


#endif // INTERVENTION_MANAGER_H