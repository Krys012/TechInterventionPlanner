//
// Created by Léo KRYS on 21/04/2025.
//

#include "Decorator/InterventionDecorator.h"

InterventionDecorator::InterventionDecorator(std::unique_ptr<Intervention> intervention)
    : intervention(std::move(intervention)) {
}

std::string InterventionDecorator::getType() const {
    return intervention->getType();
}

std::string InterventionDecorator::getInfo() const {
    return intervention->getInfo();
}

int InterventionDecorator::getId() const {
    return intervention->getId();
}

void InterventionDecorator::setId(int id) {
    intervention->setId(id);
}

std::string InterventionDecorator::getLocation() const {
    return intervention->getLocation();
}

void InterventionDecorator::setLocation(const std::string& location) {
    intervention->setLocation(location);
}

std::time_t InterventionDecorator::getDate() const {
    return intervention->getDate();
}

void InterventionDecorator::setDate(std::time_t date) {
    intervention->setDate(date);
}

int InterventionDecorator::getDuration() const {
    return intervention->getDuration();
}

void InterventionDecorator::setDuration(int duration) {
    intervention->setDuration(duration);
}

std::string InterventionDecorator::getTechnicianId() const {
    return intervention->getTechnicianId();
}

void InterventionDecorator::setTechnicianId(const std::string& technicianId) {
    intervention->setTechnicianId(technicianId);
}

std::string InterventionDecorator::getStatus() const {
    return intervention->getStatus();
}

void InterventionDecorator::setStatus(const std::string& status) {
    intervention->setStatus(status);
}

std::string InterventionDecorator::getComments() const {
    return intervention->getComments();
}

void InterventionDecorator::setComments(const std::string& comments) {
    intervention->setComments(comments);
}