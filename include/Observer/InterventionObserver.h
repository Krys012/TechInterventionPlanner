//
// Created by Léo KRYS on 21/04/2025.
//

#ifndef INTERVENTION_OBSERVER_H
#define INTERVENTION_OBSERVER_H

#include <string>

/**
 * @class InterventionObserver
 * @brief Interface for observers that react to intervention events
 */
class InterventionObserver {
public:
    /**
     * @brief Virtual destructor
     */
    virtual ~InterventionObserver() = default;

    /**
     * @brief Notification method called when intervention events occur
     * @param message Message describing the event
     */
    virtual void notify(const std::string& message) = 0;

    /**
     * @brief Check if this observer is interested in a specific event
     * @param eventType Type of event (e.g., "creation", "modification", "deletion")
     * @return True if the observer is interested in this event type
     */
    virtual bool isInterestedIn(const std::string& eventType) const {
       (void)eventType; // Explicitly mark as unused
       return true; // Default implementation is interested in all events
    }
};

#endif // INTERVENTION_OBSERVER_H
