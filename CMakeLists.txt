cmake_minimum_required(VERSION 3.12)
project(TechInterventionPlanner VERSION 1.0)

# Define C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compilation options
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Enable debug option for Debug configurations
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

# Add nlohmann/json as a dependency
include(FetchContent)
FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.2  # You can update to the latest version available
)
FetchContent_MakeAvailable(json)

# Project structure organized by patterns and business logic
set(SOURCES
        src/Model/Intervention.cpp
        src/Model/Technician.cpp
        src/Model/MaintenanceIntervention.cpp
        src/Model/EmergencyIntervention.cpp
        src/Business/InterventionPlanner.cpp
        src/Business/TechnicianManager.cpp
        src/Business/NotificationSystem.cpp
        src/Factory/MaintenanceFactory.cpp
        src/Factory/EmergencyFactory.cpp
        src/Factory/InterventionFactoryRegistry.cpp
        src/main.cpp
        src/Observer/ConsoleObserver.cpp
        src/Observer/LogObserver.cpp
        src/Observer/TechnicianNotificationObserver.cpp
        src/Decorator/InterventionDecorator.cpp
        src/Decorator/GPSTrackingDecorator.cpp
        src/Decorator/AttachmentsDecorator.cpp
        src/Proxy/InterventionManagerSecure.cpp
        src/Facade/InterventionManager.cpp
        src/CLI/AuthenticationSystem.cpp
        src/CLI/CLI.cpp

)

# Get all header files
set(HEADERS
        include/Model/Intervention.h
        include/Model/Technician.h
        include/Model/EmergencyIntervention.h
        include/Model/MaintenanceIntervention.h
        include/Business/TechnicianManager.h
        include/Business/NotificationSystem.h
        include/Factory/InterventionFactory.h
        include/Factory/MaintenanceFactory.h
        include/Factory/EmergencyFactory.h
        include/Factory/InterventionFactoryRegistry.h
        include/Observer/InterventionObserver.h
        include/Observer/ConsoleObserver.h
        include/Observer/LogObserver.h
        include/Observer/TechnicianNotificationObserver.h
        include/Decorator/InterventionDecorator.h
        include/Decorator/GPSTrackingDecorator.h
        include/Decorator/AttachmentsDecorator.h
        include/Proxy/IInterventionManager.h
        include/Proxy/InterventionManagerSecure.h
        include/Facade/InterventionManager.h
        include/CLI/CLI.h
        include/CLI/AuthenticationSystem.h

)

# Configure include directories
include_directories(
        include
        include/Model
        include/Business
        include/Factory
        include/Decorator
        include/Observer
        include/Facade
        include/Proxy
        include/CLI
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link against nlohmann_json
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

# OS-specific adaptations
if(WIN32)
    # Windows-specific configurations
    target_compile_definitions(${PROJECT_NAME} PRIVATE WINDOWS_PLATFORM)
elseif(APPLE)
    # macOS-specific configurations
    target_compile_definitions(${PROJECT_NAME} PRIVATE MACOS_PLATFORM)
elseif(UNIX)
    # Linux-specific configurations
    target_compile_definitions(${PROJECT_NAME} PRIVATE LINUX_PLATFORM)
endif()

# Install the executable
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# Facilitate debugging with status messages
message(STATUS "Operating system: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Build mode: ${CMAKE_BUILD_TYPE}")

# Option to enable unit tests (to be implemented later)
option(BUILD_TESTS "Build tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()