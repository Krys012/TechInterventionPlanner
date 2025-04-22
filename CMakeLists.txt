cmake_minimum_required(VERSION 3.12)
project(TechInterventionPlanner VERSION 1.0)

# Définir le standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options de compilation
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Activer l'option de debug pour les configurations Debug
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

# Structure du projet organisée par patterns et logique métier
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

# Récupérer tous les fichiers d'en-tête
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

# Configurer les répertoires d'inclusion
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

# Créer l'exécutable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Adapations spécifiques par OS
if(WIN32)
    # Configurations spécifiques à Windows
    target_compile_definitions(${PROJECT_NAME} PRIVATE WINDOWS_PLATFORM)
elseif(APPLE)
    # Configurations spécifiques à macOS
    target_compile_definitions(${PROJECT_NAME} PRIVATE MACOS_PLATFORM)
elseif(UNIX)
    # Configurations spécifiques à Linux
    target_compile_definitions(${PROJECT_NAME} PRIVATE LINUX_PLATFORM)
endif()

# Installer l'exécutable
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# Faciliter le débogage avec des messages de statut
message(STATUS "Système d'exploitation: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Compilateur: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Mode de compilation: ${CMAKE_BUILD_TYPE}")

# Option pour permettre des tests unitaires (à implémenter ultérieurement)
option(BUILD_TESTS "Build tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()