//
// Created by Léo KRYS on 20/04/2025.
//

#include "Business/InterventionPlanner.h"
#include <algorithm>
#include <ctime>

InterventionPlanner::InterventionPlanner() {
    // Initialize if needed
}

bool InterventionPlanner::scheduleIntervention(int interventionId, std::time_t date, int duration) {
    // Check if the time slot is available
    if (!isTimeSlotAvailable(date, duration)) {
        return false;
    }

    // Normalize the date to midnight of the day for the daily map
    std::time_t dayStart = normalizeToDay(date);

    // Add the intervention to the daily map
    dailyInterventions[dayStart].push_back(interventionId);

    return true;
}

bool InterventionPlanner::rescheduleIntervention(int interventionId, std::time_t oldDate,
                                              std::time_t newDate, int duration) {
    // Check if the new time slot is available
    if (!isTimeSlotAvailable(newDate, duration)) {
        return false;
    }

    // Remove from old date
    if (!removeIntervention(interventionId, oldDate)) {
        return false;
    }

    // Add to new date
    return scheduleIntervention(interventionId, newDate, duration);
}

bool InterventionPlanner::removeIntervention(int interventionId, std::time_t date) {
    // Normalize the date to midnight of the day
    std::time_t dayStart = normalizeToDay(date);

    // Check if we have interventions for this day
    auto it = dailyInterventions.find(dayStart);
    if (it == dailyInterventions.end()) {
        return false;
    }

    // Find and remove the intervention ID
    auto& interventions = it->second;
    auto idIt = std::find(interventions.begin(), interventions.end(), interventionId);

    if (idIt == interventions.end()) {
        return false;
    }

    interventions.erase(idIt);

    // If the day is now empty, remove it from the map
    if (interventions.empty()) {
        dailyInterventions.erase(it);
    }

    return true;
}

std::vector<int> InterventionPlanner::getInterventionsForDay(std::time_t date) const {
    // Normalize the date to midnight of the day
    std::time_t dayStart = normalizeToDay(date);

    // Find interventions for this day
    auto it = dailyInterventions.find(dayStart);
    if (it == dailyInterventions.end()) {
        return std::vector<int>(); // Empty vector
    }

    return it->second;
}

std::map<int, int> InterventionPlanner::countInterventionsPerDay(int month, int year) const {
    std::map<int, int> counts;

    // Initialize counts for all days in the month
    int daysInMonth = getDaysInMonth(month, year);
    for (int day = 1; day <= daysInMonth; ++day) {
        counts[day] = 0;
    }

    // For each day in our daily interventions map
    for (const auto& entry : dailyInterventions) {
        // Get the day's date components
        std::time_t dayTime = entry.first;
        struct tm* timeinfo = std::localtime(&dayTime);

        // Check if it's in the requested month and year
        if (timeinfo->tm_mon + 1 == month && timeinfo->tm_year + 1900 == year) {
            // Add the count of interventions for this day
            counts[timeinfo->tm_mday] = entry.second.size();
        }
    }

    return counts;
}

bool InterventionPlanner::isTimeSlotAvailable(std::time_t date, int duration) const {
    // In a real application, this would check for overlapping interventions
    // For simplicity, we'll just check if the day is not overbooked

    // Get the day's interventions
    std::time_t dayStart = normalizeToDay(date);

    auto it = dailyInterventions.find(dayStart);
    if (it == dailyInterventions.end()) {
        return true; // No interventions scheduled for this day
    }

    // Check if the day has fewer than the maximum allowed interventions
    // (arbitrary limit for demonstration)
    const int MAX_INTERVENTIONS_PER_DAY = 10;
    return it->second.size() < MAX_INTERVENTIONS_PER_DAY;
}

bool InterventionPlanner::optimizeSchedule(std::time_t date) {
    // In a real application, this would reorder interventions for optimal routing
    // For this demo, we'll just return true
    return true;
}

// Helper function to normalize a timestamp to midnight of the day
std::time_t InterventionPlanner::normalizeToDay(std::time_t date) const {
    struct tm* timeinfo = std::localtime(&date);
    timeinfo->tm_hour = 0;
    timeinfo->tm_min = 0;
    timeinfo->tm_sec = 0;
    return std::mktime(timeinfo);
}

// Helper function to get the number of days in a month
int InterventionPlanner::getDaysInMonth(int month, int year) const {
    static const int daysPerMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (month < 1 || month > 12) {
        return 0;
    }

    // February in leap year
    if (month == 2 && (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))) {
        return 29;
    }

    return daysPerMonth[month - 1];
}