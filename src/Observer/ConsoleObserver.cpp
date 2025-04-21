//
// Created by Léo KRYS on 21/04/2025.
//

#include "Observer/ConsoleObserver.h"
#include <iostream>
#include <algorithm>

ConsoleObserver::ConsoleObserver()
    : interestedEvents() { // Empty vector means interested in all events
}

ConsoleObserver::ConsoleObserver(const std::vector<std::string>& interestedEvents)
    : interestedEvents(interestedEvents) {
}

void ConsoleObserver::notify(const std::string& message) {
    // Display the message in the console with a prefix
    std::cout << "[NOTIFICATION] " << message << std::endl;
}

bool ConsoleObserver::isInterestedIn(const std::string& eventType) const {
    // If no specific events are listed, interested in all events
    if (interestedEvents.empty()) {
        return true;
    }

    // Check if the eventType is in the list of interested events
    return std::find(interestedEvents.begin(), interestedEvents.end(), eventType) != interestedEvents.end();
}