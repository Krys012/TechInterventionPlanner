//
// Created by Léo KRYS on 21/04/2025.
//

#ifndef I_INTERVENTION_MANAGER_H
#define I_INTERVENTION_MANAGER_H

#include <string>
#include <vector>
#include <map>   // Add this for std::map
#include <ctime>
#include <memory>
#include "Model/Intervention.h"
#include "Model/Technician.h"

/**
 * @class IInterventionManager
 * @brief Interface for intervention management operations
 *
 * This interface defines the operations that can be performed on interventions.
 * Both the real manager and the proxy manager will implement this interface.
 */
class IInterventionManager {
public:
    /**
     * @brief Virtual destructor
     */
    virtual ~IInterventionManager() = default;

    /**
     * @brief Create a new intervention
     * @param type Type of intervention (e.g., "Maintenance", "Emergency")
     * @param location Location of the intervention
     * @param date Date and time of the intervention
     * @param duration Expected duration in minutes
     * @return ID of the created intervention, or -1 if creation failed
     */
    virtual int createIntervention(const std::string& type,
                                  const std::string& location,
                                  std::time_t date,
                                  int duration) = 0;

    /**
     * @brief Delete an intervention
     * @param interventionId ID of the intervention to delete
     * @return True if deletion was successful
     */
    virtual bool deleteIntervention(int interventionId) = 0;

    /**
     * @brief Modify an intervention
     * @param interventionId ID of the intervention to modify
     * @param location New location (empty string to keep current)
     * @param date New date (0 to keep current)
     * @param duration New duration (0 to keep current)
     * @return True if modification was successful
     */
    virtual bool modifyIntervention(int interventionId,
                                  const std::string& location,
                                  std::time_t date,
                                  int duration) = 0;

    /**
     * @brief Change the status of an intervention
     * @param interventionId ID of the intervention
     * @param status New status
     * @param comments Optional comments about the status change
     * @return True if status change was successful
     */
    virtual bool changeInterventionStatus(int interventionId,
                                        const std::string& status,
                                        const std::string& comments = "") = 0;

    /**
     * @brief Assign a technician to an intervention
     * @param interventionId ID of the intervention
     * @param technicianId ID of the technician
     * @return True if assignment was successful
     */
    virtual bool assignTechnician(int interventionId,
                                const std::string& technicianId) = 0;

    /**
     * @brief Get an intervention by ID
     * @param interventionId ID of the intervention
     * @return Pointer to the intervention, or nullptr if not found
     */
    virtual const Intervention* getIntervention(int interventionId) const = 0;

    /**
     * @brief Get all interventions scheduled for a specific day
     * @param date Date to check
     * @return Vector of intervention pointers
     */
    virtual std::vector<const Intervention*> getInterventionsForDay(std::time_t date) const = 0;

    /**
     * @brief Get count of interventions per day for a month
     * @param month Month (1-12)
     * @param year Year
     * @return Map of day number to intervention count
     */
    virtual std::map<int, int> getInterventionCountsForMonth(int month, int year) const = 0;

    /**
     * @brief Save all interventions to persistent storage
     * @return True if save was successful
     */
    virtual bool saveInterventions() = 0;

    virtual bool decorateWithGPS(int interventionId) = 0;
    virtual bool decorateWithAttachments(int interventionId) = 0;
    virtual bool addGPSCoordinate(int interventionId, double latitude, double longitude) = 0;
    virtual bool addAttachment(int interventionId, const std::string& filename, const std::string& description = "") = 0;
};

#endif // I_INTERVENTION_MANAGER_H