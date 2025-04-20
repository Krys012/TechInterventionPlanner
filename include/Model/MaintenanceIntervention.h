//
// Created by Léo KRYS on 20/04/2025.
//
#ifndef MAINTENANCE_INTERVENTION_H
#define MAINTENANCE_INTERVENTION_H

#include "Model/Intervention.h"

/**
 * @class MaintenanceIntervention
 * @brief Class representing a maintenance type intervention
 */
class MaintenanceIntervention : public Intervention {
public:
    /**
     * @brief Default constructor
     */
    MaintenanceIntervention();

    /**
     * @brief Constructor with parameters
     * @param location Location of the intervention
     * @param date Date and time of the intervention
     * @param duration Expected duration in minutes
     */
    MaintenanceIntervention(const std::string& location, std::time_t date, int duration);

    /**
     * @brief Get the intervention type
     * @return "Maintenance"
     */
    std::string getType() const override;
};

#endif // MAINTENANCE_INTERVENTION_H