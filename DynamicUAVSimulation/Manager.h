#ifndef MANAGER_H
#define MANAGER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <windows.h>
#include <iomanip> // Include <iomanip> for formatting output
#include <vector> // Include <vector> for storing commands
#include "UAV.h"    // Include the UAV header file
#include <chrono> // For time measurement
#include <thread> // For sleep
#include <algorithm> // For std::sort

// Structure to hold configuration data
struct Config {
    double Dt;      // Time step
    int N_uav;      // Number of UAVs
    double R;       // Minimum turning radius
    double X0;      // Initial X coordinate
    double Y0;      // Initial Y coordinate
    double Z0;      // Initial Z coordinate
    double V0;      // Initial velocity
    double Az;      // Initial azimuth
    double TimeLim; // Time limit
};

// Structure to hold command data
struct Command {
    double time; // Command execution time
    int num;     // Number of the UAV
    double x;    // X coordinate of the new destination
    double y;    // Y coordinate of the new destination
};

// Function to get the current directory
// Retrieves the current directory path.
// Returns:
//  A string containing the current directory path.
//Function to get the current directory
std::string getCurrentDirectory();

// Function to read configuration from file
// Reads configuration parameters from a file and populates a Config struct.
// Parameters:
//  - filename: The name of the configuration file to read from.
//  - config: Reference to a Config struct to populate with configuration parameters.
// Returns:
//  True if the file was successfully read and parameters were populated, false otherwise.
bool readConfigFromFile(const std::string& filename, Config& config);

// Function to read commands from file
// Reads commands from a file and populates a vector with Command structs.
// Each command is expected to be formatted as: "<time> <num> <x> <y>"
// Parameters:
//  - filename: The name of the file containing the commands.
//  - commands: Reference to a vector to populate with Command structs.
// Returns:
//  True if the file was successfully read and commands were populated, false otherwise.
bool readCommandsFromFile(const std::string& filename, std::vector<Command>& commands);

// Function to initialize UAVs
// Initializes an array of UAVs based on the provided configuration.
// Parameters:
//  - config: The configuration object containing UAV parameters.
//  - uavs: Reference to a pointer to UAV array. This pointer will be assigned to the newly created array.
void initializeUAVs(Config& config, UAV*& uavs);

// Function to print configuration
// Prints the configuration parameters to the standard output.
// Parameters:
//  - config: The configuration object to be printed.
void printConfiguration(const Config& config);

// Function to print commands
// Prints the list of commands to the standard output.
// Parameters:
//  - commands: The vector of commands to be printe
void printCommands(const std::vector<Command>& commands);

// Function to print UAV details
// Prints details of each UAV to the standard output.
// Parameters:
//  - uavs: Pointer to the array of UAVs.
//  - num_uavs: The number of UAVs in the array.
void printUAVDetails(UAV* uavs, int num_uavs);

// Writes the current UAV details (time, position, azimuth) to a specified file.
// Parameters:
//  - filename: The name of the file to write to.
//  - currentTime: The current time.
//  - x: The X coordinate of the UAV.
//  - y: The Y coordinate of the UAV.
//  - azimuth: The azimuth angle of the UAV.
void writeUAVDetailsToFile(const std::string& filename, double currentTime, double x, double y, double azimuth);

#endif // MANAGER_H

