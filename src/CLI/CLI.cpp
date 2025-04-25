//
// Created by Léo KRYS on 21/04/2025.
//

#include "CLI/CLI.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>

CLI::CLI() : running(false), currentUser("") {
    initializeCommands();
}

void CLI::initializeCommands() {
    // Basic commands
    commandHandlers["help"] = [this](const auto& args) { handleHelp(args); };
    commandHelp["help"] = "Display available commands";

    commandHandlers["exit"] = [this](const auto& args) { handleExit(args); };
    commandHelp["exit"] = "Exit the application";

    commandHandlers["login"] = [this](const auto& args) { handleLogin(args); };
    commandHelp["login"] = "Login with username and password: login <username> <password>";

    commandHandlers["logout"] = [this](const auto& args) { handleLogout(args); };
    commandHelp["logout"] = "Logout from the current session";

    // Intervention management
    commandHandlers["list"] = [this](const auto& args) { handleListInterventions(args); };
    commandHelp["list"] = "List interventions: list [date]";

    commandHandlers["view"] = [this](const auto& args) { handleViewIntervention(args); };
    commandHelp["view"] = "View intervention details: view <id>";

    commandHandlers["create"] = [this](const auto& args) { handleCreateIntervention(args); };
    commandHelp["create"] = "Create a new intervention: create <type> <location> <date> <duration>";

    commandHandlers["modify"] = [this](const auto& args) { handleModifyIntervention(args); };
    commandHelp["modify"] = "Modify an intervention: modify <id> <field> <value>";

    commandHandlers["delete"] = [this](const auto& args) { handleDeleteIntervention(args); };
    commandHelp["delete"] = "Delete an intervention: delete <id>";

    commandHandlers["assign"] = [this](const auto& args) { handleAssignTechnician(args); };
    commandHelp["assign"] = "Assign a technician to an intervention: assign <intervention_id> <technician_id>";

    commandHandlers["status"] = [this](const auto& args) { handleChangeStatus(args); };
    commandHelp["status"] = "Change intervention status: status <id> <new_status> [comments]";

    // Technician management
    commandHandlers["addtech"] = [this](const auto& args) { handleAddTechnician(args); };
    commandHelp["addtech"] = "Add a new technician: addtech <id> <name> <specialty> <contact>";

    commandHandlers["techs"] = [this](const auto& args) { handleListTechnicians(args); };
    commandHelp["techs"] = "List all technicians";

    // Calendar
    commandHandlers["calendar"] = [this](const auto& args) { handleCalendar(args); };
    commandHelp["calendar"] = "Show calendar for a month: calendar <month> <year>";

    // Data management
    commandHandlers["initialize"] = [this](const auto& args) { handleInitialize(args); };
    commandHelp["initialize"] = "Initialize the system with sample data";

    commandHandlers["export"] = [this](const auto& args) { handleExport(args); };
    commandHelp["export"] = "Export schedule to a file: export <format> <output_file>";

    // Decorator commands
    commandHandlers["decorate"] = [this](const auto& args) { handleDecorateIntervention(args); };
    commandHelp["decorate"] = "Add a decorator to an intervention: decorate <id> <decorator_type> [<parameters>]";

    commandHandlers["addattachment"] = [this](const auto& args) { handleAddAttachment(args); };
    commandHelp["addattachment"] = "Add an attachment to a decorated intervention: addattachment <id> <filename> [<description>]";

    commandHandlers["addgpscoord"] = [this](const auto& args) { handleAddGPSCoordinate(args); };
    commandHelp["addgpscoord"] = "Add GPS coordinates to a decorated intervention: addgpscoord <id> <latitude> <longitude>";
}

void CLI::start() {
    running = true;
    std::cout << "=============================================" << std::endl;
    std::cout << "   Technical Intervention Management System  " << std::endl;
    std::cout << "=============================================" << std::endl;
    std::cout << "Type 'help' for a list of commands." << std::endl;

    while (running) {
        // Display prompt
        std::cout << std::endl;
        if (currentUser.empty()) {
            std::cout << "> ";
        } else {
            std::cout << currentUser << "> ";
        }

        // Get command
        std::string commandStr;
        std::getline(std::cin, commandStr);

        // Process command
        try {
            processCommand(commandStr);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

bool CLI::processCommand(const std::string& commandStr) {
    // Parse command
    auto [command, args] = parseCommand(commandStr);

    // Skip empty commands
    if (command.empty()) {
        return true;
    }

    // Convert command to lowercase
    std::transform(command.begin(), command.end(), command.begin(),
                  [](unsigned char c) { return std::tolower(c); });

    // Find handler
    auto it = commandHandlers.find(command);
    if (it == commandHandlers.end()) {
        std::cout << "Unknown command: " << command << std::endl;
        std::cout << "Type 'help' for a list of commands." << std::endl;
        return true;
    }

    // Execute handler
    try {
        it->second(args);
    } catch (const std::exception& e) {
        std::cout << "Error executing command: " << e.what() << std::endl;
    }

    return running;
}

std::pair<std::string, std::vector<std::string>> CLI::parseCommand(const std::string& commandStr) const {
    std::istringstream iss(commandStr);
    std::string command;
    iss >> command;

    std::vector<std::string> args;
    std::string arg;

    // Check for quoted arguments
    std::string remaining;
    std::getline(iss, remaining);

    // Trim leading whitespace
    remaining.erase(0, remaining.find_first_not_of(" \t"));

    // Parse remaining arguments
    std::size_t pos = 0;
    while (pos < remaining.length()) {
        // Check if argument starts with a quote
        if (remaining[pos] == '"') {
            // Find closing quote
            std::size_t endQuote = remaining.find('"', pos + 1);
            if (endQuote == std::string::npos) {
                // No closing quote, treat rest as a single argument
                args.push_back(remaining.substr(pos + 1));
                break;
            }

            // Extract quoted argument
            args.push_back(remaining.substr(pos + 1, endQuote - pos - 1));
            pos = endQuote + 1;
        } else {
            // Find next space
            std::size_t endSpace = remaining.find(' ', pos);
            if (endSpace == std::string::npos) {
                // No more spaces, treat rest as a single argument
                args.push_back(remaining.substr(pos));
                break;
            }

            // Extract argument
            args.push_back(remaining.substr(pos, endSpace - pos));
            pos = endSpace;
        }

        // Skip whitespace
        pos = remaining.find_first_not_of(" \t", pos);
        if (pos == std::string::npos) {
            break;
        }
    }

    return std::make_pair(command, args);
}

std::time_t CLI::parseDateTime(const std::string& dateStr) const {
    struct std::tm tm = {};

    // Try to parse "YYYY-MM-DD HH:MM" format
    if (std::sscanf(dateStr.c_str(), "%d-%d-%d %d:%d",
                   &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
                   &tm.tm_hour, &tm.tm_min) == 5) {
        tm.tm_year -= 1900; // Years since 1900
        tm.tm_mon -= 1;     // Months are 0-based
        return std::mktime(&tm);
    }

    // Try to parse "YYYY-MM-DD" format
    if (std::sscanf(dateStr.c_str(), "%d-%d-%d",
                   &tm.tm_year, &tm.tm_mon, &tm.tm_mday) == 3) {
        tm.tm_year -= 1900; // Years since 1900
        tm.tm_mon -= 1;     // Months are 0-based
        return std::mktime(&tm);
    }

    // Failed to parse
    return 0;
}

std::string CLI::formatDateTime(std::time_t time) const {
    char buffer[30];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", std::localtime(&time));
    return std::string(buffer);
}

// Command handlers

void CLI::handleHelp([[maybe_unused]] const std::vector<std::string>& args) {
    std::cout << "Available commands:" << std::endl;

    // Calculate the longest command name for alignment
    std::size_t maxLength = 0;
    for (const auto& [cmd, _] : commandHelp) {
        maxLength = std::max(maxLength, cmd.length());
    }

    // Display commands with descriptions
    for (const auto& [cmd, desc] : commandHelp) {
        std::cout << "  " << std::left << std::setw(maxLength + 2) << cmd << desc << std::endl;
    }
}

void CLI::handleExit([[maybe_unused]] const std::vector<std::string>& args) {
    std::cout << "Exiting application." << std::endl;
    running = false;
}

void CLI::handleLogin(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Usage: login <username> <password>" << std::endl;
        return;
    }

    if (!currentUser.empty()) {
        std::cout << "Already logged in as " << currentUser << ". Please logout first." << std::endl;
        return;
    }

    if (authSystem.authenticate(args[0], args[1])) {
        currentUser = authSystem.getCurrentUser();
        std::string role = authSystem.getUserRole(currentUser);

        std::cout << "Login successful. Welcome, " << currentUser << " (" << role << ")." << std::endl;

        // Create secure manager with user's permissions
        manager = std::make_unique<InterventionManagerSecure>(currentUser);

        // Map user roles to access levels
        if (role == "admin") {
            manager->setUserAccessLevel(currentUser, InterventionManagerSecure::AccessLevel::FULL);
        } else if (role == "manager") {
            manager->setUserAccessLevel(currentUser, InterventionManagerSecure::AccessLevel::MODIFY);
        } else if (role == "technician") {
            manager->setUserAccessLevel(currentUser, InterventionManagerSecure::AccessLevel::READ);
        } else {
            manager->setUserAccessLevel(currentUser, InterventionManagerSecure::AccessLevel::READ);
        }
    } else {
        std::cout << "Login failed. Invalid username or password." << std::endl;
    }
}

void CLI::handleLogout([[maybe_unused]] const std::vector<std::string>& args) {
    if (currentUser.empty()) {
        std::cout << "Not logged in." << std::endl;
        return;
    }

    authSystem.logout();
    manager.reset();
    std::cout << "Logged out." << std::endl;
    currentUser = "";
}

void CLI::handleListInterventions(const std::vector<std::string>& args) {
    if (currentUser.empty()) {
        std::cout << "Please login first." << std::endl;
        return;
    }

    std::time_t date;
    if (args.empty()) {
        // Use today's date if none provided
        date = std::time(nullptr);
    } else {
        date = parseDateTime(args[0]);
        if (date == 0) {
            std::cout << "Invalid date format. Use YYYY-MM-DD." << std::endl;
            return;
        }
    }

    auto interventions = manager->getInterventionsForDay(date);

    if (interventions.empty()) {
        std::cout << "No interventions scheduled for " << formatDateTime(date) << std::endl;
        return;
    }

    std::cout << "Interventions for " << formatDateTime(date) << ":" << std::endl;
    std::cout << std::left
              << std::setw(4) << "ID"
              << std::setw(15) << "Type"
              << std::setw(20) << "Location"
              << std::setw(15) << "Time"
              << std::setw(10) << "Duration"
              << std::setw(15) << "Technician"
              << std::setw(15) << "Status"
              << std::endl;

    std::cout << std::string(85, '-') << std::endl;

    for (const auto* intervention : interventions) {
        char timeBuffer[10];
        std::time_t date = intervention->getDate();
        std::strftime(timeBuffer, sizeof(timeBuffer), "%H:%M", std::localtime(&date));

        std::cout << std::left
                  << std::setw(4) << intervention->getId()
                  << std::setw(15) << intervention->getType()
                  << std::setw(20) << intervention->getLocation()
                  << std::setw(15) << timeBuffer
                  << std::setw(10) << intervention->getDuration()
                  << std::setw(15) << intervention->getTechnicianId()
                  << std::setw(15) << intervention->getStatus()
                  << std::endl;
    }
}

void CLI::handleViewIntervention(const std::vector<std::string>& args) {
    if (currentUser.empty()) {
        std::cout << "Please login first." << std::endl;
        return;
    }

    if (args.empty()) {
        std::cout << "Usage: view <id>" << std::endl;
        return;
    }

    try {
        int id = std::stoi(args[0]);
        const Intervention* intervention = manager->getIntervention(id);

        if (!intervention) {
            std::cout << "Intervention not found." << std::endl;
            return;
        }

        // Afficher directement les informations retournées par getInfo()
        std::cout << intervention->getInfo() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Invalid intervention ID." << std::endl;
    }
}

void CLI::handleCreateIntervention(const std::vector<std::string>& args) {
    if (currentUser.empty()) {
        std::cout << "Please login first." << std::endl;
        return;
    }

    if (!authSystem.currentUserHasRole("admin") && !authSystem.currentUserHasRole("manager")) {
        std::cout << "Permission denied. Only admins and managers can create interventions." << std::endl;
        return;
    }

    if (args.size() < 4) {
        std::cout << "Usage: create <type> <location> <date> <duration>" << std::endl;
        std::cout << "Example: create Maintenance \"Office Building A\" \"2025-06-15 14:30\" 120" << std::endl;
        return;
    }

    std::string type = args[0];
    std::string location = args[1];
    std::time_t date = parseDateTime(args[2]);

    if (date == 0) {
        std::cout << "Invalid date format. Use YYYY-MM-DD HH:MM." << std::endl;
        return;
    }

    int duration;
    try {
        duration = std::stoi(args[3]);
    } catch (const std::exception& e) {
        std::cout << "Invalid duration." << std::endl;
        return;
    }

    if (duration <= 0) {
        std::cout << "Duration must be positive." << std::endl;
        return;
    }

    int id = manager->createIntervention(type, location, date, duration);

    if (id != -1) {
        std::cout << "Intervention created with ID: " << id << std::endl;
    } else {
        std::cout << "Failed to create intervention." << std::endl;
    }
}

void CLI::handleModifyIntervention(const std::vector<std::string>& args) {
    if (currentUser.empty()) {
        std::cout << "Please login first." << std::endl;
        return;
    }

    if (!authSystem.currentUserHasRole("admin") && !authSystem.currentUserHasRole("manager")) {
        std::cout << "Permission denied. Only admins and managers can modify interventions." << std::endl;
        return;
    }

    if (args.size() < 3) {
        std::cout << "Usage: modify <id> <field> <value>" << std::endl;
        std::cout << "Fields: location, date, duration" << std::endl;
        std::cout << "Example: modify 1 location \"New Location\"" << std::endl;
        return;
    }

    int id;
    try {
        id = std::stoi(args[0]);
    } catch (const std::exception& e) {
        std::cout << "Invalid intervention ID." << std::endl;
        return;
    }

    const Intervention* intervention = manager->getIntervention(id);
    if (!intervention) {
        std::cout << "Intervention not found." << std::endl;
        return;
    }

    std::string field = args[1];
    std::transform(field.begin(), field.end(), field.begin(),
                  [](unsigned char c) { return std::tolower(c); });

    std::string value = args[2];

    if (field == "location") {
        if (manager->modifyIntervention(id, value, 0, 0)) {
            std::cout << "Location updated." << std::endl;
        } else {
            std::cout << "Failed to update location." << std::endl;
        }
    } else if (field == "date") {
        std::time_t date = parseDateTime(value);
        if (date == 0) {
            std::cout << "Invalid date format. Use YYYY-MM-DD HH:MM." << std::endl;
            return;
        }

        if (manager->modifyIntervention(id, "", date, 0)) {
            std::cout << "Date updated." << std::endl;
        } else {
            std::cout << "Failed to update date." << std::endl;
        }
    } else if (field == "duration") {
        int duration;
        try {
            duration = std::stoi(value);
        } catch (const std::exception& e) {
            std::cout << "Invalid duration." << std::endl;
            return;
        }

        if (duration <= 0) {
            std::cout << "Duration must be positive." << std::endl;
            return;
        }

        if (manager->modifyIntervention(id, "", 0, duration)) {
            std::cout << "Duration updated." << std::endl;
        } else {
            std::cout << "Failed to update duration." << std::endl;
        }
    } else {
        std::cout << "Unknown field: " << field << std::endl;
        std::cout << "Valid fields are: location, date, duration" << std::endl;
    }
}

void CLI::handleDeleteIntervention(const std::vector<std::string>& args) {
    if (currentUser.empty()) {
        std::cout << "Please login first." << std::endl;
        return;
    }

    if (!authSystem.currentUserHasRole("admin")) {
        std::cout << "Permission denied. Only admins can delete interventions." << std::endl;
        return;
    }

    if (args.empty()) {
        std::cout << "Usage: delete <id>" << std::endl;
        return;
    }

    int id;
    try {
        id = std::stoi(args[0]);
    } catch (const std::exception& e) {
        std::cout << "Invalid intervention ID." << std::endl;
        return;
    }

    // Confirm deletion
    std::cout << "Are you sure you want to delete intervention #" << id << "? (y/n): ";
    std::string confirmation;
    std::getline(std::cin, confirmation);

    if (confirmation != "y" && confirmation != "Y") {
        std::cout << "Deletion cancelled." << std::endl;
        return;
    }

    if (manager->deleteIntervention(id)) {
        std::cout << "Intervention deleted." << std::endl;
    } else {
        std::cout << "Failed to delete intervention." << std::endl;
    }
}

void CLI::handleAssignTechnician(const std::vector<std::string>& args) {
    if (currentUser.empty()) {
        std::cout << "Please login first." << std::endl;
        return;
    }

    if (!authSystem.currentUserHasRole("admin") && !authSystem.currentUserHasRole("manager")) {
        std::cout << "Permission denied. Only admins and managers can assign technicians." << std::endl;
        return;
    }

    if (args.size() < 2) {
        std::cout << "Usage: assign <intervention_id> <technician_id>" << std::endl;
        return;
    }

    int interventionId;
    try {
        interventionId = std::stoi(args[0]);
    } catch (const std::exception& e) {
        std::cout << "Invalid intervention ID." << std::endl;
        return;
    }

    std::string technicianId = args[1];

    if (manager->assignTechnician(interventionId, technicianId)) {
        std::cout << "Technician assigned." << std::endl;
    } else {
        std::cout << "Failed to assign technician." << std::endl;
    }
}

void CLI::handleChangeStatus(const std::vector<std::string>& args) {
    if (currentUser.empty()) {
        std::cout << "Please login first." << std::endl;
        return;
    }

    if (!authSystem.currentUserHasRole("admin") &&
        !authSystem.currentUserHasRole("manager") &&
        !authSystem.currentUserHasRole("technician")) {
        std::cout << "Permission denied. Only admins, managers, and technicians can change statuses." << std::endl;
        return;
    }

    if (args.size() < 2) {
        std::cout << "Usage: status <id> <new_status> [comments]" << std::endl;
        std::cout << "Example: status 1 \"In Progress\" \"Started work on the site\"" << std::endl;
        return;
    }

    int id;
    try {
        id = std::stoi(args[0]);
    } catch (const std::exception& e) {
        std::cout << "Invalid intervention ID." << std::endl;
        return;
    }

    std::string status = args[1];
    std::string comments = args.size() > 2 ? args[2] : "";

    if (manager->changeInterventionStatus(id, status, comments)) {
        std::cout << "Status updated." << std::endl;
    } else {
        std::cout << "Failed to update status." << std::endl;
    }
}

void CLI::handleAddTechnician(const std::vector<std::string>& args) {
    if (currentUser.empty()) {
        std::cout << "Please login first." << std::endl;
        return;
    }

    if (!authSystem.currentUserHasRole("admin") && !authSystem.currentUserHasRole("manager")) {
        std::cout << "Permission denied. Only admins and managers can add technicians." << std::endl;
        return;
    }

    if (args.size() < 4) {
        std::cout << "Usage: addtech <id> <name> <specialty> <contact>" << std::endl;
        std::cout << "Example: addtech TECH003 \"John Smith\" \"Electrical\" \"john@example.com\"" << std::endl;
        return;
    }

    std::string id = args[0];
    std::string name = args[1];
    std::string specialty = args[2];
    std::string contact = args[3];

    Technician technician(id, name, specialty, contact);

    // Add the technician to the system
    // Note: We need to access this through our facade
    // For this example, we'll assume InterventionManager has an addTechnician method
    InterventionManager* realManager = manager->getRealManager();
    if (realManager != nullptr && realManager->addTechnician(technician)) {
        std::cout << "Technician added." << std::endl;

        // Also create a user account for the technician if it doesn't exist
        if (!authSystem.getUserRole(id).empty()) {
            std::cout << "User account already exists for this technician." << std::endl;
        } else {
            // Default password is the ID + "123"
            authSystem.addUser(id, id + "123", "technician");
            std::cout << "User account created for technician. Default password: " << id << "123" << std::endl;
        }
    } else {
        std::cout << "Failed to add technician." << std::endl;
    }
}

void CLI::handleListTechnicians([[maybe_unused]] const std::vector<std::string>& args) {
    if (currentUser.empty()) {
        std::cout << "Please login first." << std::endl;
        return;
    }

    InterventionManager* realManager = manager->getRealManager();
    if (realManager == nullptr) {
        std::cout << "Failed to access technician information." << std::endl;
        return;
    }

    const auto& technicians = realManager->getAllTechnicians();

    if (technicians.empty()) {
        std::cout << "No technicians found." << std::endl;
        return;
    }

    std::cout << "Technicians:" << std::endl;
    std::cout << std::left
              << std::setw(10) << "ID"
              << std::setw(20) << "Name"
              << std::setw(20) << "Specialty"
              << std::setw(20) << "Contact"
              << std::endl;

    std::cout << std::string(70, '-') << std::endl;

    for (const auto& [id, technician] : technicians) {
        std::cout << std::left
                  << std::setw(10) << id
                  << std::setw(20) << technician.getName()
                  << std::setw(20) << technician.getSpecialty()
                  << std::setw(20) << technician.getContact()
                  << std::endl;
    }
}

void CLI::handleCalendar(const std::vector<std::string>& args) {
    if (currentUser.empty()) {
        std::cout << "Please login first." << std::endl;
        return;
    }

    int month, year;

    if (args.size() < 2) {
        // Use current month and year if not provided
        std::time_t now = std::time(nullptr);
        struct std::tm* tm = std::localtime(&now);
        month = tm->tm_mon + 1; // tm_mon is 0-based
        year = tm->tm_year + 1900; // tm_year is years since 1900
    } else {
        try {
            month = std::stoi(args[0]);
            year = std::stoi(args[1]);
        } catch (const std::exception& e) {
            std::cout << "Invalid month or year." << std::endl;
            return;
        }

        if (month < 1 || month > 12) {
            std::cout << "Month must be between 1 and 12." << std::endl;
            return;
        }
    }

    // Get intervention counts by day
    auto counts = manager->getInterventionCountsForMonth(month, year);

    // Get month name
    const char* monthNames[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };

    std::cout << monthNames[month - 1] << " " << year << std::endl;
    std::cout << "Mo Tu We Th Fr Sa Su" << std::endl;

    // Calculate the first day of the month
    std::tm tm = {};
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = 1;
    std::mktime(&tm);

    // 0 = Sunday, 1 = Monday, ..., 6 = Saturday
    int firstDayOfWeek = tm.tm_wday;
    // Convert to Monday-based (0 = Monday, ..., 6 = Sunday)
    firstDayOfWeek = (firstDayOfWeek + 6) % 7;

    // Calculate the number of days in the month
    int daysInMonth;
    if (month == 2) {
        // February - check for leap year
        bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        daysInMonth = isLeapYear ? 29 : 28;
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        // April, June, September, November
        daysInMonth = 30;
    } else {
        // January, March, May, July, August, October, December
        daysInMonth = 31;
    }

    // Print leading spaces
    for (int i = 0; i < firstDayOfWeek; ++i) {
        std::cout << "   ";
    }

    // Print days with counts
    for (int day = 1; day <= daysInMonth; ++day) {
        int count = counts[day];

        if (count > 0) {
            std::cout << std::setw(2) << day << "*";
        } else {
            std::cout << std::setw(2) << day << " ";
        }

        // New line after Saturday or at the end of the month
        if ((firstDayOfWeek + day) % 7 == 0 || day == daysInMonth) {
            std::cout << std::endl;
        }
    }

    std::cout << "* = Day with interventions" << std::endl;
}

void CLI::handleInitialize([[maybe_unused]] const std::vector<std::string>& args) {
    if (currentUser.empty()) {
        std::cout << "Please login first." << std::endl;
        return;
    }

    if (!authSystem.currentUserHasRole("admin")) {
        std::cout << "Permission denied. Only admins can initialize the system." << std::endl;
        return;
    }

    InterventionManager* realManager = manager->getRealManager();
    if (realManager == nullptr) {
        std::cout << "Failed to access the manager." << std::endl;
        return;
    }

    if (realManager->initializeWithSampleData()) {
        std::cout << "System initialized with sample data." << std::endl;
    } else {
        std::cout << "Failed to initialize the system." << std::endl;
    }
}

void CLI::handleExport(const std::vector<std::string>& args) {
    if (currentUser.empty()) {
        std::cout << "Please login first." << std::endl;
        return;
    }

    if (!authSystem.currentUserHasRole("admin") && !authSystem.currentUserHasRole("manager")) {
        std::cout << "Permission denied. Only admins and managers can export data." << std::endl;
        return;
    }

    if (args.size() < 2) {
        std::cout << "Usage: export <format> <output_file>" << std::endl;
        std::cout << "Formats: text, csv, json" << std::endl;
        std::cout << "Example: export csv schedule.csv" << std::endl;
        return;
    }

    std::string format = args[0];
    std::string outputFile = args[1];

    // Convert format to lowercase
    std::transform(format.begin(), format.end(), format.begin(),
                  [](unsigned char c) { return std::tolower(c); });

    // Check format
    if (format != "text" && format != "csv" && format != "json") {
        std::cout << "Invalid format. Use one of: text, csv, json" << std::endl;
        return;
    }

    InterventionManager* realManager = manager->getRealManager();
    if (realManager == nullptr) {
        std::cout << "Failed to access the manager." << std::endl;
        return;
    }

    if (realManager->exportSchedule(format, outputFile)) {
        std::cout << "Schedule exported to " << outputFile << " in " << format << " format." << std::endl;
    } else {
        std::cout << "Failed to export schedule." << std::endl;
    }
}

void CLI::handleDecorateIntervention(const std::vector<std::string>& args) {
    if (currentUser.empty()) {
        std::cout << "Please login first." << std::endl;
        return;
    }

    if (!authSystem.currentUserHasRole("admin") && !authSystem.currentUserHasRole("manager")) {
        std::cout << "Permission denied. Only admins and managers can decorate interventions." << std::endl;
        return;
    }

    if (args.size() < 2) {
        std::cout << "Usage: decorate <id> <decorator_type>" << std::endl;
        std::cout << "Available decorator types: gps, attachments" << std::endl;
        return;
    }

    int id;
    try {
        id = std::stoi(args[0]);
    } catch (const std::exception& e) {
        std::cout << "Invalid intervention ID." << std::endl;
        return;
    }

    std::string type = args[1];

    // Get the intervention
    const Intervention* intervention = manager->getIntervention(id);
    if (intervention == nullptr) {
        std::cout << "Intervention not found." << std::endl;
        return;
    }

    // Apply decorator
    bool success = false;
    if (type == "gps") {
        success = manager->decorateWithGPS(id);
        if (success) {
            std::cout << "GPS tracking added to intervention #" << id << "." << std::endl;
        }
    } else if (type == "attachments") {
        success = manager->decorateWithAttachments(id);
        if (success) {
            std::cout << "Attachments capability added to intervention #" << id << "." << std::endl;
        }
    } else {
        std::cout << "Unknown decorator type: " << type << std::endl;
        std::cout << "Available decorator types: gps, attachments" << std::endl;
        return;
    }

    if (!success) {
        std::cout << "Failed to decorate intervention." << std::endl;
    }
}

void CLI::handleAddAttachment(const std::vector<std::string>& args) {
    if (currentUser.empty()) {
        std::cout << "Please login first." << std::endl;
        return;
    }

    if (!authSystem.currentUserHasRole("admin") &&
        !authSystem.currentUserHasRole("manager") &&
        !authSystem.currentUserHasRole("technician")) {
        std::cout << "Permission denied. Only admins, managers, and technicians can add attachments." << std::endl;
        return;
    }

    if (args.size() < 2) {
        std::cout << "Usage: addattachment <id> <filename> [<description>]" << std::endl;
        return;
    }

    int id;
    try {
        id = std::stoi(args[0]);
    } catch (const std::exception& e) {
        std::cout << "Invalid intervention ID." << std::endl;
        return;
    }

    std::string filename = args[1];
    std::string description = args.size() > 2 ? args[2] : "";

    // Add attachment
    if (manager->addAttachment(id, filename, description)) {
        std::cout << "Attachment added to intervention #" << id << "." << std::endl;
    } else {
        std::cout << "Failed to add attachment. Make sure the intervention exists and has the attachments decorator." << std::endl;
    }
}

void CLI::handleAddGPSCoordinate(const std::vector<std::string>& args) {
    if (currentUser.empty()) {
        std::cout << "Please login first." << std::endl;
        return;
    }

    if (!authSystem.currentUserHasRole("admin") &&
        !authSystem.currentUserHasRole("manager") &&
        !authSystem.currentUserHasRole("technician")) {
        std::cout << "Permission denied. Only admins, managers, and technicians can add GPS coordinates." << std::endl;
        return;
    }

    if (args.size() < 3) {
        std::cout << "Usage: addgpscoord <id> <latitude> <longitude>" << std::endl;
        return;
    }

    int id;
    double latitude, longitude;
    try {
        id = std::stoi(args[0]);
        latitude = std::stod(args[1]);
        longitude = std::stod(args[2]);
    } catch (const std::exception& e) {
        std::cout << "Invalid parameters. Please provide numeric values." << std::endl;
        return;
    }

    // Add GPS coordinate
    if (manager->addGPSCoordinate(id, latitude, longitude)) {
        std::cout << "GPS coordinate added to intervention #" << id << "." << std::endl;
    } else {
        std::cout << "Failed to add GPS coordinate. Make sure the intervention exists and has the GPS decorator." << std::endl;
    }
}