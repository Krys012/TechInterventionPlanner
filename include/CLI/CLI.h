//
// Created by Léo KRYS on 21/04/2025.
//

#ifndef CLI_H
#define CLI_H

#include "Proxy/InterventionManagerSecure.h"
#include "CLI/AuthenticationSystem.h"
#include <string>
#include <vector>
#include <map>
#include <functional>

/**
 * @class CLI
 * @brief Command Line Interface for the intervention management system
 */
class CLI {
private:
    AuthenticationSystem authSystem;
    std::unique_ptr<InterventionManagerSecure> manager;
    bool running;
    std::string currentUser;

    // Command handlers
    using CommandHandler = std::function<void(const std::vector<std::string>&)>;
    std::map<std::string, CommandHandler> commandHandlers;
    std::map<std::string, std::string> commandHelp;

    /**
     * @brief Initialize command handlers
     */
    void initializeCommands();

    /**
     * @brief Display the main menu
     */
    void displayMenu() const;

    /**
     * @brief Parse a command string into command and arguments
     * @param commandStr The command string to parse
     * @return Pair of command name and vector of arguments
     */
    std::pair<std::string, std::vector<std::string>> parseCommand(const std::string& commandStr) const;

    /**
     * @brief Convert a string to a time_t value
     * @param dateStr String in format "YYYY-MM-DD HH:MM"
     * @return time_t value, or 0 if parsing failed
     */
    std::time_t parseDateTime(const std::string& dateStr) const;

    /**
     * @brief Format a time_t value as a string
     * @param time The time_t value to format
     * @return Formatted string in format "YYYY-MM-DD HH:MM"
     */
    std::string formatDateTime(std::time_t time) const;

    // Command handler methods
    void handleHelp(const std::vector<std::string>& args);
    void handleExit(const std::vector<std::string>& args);
    void handleLogin(const std::vector<std::string>& args);
    void handleLogout(const std::vector<std::string>& args);
    void handleListInterventions(const std::vector<std::string>& args);
    void handleViewIntervention(const std::vector<std::string>& args);
    void handleCreateIntervention(const std::vector<std::string>& args);
    void handleModifyIntervention(const std::vector<std::string>& args);
    void handleDeleteIntervention(const std::vector<std::string>& args);
    void handleAssignTechnician(const std::vector<std::string>& args);
    void handleChangeStatus(const std::vector<std::string>& args);
    void handleAddTechnician(const std::vector<std::string>& args);
    void handleListTechnicians(const std::vector<std::string>& args);
    void handleCalendar(const std::vector<std::string>& args);
    void handleSave(const std::vector<std::string>& args);
    void handleLoad(const std::vector<std::string>& args);
    void handleSaveData(const std::vector<std::string>& args);
    void handleLoadData(const std::vector<std::string>& args);
    void handleDecorateIntervention(const std::vector<std::string>& args);
    void handleAddAttachment(const std::vector<std::string>& args);
    void handleAddGPSCoordinate(const std::vector<std::string>& args);


public:
    /**
     * @brief Constructor
     */
    CLI();

    /**
     * @brief Start the CLI application
     */
    void start();

    /**
     * @brief Process a single command
     * @param commandStr The command string to process
     * @return True if processing should continue, false to exit
     */
    bool processCommand(const std::string& commandStr);
};

#endif // CLI_H
