//
// Created by Léo KRYS on 20/04/2025.
//

#ifndef TECHNICIAN_H
#define TECHNICIAN_H

#include <string>
#include <vector>

/**
 * @class Technician
 * @brief Represents a technician who performs interventions
 */
class Technician {
private:
    std::string id;
    std::string name;
    std::string specialty;
    std::string contact;

public:
    /**
     * @brief Default constructor
     */
    Technician();

    /**
     * @brief Constructor with parameters
     * @param id Unique identifier for the technician
     * @param name Full name of the technician
     * @param specialty Area of specialization
     * @param contact Contact details
     */
    Technician(const std::string& id, const std::string& name,
              const std::string& specialty, const std::string& contact);

    // Getters and setters
    std::string getId() const;
    void setId(const std::string& id);

    std::string getName() const;
    void setName(const std::string& name);

    std::string getSpecialty() const;
    void setSpecialty(const std::string& specialty);

    std::string getContact() const;
    void setContact(const std::string& contact);

    /**
     * @brief Get technician information
     * @return Formatted information as a string
     */
    std::string getInfo() const;
};

#endif // TECHNICIAN_H