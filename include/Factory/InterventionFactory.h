//
// Created by Léo KRYS on 20/04/2025.
//

#ifndef INTERVENTION_FACTORY_H
#define INTERVENTION_FACTORY_H

#include <memory>
#include <string>
#include <ctime>
#include "Model/Intervention.h"

/**
 * @class InterventionFactory
 * @brief Abstract factory interface for creating interventions
 */
class InterventionFactory {
public:
    /**
     * @brief Virtual destructor
     */
    virtual ~InterventionFactory() = default;

    /**
     * @brief Factory method to create an intervention
     * @return Unique pointer to a new intervention
     */
    virtual std::unique_ptr<Intervention> createIntervention() const = 0;

    /**
     * @brief Factory method to create an intervention with parameters
     * @param location The location of the intervention
     * @param date The date and time of the intervention
     * @param duration The duration in minutes
     * @return Unique pointer to a new intervention with the given parameters
     */
    virtual std::unique_ptr<Intervention> createIntervention(
        const std::string& location,
        std::time_t date,
        int duration) const = 0;
};

#endif // INTERVENTION_FACTORY_H