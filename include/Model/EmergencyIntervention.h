//
// Created by Léo KRYS on 20/04/2025.
//

#ifndef EMERGENCY_INTERVENTION_H
#define EMERGENCY_INTERVENTION_H

#include "Intervention.h"

/**
 * @class EmergencyIntervention
 * @brief Class representing an emergency type intervention
 */
class EmergencyIntervention : public Intervention {
private:
    int priority; // 1-5, 5 being the highest

public:
    /**
     * @brief Default constructor
     */
    EmergencyIntervention();

    /**
     * @brief Constructor with parameters
     * @param location Location of the intervention
     * @param date Date and time of the intervention
     * @param duration Expected duration in minutes
     * @param priority Priority level (1-5)
     */
    EmergencyIntervention(const std::string& location, std::time_t date, int duration, int priority = 3);

    /**
     * @brief Get the intervention type
     * @return "Emergency"
     */
    std::string getType() const override;

    /**
     * @brief Get the priority level
     * @return Priority level (1-5)
     */
    int getPriority() const;

    /**
     * @brief Set the priority level
     * @param priority Priority level (1-5)
     */
    void setPriority(int priority);

    /**
     * @brief Override getInfo method to include priority
     * @return Formatted information as a string
     */
    std::string getInfo() const override;
};

#endif // EMERGENCY_INTERVENTION_H
