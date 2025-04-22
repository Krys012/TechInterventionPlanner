//
// Created by Léo KRYS on 21/04/2025.
//

#include "CLI/AuthenticationSystem.h"
#include <algorithm>
#include <functional>

AuthenticationSystem::AuthenticationSystem() {
    // Add some default users
    addUser("admin", "admin123", "admin");
    addUser("manager", "manager123", "manager");
    addUser("tech", "tech123", "technician");
    addUser("guest", "guest123", "guest");
}

std::string AuthenticationSystem::hashPassword(const std::string& password) const {
    // This is a very simple hash for demonstration purposes only
    // In a real system, use a secure hashing library with salting

    std::string result = password;
    std::hash<std::string> hasher;
    return std::to_string(hasher(result));
}

bool AuthenticationSystem::addUser(const std::string& username, const std::string& password, const std::string& role) {
    // Check if user already exists
    if (users.find(username) != users.end()) {
        return false;
    }

    // Validate role
    if (role != "admin" && role != "manager" && role != "technician" && role != "guest") {
        return false;
    }

    // Create user
    users[username] = UserInfo(hashPassword(password), role);
    return true;
}

bool AuthenticationSystem::authenticate(const std::string& username, const std::string& password) {
    auto it = users.find(username);
    if (it == users.end()) {
        return false;
    }

    if (it->second.passwordHash == hashPassword(password)) {
        currentUser = username;
        return true;
    }

    return false;
}

void AuthenticationSystem::logout() {
    currentUser = "";
}

std::string AuthenticationSystem::getCurrentUser() const {
    return currentUser;
}

std::string AuthenticationSystem::getUserRole(const std::string& username) const {
    auto it = users.find(username);
    if (it == users.end()) {
        return "";
    }

    return it->second.role;
}

bool AuthenticationSystem::currentUserHasRole(const std::string& role) const {
    if (currentUser.empty()) {
        return false;
    }

    // Admin role has access to everything
    std::string userRole = getUserRole(currentUser);
    if (userRole == "admin") {
        return true;
    }

    // Exact role match
    if (userRole == role) {
        return true;
    }

    // Manager can do what technicians can do
    if (userRole == "manager" && role == "technician") {
        return true;
    }

    return false;
}