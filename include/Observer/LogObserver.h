//
// Created by Léo KRYS on 21/04/2025.
//

#ifndef LOG_OBSERVER_H
#define LOG_OBSERVER_H

#include <string>
#include <fstream>
#include "Observer/InterventionObserver.h"

/**
 * @class LogObserver
 * @brief Observer that writes messages to a log file
 */
class LogObserver : public InterventionObserver {
private:
    std::string logFilePath;

public:
    /**
     * @brief Constructor
     * @param logFilePath Path to the log file
     */
    explicit LogObserver(const std::string& logFilePath);

    /**
     * @brief Writes the message to the log file
     * @param message Message describing the event
     */
    void notify(const std::string& message) override;
};

#endif // LOG_OBSERVER_H
