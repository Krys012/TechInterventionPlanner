//
// Created by Léo KRYS on 20/04/2025.
//

#ifndef EMERGENCY_FACTORY_H
#define EMERGENCY_FACTORY_H

#include "Factory/InterventionFactory.h"
#include "Model/EmergencyIntervention.h"

/**
 * @class EmergencyFactory
 * @brief Factory for creating emergency interventions
 */
class EmergencyFactory : public InterventionFactory {
private:
    int defaultPriority;

public:
    /**
     * @brief Constructor
     * @param defaultPriority Default priority for created interventions (1-5)
     */
    explicit EmergencyFactory(int defaultPriority = 3);

    /**
     * @brief Create a default emergency intervention
     * @return A unique pointer to a new emergency intervention
     */
    std::unique_ptr<Intervention> createIntervention() const override;

    /**
     * @brief Create an emergency intervention with parameters
     * @param location The location of the intervention
     * @param date The date and time of the intervention
     * @param duration The duration in minutes
     * @return A unique pointer to a new emergency intervention with priority
     */
    std::unique_ptr<Intervention> createIntervention(
        const std::string& location,
        std::time_t date,
        int duration) const override;

    /**
     * @brief Create an emergency intervention with parameters and priority
     * @param location The location of the intervention
     * @param date The date and time of the intervention
     * @param duration The duration in minutes
     * @param priority The priority level (1-5)
     * @return A unique pointer to a new emergency intervention
     */
    std::unique_ptr<Intervention> createEmergencyIntervention(
        const std::string& location,
        std::time_t date,
        int duration,
        int priority) const;
};

#endif // EMERGENCY_FACTORY_H
