//
// Created by Léo KRYS on 20/04/2025.
//

#ifndef TECHNICIAN_MANAGER_H
#define TECHNICIAN_MANAGER_H

#include <map>
#include <vector>
#include <string>
#include <ctime>
#include "Model/Technician.h"

/**
 * @class TechnicianManager
 * @brief Manages technicians and their availability
 */
class TechnicianManager {
private:
    std::map<std::string, Technician> technicians; // Maps ID to technician
    std::map<std::string, std::map<std::time_t, int>> assignments; // Technician ID -> (date -> duration)

public:
    /**
     * @brief Default constructor
     */
    TechnicianManager();

    /**
     * @brief Add a new technician
     * @param technician Technician to add
     * @return True if technician was added successfully
     */
    bool addTechnician(const Technician& technician);

    /**
     * @brief Remove a technician
     * @param technicianId ID of the technician to remove
     * @return True if technician was removed successfully
     */
    bool removeTechnician(const std::string& technicianId);

    /**
     * @brief Get a technician by ID
     * @param technicianId ID of the technician
     * @return Pointer to technician or nullptr if not found
     */
    const Technician* getTechnician(const std::string& technicianId) const;

    /**
     * @brief Get all technicians
     * @return Map of technician IDs to technicians
     */
    const std::map<std::string, Technician>& getAllTechnicians() const;

    /**
     * @brief Check if a technician is available at a specific time
     * @param technicianId ID of the technician
     * @param date Date and time to check
     * @param duration Duration in minutes
     * @return True if technician is available
     */
    bool isTechnicianAvailable(const std::string& technicianId, std::time_t date, int duration) const;

    /**
     * @brief Assign a technician to an intervention
     * @param technicianId ID of the technician
     * @param date Date and time of the intervention
     * @param duration Duration in minutes
     * @return True if assignment was successful
     */
    bool assignTechnician(const std::string& technicianId, std::time_t date, int duration);

    /**
     * @brief Unassign a technician from an intervention
     * @param technicianId ID of the technician
     * @param date Date and time of the intervention
     * @return True if unassignment was successful
     */
    bool unassignTechnician(const std::string& technicianId, std::time_t date);

    /**
     * @brief Find available technicians for a time slot
     * @param date Date and time of the intervention
     * @param duration Duration in minutes
     * @param specialty Optional specialty requirement
     * @return Vector of available technician IDs
     */
    std::vector<std::string> findAvailableTechnicians(std::time_t date, int duration,
                                                    const std::string& specialty = "") const;
};

#endif // TECHNICIAN_MANAGER_H