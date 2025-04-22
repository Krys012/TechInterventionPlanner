//
// Created by Léo KRYS on 21/04/2025.
//

#ifndef AUTHENTICATION_SYSTEM_H
#define AUTHENTICATION_SYSTEM_H

#include <string>
#include <map>

/**
 * @class AuthenticationSystem
 * @brief Handles user authentication and permissions
 */
class AuthenticationSystem {
private:
    struct UserInfo {
        std::string passwordHash;
        std::string role; // admin, manager, technician, guest

        // Add default constructor
        UserInfo() : passwordHash(""), role("") {}

        UserInfo(const std::string& pwd, const std::string& r)
            : passwordHash(pwd), role(r) {}
    };

    std::map<std::string, UserInfo> users;
    std::string currentUser;

    /**
     * @brief Hash a password (simple implementation for demo)
     * @param password Plain text password
     * @return Hashed password
     */
    std::string hashPassword(const std::string& password) const;

public:
    /**
     * @brief Constructor
     */
    AuthenticationSystem();

    /**
     * @brief Add a user to the system
     * @param username Username
     * @param password Plain text password
     * @param role User role (admin, manager, technician, guest)
     * @return True if user was added
     */
    bool addUser(const std::string& username, const std::string& password, const std::string& role);

    /**
     * @brief Authenticate a user
     * @param username Username
     * @param password Plain text password
     * @return True if authentication succeeded
     */
    bool authenticate(const std::string& username, const std::string& password);

    /**
     * @brief Log out the current user
     */
    void logout();

    /**
     * @brief Get the current user
     * @return Username of current user, or empty string if not logged in
     */
    std::string getCurrentUser() const;

    /**
     * @brief Get the role of a user
     * @param username Username
     * @return Role of the user, or empty string if user not found
     */
    std::string getUserRole(const std::string& username) const;

    /**
     * @brief Check if the current user has a specific role
     * @param role Role to check
     * @return True if current user has the role
     */
    bool currentUserHasRole(const std::string& role) const;
};

#endif // AUTHENTICATION_SYSTEM_H