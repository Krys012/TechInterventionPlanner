# TechInterventionPlanner - Maintenance & Troubleshooting Guide

This document provides information for developers maintaining the TechInterventionPlanner application and addressing common issues.

## Common Build Issues

### Missing Header Files

If you encounter errors about missing header files:

```
error: '#include' file not found
```

**Solution**:
- Check that all include paths are properly set in CMakeLists.txt
- Verify file paths in #include directives
- Make sure the file exists in the specified location

### iostream/fstream Errors

If you see errors related to std::ofstream or other stream classes:

```
error: implicit instantiation of undefined template 'std::basic_ofstream<char>'
```

**Solution**:
- Add `#include <fstream>` to any file using file stream operations
- Ensure you're not using file operations without the proper header

### Linking Errors

For undefined reference errors:

```
undefined reference to 'InterventionManager::exportSchedule'
```

**Solution**:
- Ensure the function is both declared in the header and implemented in the .cpp file
- Check for mismatched function signatures between declaration and implementation
- Verify the function is being compiled (included in CMakeLists.txt source list)

## Code Maintenance

### Adding New Classes

When adding new classes to the project:

1. Create header file in the appropriate include/ subdirectory
2. Create implementation file in the corresponding src/ subdirectory
3. Add both files to CMakeLists.txt in the HEADERS and SOURCES sections
4. Include necessary headers in your new files

Example CMakeLists.txt update:

```cmake
set(HEADERS
    # Existing headers
    include/NewDirectory/NewClass.h
)

set(SOURCES
    # Existing sources
    src/NewDirectory/NewClass.cpp
)
```

### Modifying the Interface

When changing the `IInterventionManager` interface:

1. Update the interface in `include/Proxy/IInterventionManager.h`
2. Update both implementations:
    - `src/Facade/InterventionManager.cpp`
    - `src/Proxy/InterventionManagerSecure.cpp`
3. Update any CLI commands that use the modified methods

### Adding CLI Commands

To add new commands to the CLI:

1. Add a command handler method to CLI.h:
```cpp
void handleNewCommand(const std::vector<std::string>& args);
```

2. Implement the handler in CLI.cpp
3. Register the command in initializeCommands():
```cpp
commandHandlers["newcmd"] = [this](const auto& args) { handleNewCommand(args); };
commandHelp["newcmd"] = "Description of new command";
```

## Memory Management

The application uses smart pointers for automatic memory management:

- `std::unique_ptr` for exclusive ownership (most object instances)
- `std::shared_ptr` for shared ownership (observers, etc.)

### Common Memory Issues

If you encounter memory issues:

1. Check for raw pointer usage without proper cleanup
2. Ensure smart pointers are used for all dynamically allocated objects
3. Look for circular references that might prevent object destruction
4. Verify that no dangling pointers are being used

## Export Functionality

The export feature replaces the previous save/load functionality with a one-way export to different file formats.

### Supporting a New Export Format

To add a new export format:

1. Add a new format option in the `exportSchedule` method in `InterventionManager.cpp`
2. Implement the export logic for the new format
3. Update the CLI documentation to mention the new format

Example:

```cpp
else if (format == "xml") {
    // XML export implementation
    file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    file << "<interventions>\n";
    
    for (const auto& pair : interventions) {
        const auto& id = pair.first;
        const auto& intervention = pair.second;
        
        // Format XML output
        // ...
    }
    
    file << "</interventions>";
}
```

## Observer Pattern

The Observer pattern is used for notifications in the system. Common maintenance tasks include:

### Adding a New Observer Type

1. Create a new class inheriting from `InterventionObserver`
2. Implement the `notify` method
3. Optionally override `isInterestedIn` to filter events

Example:

```cpp
class EmailObserver : public InterventionObserver {
private:
    std::string emailServer;
    
public:
    explicit EmailObserver(const std::string& server);
    void notify(const std::string& message) override;
    bool isInterestedIn(const std::string& eventType) const override;
};
```

### Common Observer Issues

- **Silent observers**: Check if observers are properly registered with the subject
- **Too many notifications**: Use the `isInterestedIn` method to filter events
- **Memory leaks**: Use shared_ptr for observer registration

## Initialization System

The system provides an `initialize` command to populate data. To modify the sample data:

1. Locate the `initializeWithSampleData` method in `InterventionManager.cpp`
2. Modify the sample interventions and technicians as needed
3. Consider adding different initialization options if needed

## Troubleshooting Guide

### Authentication Issues

If users can't log in:

1. Check default credentials in `AuthenticationSystem.cpp`
2. Verify that the `authenticate` method is working correctly
3. Ensure the password hashing method (even if simple) is consistent

### Command Execution Failures

If commands fail silently:

1. Check access control in `InterventionManagerSecure` - users might not have permission
2. Look for exceptions being caught without proper error reporting
3. Verify that the command arguments are parsed correctly

### Export Failures

If exporting data fails:

1. Verify that the output directory exists and is writable
2. Check for special characters in filenames
3. Ensure the file format is supported
4. Check if the file is already open by another process

### Intervention Creation Issues

If interventions can't be created:

1. Verify that the factory for the specified type is registered
2. Check that date formats are parsed correctly
3. Ensure the intervention planner accepts the scheduled time

## Logging

The application uses a simple logging system through observers. To improve logging:

1. Ensure a `LogObserver` is registered with the intervention manager
2. Check that the log file location is writable
3. Consider implementing log rotation for long-running instances

## Performance Optimization

If the application becomes slow with many interventions:

1. Consider using more efficient containers for frequent operations
2. Optimize the intervention lookup in calendar views
3. Batch notifications when making multiple changes
4. Profile the code to identify bottlenecks

## Extending the Export System

If you need more advanced export capabilities:

1. Consider implementing a Strategy pattern for different export formats
2. Add options for filtering which data gets exported
3. Implement import functionality if needed in the future
4. Add export progress reporting for large datasets