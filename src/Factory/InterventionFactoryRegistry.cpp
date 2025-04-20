//
// Created by Léo KRYS on 20/04/2025.
//

#include "Factory/InterventionFactoryRegistry.h"

InterventionFactoryRegistry& InterventionFactoryRegistry::getInstance() {
    static InterventionFactoryRegistry instance;
    return instance;
}

void InterventionFactoryRegistry::registerFactory(
    const std::string& type,
    std::unique_ptr<InterventionFactory> factory) {

    factories[type] = std::move(factory);
}

const InterventionFactory* InterventionFactoryRegistry::getFactory(const std::string& type) const {
    auto it = factories.find(type);
    if (it != factories.end()) {
        return it->second.get();
    }
    return nullptr;
}

std::unique_ptr<Intervention> InterventionFactoryRegistry::createIntervention(
    const std::string& type,
    const std::string& location,
    std::time_t date,
    int duration) const {

    const InterventionFactory* factory = getFactory(type);
    if (factory) {
        return factory->createIntervention(location, date, duration);
    }
    return nullptr;
}