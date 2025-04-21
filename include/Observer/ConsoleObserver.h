//
// Created by Léo KRYS on 21/04/2025.
//

#ifndef CONSOLE_OBSERVER_H
#define CONSOLE_OBSERVER_H

#include <vector>
#include "Observer/InterventionObserver.h"

/**
 * @class ConsoleObserver
 * @brief Observer that displays messages in the console
 */
class ConsoleObserver : public InterventionObserver {
private:
    std::vector<std::string> interestedEvents;

public:
    /**
     * @brief Default constructor - interested in all events
     */
    ConsoleObserver();

    /**
     * @brief Constructor with specific events of interest
     * @param interestedEvents Vector of event types this observer is interested in
     */
    explicit ConsoleObserver(const std::vector<std::string>& interestedEvents);

    /**
     * @brief Displays the message in the console
     * @param message Message describing the event
     */
    void notify(const std::string& message) override;

    /**
     * @brief Check if this observer is interested in a specific event
     * @param eventType Type of event
     * @return True if the observer is interested in this event type
     */
    bool isInterestedIn(const std::string& eventType) const override;
};

#endif // CONSOLE_OBSERVER_H
