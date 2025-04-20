//
// Created by Léo KRYS on 20/04/2025.
//
#ifndef INTERVENTION_H
#define INTERVENTION_H

#include <string>
#include <ctime>

/**
 * @class Intervention
 * @brief Abstract class representing a technical intervention
 */
class Intervention {
protected:
    int id;
    std::string location;
    std::time_t date;
    int duration;  // In minutes
    std::string technicianId;
    std::string status;
    std::string comments;

public:
    /**
     * @brief Default constructor
     */
    Intervention();

    /**
     * @brief Constructor with parameters
     * @param location Location of the intervention
     * @param date Date and time of the intervention
     * @param duration Expected duration in minutes
     */
    Intervention(const std::string& location, std::time_t date, int duration);

    /**
     * @brief Virtual destructor
     */
    virtual ~Intervention() = default;

    // Getters and setters
    int getId() const;
    void setId(int id);

    std::string getLocation() const;
    void setLocation(const std::string& location);

    std::time_t getDate() const;
    void setDate(std::time_t date);

    int getDuration() const;
    void setDuration(int duration);

    std::string getTechnicianId() const;
    void setTechnicianId(const std::string& technicianId);

    std::string getStatus() const;
    void setStatus(const std::string& status);

    std::string getComments() const;
    void setComments(const std::string& comments);

    /**
     * @brief Pure virtual method to get the intervention type
     * @return Type of intervention as a string
     */
    virtual std::string getType() const = 0;

    /**
     * @brief Get detailed information about the intervention
     * @return Formatted information as a string
     */
    virtual std::string getInfo() const;
};

#endif // INTERVENTION_H