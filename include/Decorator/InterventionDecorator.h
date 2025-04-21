//
// Created by Léo KRYS on 21/04/2025.
//

#ifndef INTERVENTION_DECORATOR_H
#define INTERVENTION_DECORATOR_H

#include "Model/Intervention.h"
#include <memory>

/**
 * @class InterventionDecorator
 * @brief Base decorator class for adding functionality to interventions
 */
class InterventionDecorator : public Intervention {
protected:
    std::unique_ptr<Intervention> intervention;

public:
    /**
     * @brief Constructor
     * @param intervention The intervention to decorate
     */
    explicit InterventionDecorator(std::unique_ptr<Intervention> intervention);

    /**
     * @brief Get the type of the underlying intervention
     * @return Type as string
     */
    std::string getType() const override;

    /**
     * @brief Get information about the intervention
     * @return Formatted string with intervention details
     */
    std::string getInfo() const override;

    // Override all the getter/setter methods to delegate to the wrapped intervention
    int getId() const override;
    void setId(int id) override;

    std::string getLocation() const override;
    void setLocation(const std::string& location) override;

    std::time_t getDate() const override;
    void setDate(std::time_t date) override;

    int getDuration() const override;
    void setDuration(int duration) override;

    std::string getTechnicianId() const override;
    void setTechnicianId(const std::string& technicianId) override;

    std::string getStatus() const override;
    void setStatus(const std::string& status) override;

    std::string getComments() const override;
    void setComments(const std::string& comments) override;
};

#endif // INTERVENTION_DECORATOR_H
