//
// Created by Léo KRYS on 20/04/2025.
//

#ifndef INTERVENTION_PLANNER_H
#define INTERVENTION_PLANNER_H

#include <vector>
#include <map>
#include <ctime>
#include "Model/Intervention.h"

/**
 * @class InterventionPlanner
 * @brief Manages scheduling and planning of interventions
 */
class InterventionPlanner {
private:
    std::map<std::time_t, std::vector<int>> dailyInterventions; // Maps date to list of intervention IDs

public:
    /**
     * @brief Default constructor
     */
    InterventionPlanner();

    /**
     * @brief Schedule an intervention
     * @param interventionId ID of the intervention to schedule
     * @param date Date and time for the intervention
     * @param duration Duration in minutes
     * @return True if scheduling was successful
     */
    bool scheduleIntervention(int interventionId, std::time_t date, int duration);

    /**
     * @brief Reschedule an existing intervention
     * @param interventionId ID of the intervention to reschedule
     * @param oldDate Previous scheduled date
     * @param newDate New date for the intervention
     * @param duration Duration in minutes
     * @return True if rescheduling was successful
     */
    bool rescheduleIntervention(int interventionId, std::time_t oldDate, std::time_t newDate, int duration);

    /**
     * @brief Remove an intervention from the schedule
     * @param interventionId ID of the intervention to remove
     * @param date Scheduled date of the intervention
     * @return True if removal was successful
     */
    bool removeIntervention(int interventionId, std::time_t date);

    /**
     * @brief Get all intervention IDs scheduled for a specific day
     * @param date Date to check
     * @return Vector of intervention IDs
     */
    std::vector<int> getInterventionsForDay(std::time_t date) const;

    /**
     * @brief Count interventions per day for a month
     * @param month Month (1-12)
     * @param year Year (e.g., 2025)
     * @return Map of day of month to intervention count
     */
    std::map<int, int> countInterventionsPerDay(int month, int year) const;

    /**
     * @brief Check if a time slot is available
     * @param date Start date and time
     * @param duration Duration in minutes
     * @return True if the time slot is available
     */
    bool isTimeSlotAvailable(std::time_t date, int duration) const;

    /**
     * @brief Optimize the daily schedule
     * @param date Date to optimize
     * @return True if optimization was successful
     */
    bool optimizeSchedule(std::time_t date);

private:
   /**
    * @brief Helper function to normalize a timestamp to midnight of the day
    * @param date Date to normalize
    * @return Timestamp at midnight of the same day
    */
   std::time_t normalizeToDay(std::time_t date) const;

   /**
    * @brief Helper function to get the number of days in a month
    * @param month Month (1-12)
    * @param year Year (e.g., 2025)
    * @return Number of days in the month
    */
   int getDaysInMonth(int month, int year) const;
};

#endif // INTERVENTION_PLANNER_H