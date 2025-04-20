//
// Created by Léo KRYS on 20/04/2025.
//

#ifndef MAINTENANCE_FACTORY_H
#define MAINTENANCE_FACTORY_H

#include "Factory/InterventionFactory.h"
#include "Model/MaintenanceIntervention.h"

/**
 * @class MaintenanceFactory
 * @brief Factory for creating maintenance interventions
 */
class MaintenanceFactory : public InterventionFactory {
public:
    /**
     * @brief Create a default maintenance intervention
     * @return A unique pointer to a new maintenance intervention
     */
    std::unique_ptr<Intervention> createIntervention() const override;

    /**
     * @brief Create a maintenance intervention with parameters
     * @param location The location of the intervention
     * @param date The date and time of the intervention
     * @param duration The duration in minutes
     * @return A unique pointer to a new maintenance intervention
     */
    std::unique_ptr<Intervention> createIntervention(
        const std::string& location,
        std::time_t date,
        int duration) const override;
};

#endif // MAINTENANCE_FACTORY_H
