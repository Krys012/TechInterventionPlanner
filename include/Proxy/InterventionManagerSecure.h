//
// Created by Léo KRYS on 21/04/2025.
//

#ifndef INTERVENTION_MANAGER_SECURE_H
#define INTERVENTION_MANAGER_SECURE_H

#include "Proxy/IInterventionManager.h"
#include "Facade/InterventionManager.h"
#include <string>
#include <map>
#include <memory>

/**
 * @class InterventionManagerSecure
 * @brief Proxy that controls access to intervention management operations
 *
 * This proxy ensures that users have appropriate permissions before
 * allowing them to perform operations on interventions.
 */
class InterventionManagerSecure : public IInterventionManager {
public:
    /**
     * @brief Enum representing access levels
     */
    enum class AccessLevel {
        NONE,       // No access
        READ,       // Read-only access
        MODIFY,     // Can modify interventions
        FULL        // Full access including creation and deletion
    };

private:
    std::unique_ptr<InterventionManager> realManager;
    std::string currentUser;
    std::map<std::string, AccessLevel> userAccessLevels;

    /**
     * @brief Check if the current user has a required access level
     * @param requiredLevel Minimum access level required
     * @return True if user has sufficient access
     */
    bool checkAccess(AccessLevel requiredLevel) const;

    /**
     * @brief Log an access attempt
     * @param operation Operation attempted
     * @param granted Whether access was granted
     */
    void logAccessAttempt(const std::string& operation, bool granted) const;

public:
    /**
     * @brief Constructor
     * @param currentUser ID of the current user
     */
    explicit InterventionManagerSecure(const std::string& currentUser);

    /**
     * @brief Set access level for a user
     * @param userId ID of the user
     * @param level Access level to set
     */
    void setUserAccessLevel(const std::string& userId, AccessLevel level);

    /**
     * @brief Get access level for the current user
     * @return Current user's access level
     */
    AccessLevel getCurrentUserAccessLevel() const;

    // IInterventionManager interface implementation
    int createIntervention(const std::string& type,
                          const std::string& location,
                          std::time_t date,
                          int duration) override;

    bool deleteIntervention(int interventionId) override;

    bool modifyIntervention(int interventionId,
                          const std::string& location,
                          std::time_t date,
                          int duration) override;

    bool changeInterventionStatus(int interventionId,
                                const std::string& status,
                                const std::string& comments = "") override;

    bool assignTechnician(int interventionId,
                        const std::string& technicianId) override;

    const Intervention* getIntervention(int interventionId) const override;

    std::vector<const Intervention*> getInterventionsForDay(std::time_t date) const override;

    std::map<int, int> getInterventionCountsForMonth(int month, int year) const override;

   /**
    * @brief Get access to the real manager
    * @return Pointer to the real InterventionManager
    * @note This method should be used with caution as it bypasses security checks
    */
   InterventionManager* getRealManager() const;

    bool decorateWithGPS(int interventionId) override;

    bool decorateWithAttachments(int interventionId) override;

    bool addGPSCoordinate(int interventionId, double latitude, double longitude) override;

    bool addAttachment(int interventionId, const std::string &filename, const std::string &description) override;

    virtual ~InterventionManagerSecure() = default;
};

#endif // INTERVENTION_MANAGER_SECURE_H