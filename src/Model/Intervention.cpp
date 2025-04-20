//
// Created by Léo KRYS on 20/04/2025.
//
#include "Model/Intervention.h"
#include <sstream>
#include <iomanip>
#include <ctime>

Intervention::Intervention()
    : id(0), location(""), date(0), duration(0),
      technicianId(""), status("Scheduled"), comments("") {
}

Intervention::Intervention(const std::string& location, std::time_t date, int duration)
    : id(0), location(location), date(date), duration(duration),
      technicianId(""), status("Scheduled"), comments("") {
}

int Intervention::getId() const {
    return id;
}

void Intervention::setId(int id) {
    this->id = id;
}

std::string Intervention::getLocation() const {
    return location;
}

void Intervention::setLocation(const std::string& location) {
    this->location = location;
}

std::time_t Intervention::getDate() const {
    return date;
}

void Intervention::setDate(std::time_t date) {
    this->date = date;
}

int Intervention::getDuration() const {
    return duration;
}

void Intervention::setDuration(int duration) {
    this->duration = duration;
}

std::string Intervention::getTechnicianId() const {
    return technicianId;
}

void Intervention::setTechnicianId(const std::string& technicianId) {
    this->technicianId = technicianId;
}

std::string Intervention::getStatus() const {
    return status;
}

void Intervention::setStatus(const std::string& status) {
    this->status = status;
}

std::string Intervention::getComments() const {
    return comments;
}

void Intervention::setComments(const std::string& comments) {
    this->comments = comments;
}

std::string Intervention::getInfo() const {
    std::ostringstream oss;

    // Format the date
    struct tm* timeinfo = std::localtime(&date);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%m/%d/%Y %H:%M", timeinfo);

    oss << "Intervention #" << id << " (" << getType() << ")" << std::endl
        << "Location: " << location << std::endl
        << "Date: " << buffer << std::endl
        << "Duration: " << duration << " minutes" << std::endl
        << "Technician: " << technicianId << std::endl
        << "Status: " << status;

    if (!comments.empty()) {
        oss << std::endl << "Comments: " << comments;
    }

    return oss.str();
}
