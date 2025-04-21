//
// Created by Léo KRYS on 21/04/2025.
//

#include "Decorator/GPSTrackingDecorator.h"
#include <sstream>
#include <iomanip>
#include <ctime>

GPSTrackingDecorator::GPSTrackingDecorator(std::unique_ptr<Intervention> intervention, int updateFrequency)
    : InterventionDecorator(std::move(intervention)), updateFrequency(updateFrequency) {
    // Validate frequency
    if (this->updateFrequency < 1) this->updateFrequency = 1;
}

void GPSTrackingDecorator::addCoordinate(double latitude, double longitude, std::time_t timestamp) {
    trackingData.emplace_back(latitude, longitude, timestamp);
}

std::pair<double, double> GPSTrackingDecorator::getCurrentLocation() const {
    if (trackingData.empty()) {
        return {0.0, 0.0}; // No data available
    }

    // Return the most recent coordinate
    const auto& latest = trackingData.back();
    return {latest.latitude, latest.longitude};
}

void GPSTrackingDecorator::setUpdateFrequency(int minutes) {
    if (minutes < 1) minutes = 1;
    updateFrequency = minutes;
}

int GPSTrackingDecorator::getUpdateFrequency() const {
    return updateFrequency;
}

const std::vector<GPSTrackingDecorator::GPSCoordinate>& GPSTrackingDecorator::getTrackingData() const {
    return trackingData;
}

std::string GPSTrackingDecorator::getInfo() const {
    // Get the base info from the wrapped intervention
    std::string baseInfo = InterventionDecorator::getInfo();

    std::ostringstream gpsInfo;
    gpsInfo << baseInfo << std::endl << "GPS Tracking: Enabled";
    gpsInfo << " (Update frequency: " << updateFrequency << " minutes)";

    if (!trackingData.empty()) {
        const auto& latest = trackingData.back();

        // Format the timestamp
        char timeBuffer[25];
        std::strftime(timeBuffer, sizeof(timeBuffer), "%m/%d/%Y %H:%M:%S",
                     std::localtime(&latest.timestamp));

        gpsInfo << std::endl << "Last position: "
                << std::fixed << std::setprecision(6)
                << latest.latitude << ", " << latest.longitude
                << " (as of " << timeBuffer << ")";

        gpsInfo << std::endl << "Total tracking points: " << trackingData.size();
    }

    return gpsInfo.str();
}