//
// Created by Léo KRYS on 20/04/2025.
//

#ifndef INTERVENTION_FACTORY_REGISTRY_H
#define INTERVENTION_FACTORY_REGISTRY_H

#include <map>
#include <string>
#include <memory>
#include "Factory/InterventionFactory.h"

/**
 * @class InterventionFactoryRegistry
 * @brief Singleton registry for intervention factories
 */
class InterventionFactoryRegistry {
private:
    std::map<std::string, std::unique_ptr<InterventionFactory>> factories;

    // Private constructor for singleton
    InterventionFactoryRegistry() = default;

public:
    // Delete copy constructor and assignment operator
    InterventionFactoryRegistry(const InterventionFactoryRegistry&) = delete;
    InterventionFactoryRegistry& operator=(const InterventionFactoryRegistry&) = delete;

    /**
     * @brief Get the singleton instance
     * @return Reference to the singleton instance
     */
    static InterventionFactoryRegistry& getInstance();

    /**
     * @brief Register a factory for a type
     * @param type The intervention type
     * @param factory Unique pointer to the factory
     */
    void registerFactory(const std::string& type, std::unique_ptr<InterventionFactory> factory);

    /**
     * @brief Get a factory for a type
     * @param type The intervention type
     * @return Pointer to the factory, or nullptr if not found
     */
    const InterventionFactory* getFactory(const std::string& type) const;

    /**
     * @brief Create an intervention of a specific type
     * @param type The intervention type
     * @param location The location of the intervention
     * @param date The date and time of the intervention
     * @param duration The duration in minutes
     * @return Unique pointer to a new intervention, or nullptr if type is invalid
     */
    std::unique_ptr<Intervention> createIntervention(
        const std::string& type,
        const std::string& location,
        std::time_t date,
        int duration) const;
};

#endif // INTERVENTION_FACTORY_REGISTRY_H