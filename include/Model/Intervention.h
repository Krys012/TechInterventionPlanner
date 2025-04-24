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

    // Getters and setters - all made virtual
    virtual int getId() const;
    virtual void setId(int id);

    virtual std::string getLocation() const;
    virtual void setLocation(const std::string& location);

    virtual std::time_t getDate() const;
    virtual void setDate(std::time_t date);

    virtual int getDuration() const;
    virtual void setDuration(int duration);

    virtual std::string getTechnicianId() const;
    virtual void setTechnicianId(const std::string& technicianId);

    virtual std::string getStatus() const;
    virtual void setStatus(const std::string& status);

    virtual std::string getComments() const;
    virtual void setComments(const std::string& comments);

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

    virtual void clear();

};

#endif // INTERVENTION_H