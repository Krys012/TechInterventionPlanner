//
// Created by Léo KRYS on 20/04/2025.
//

#include "Model/Technician.h"
#include <sstream>

Technician::Technician()
    : id(""), name(""), specialty(""), contact("") {
}

Technician::Technician(const std::string& id, const std::string& name,
                     const std::string& specialty, const std::string& contact)
    : id(id), name(name), specialty(specialty), contact(contact) {
}

std::string Technician::getId() const {
    return id;
}

void Technician::setId(const std::string& id) {
    this->id = id;
}

std::string Technician::getName() const {
    return name;
}

void Technician::setName(const std::string& name) {
    this->name = name;
}

std::string Technician::getSpecialty() const {
    return specialty;
}

void Technician::setSpecialty(const std::string& specialty) {
    this->specialty = specialty;
}

std::string Technician::getContact() const {
    return contact;
}

void Technician::setContact(const std::string& contact) {
    this->contact = contact;
}

std::string Technician::getInfo() const {
    std::ostringstream oss;

    oss << "Technician #" << id << std::endl
        << "Name: " << name << std::endl
        << "Specialty: " << specialty << std::endl
        << "Contact: " << contact;

    return oss.str();
}