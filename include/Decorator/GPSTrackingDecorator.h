//
// Created by Léo KRYS on 21/04/2025.
//

#ifndef GPS_TRACKING_DECORATOR_H
#define GPS_TRACKING_DECORATOR_H

#include "Decorator/InterventionDecorator.h"
#include <string>
#include <vector>
#include <utility> // for std::pair

/**
 * @class GPSTrackingDecorator
 * @brief Adds GPS tracking functionality to an intervention
 */
class GPSTrackingDecorator : public InterventionDecorator {
public:
    struct GPSCoordinate {
        double latitude;
        double longitude;
        std::time_t timestamp;

        GPSCoordinate(double lat, double lon, std::time_t time)
            : latitude(lat), longitude(lon), timestamp(time) {}
    };

private:
    std::vector<GPSCoordinate> trackingData;
    int updateFrequency; // in minutes

public:
    /**
     * @brief Constructor
     * @param intervention The intervention to decorate
     * @param updateFrequency How often to update coordinates (in minutes)
     */
    GPSTrackingDecorator(std::unique_ptr<Intervention> intervention, int updateFrequency = 15);

    /**
     * @brief Add a GPS coordinate to the tracking data
     * @param latitude Latitude coordinate
     * @param longitude Longitude coordinate
     * @param timestamp Time the coordinate was recorded (defaults to current time)
     */
    void addCoordinate(double latitude, double longitude, std::time_t timestamp = std::time(nullptr));

    /**
     * @brief Get the most recent GPS coordinate
     * @return Pair of latitude and longitude, or {0,0} if no data
     */
    std::pair<double, double> getCurrentLocation() const;

    /**
     * @brief Set the update frequency
     * @param minutes Minutes between updates
     */
    void setUpdateFrequency(int minutes);

    /**
     * @brief Get the update frequency
     * @return Minutes between updates
     */
    int getUpdateFrequency() const;

    /**
     * @brief Get all tracking data
     * @return Vector of GPS coordinates
     */
    const std::vector<GPSCoordinate>& getTrackingData() const;

    /**
     * @brief Override getInfo to include GPS information
     * @return Formatted string with intervention details including GPS
     */
    std::string getInfo() const override;
};

#endif // GPS_TRACKING_DECORATOR_H