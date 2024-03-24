#include "Manager.h"

// Function to get the current directory
std::string getCurrentDirectory() {
    char buffer[MAX_PATH];
    DWORD result = GetCurrentDirectoryA(MAX_PATH, buffer); // Retrieve current directory
    if (result == 0) {
        std::cerr << "Error getting current directory." << std::endl; // Print error message if failed
        return "";
    }
    return std::string(buffer); // Return current directory as string
}

// Function to read configuration from file
bool readConfigFromFile(const std::string& filename, Config& config) {
    std::string currentDir = getCurrentDirectory(); // Get current directory
    std::ifstream file(currentDir + "\\" + filename); // Open file in current directory

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file." << std::endl; // Print error message if failed to open file
        return false;
    }

    std::string line;
    while (std::getline(file, line)) { // Read each line of the file
        std::istringstream iss(line); // Create a string stream from the line
        std::string key, valueString;
        if (std::getline(iss, key, '=')) { // Split line into key and value
            if (std::getline(iss, valueString)) {
                double value = std::stod(valueString); // Convert value to double
                // Set configuration parameters based on key
                if (key == "Dt") {
                    config.Dt = value;
                }
                else if (key == "N_uav") {
                    config.N_uav = static_cast<int>(value);
                }
                else if (key == "R") {
                    config.R = value;
                }
                else if (key == "X0") {
                    config.X0 = value;
                }
                else if (key == "Y0") {
                    config.Y0 = value;
                }
                else if (key == "Z0") {
                    config.Z0 = value;
                }
                else if (key == "V0") {
                    config.V0 = value;
                }
                else if (key == "Az") {
                    config.Az = value;
                }
                else if (key == "TimeLim") {
                    config.TimeLim = value;
                }
            }
        }
    }

    file.close(); // Close the file
    return true;
}

// Function to read commands from file
bool readCommandsFromFile(const std::string& filename, std::vector<Command>& commands) {
    std::string currentDir = getCurrentDirectory(); // Get current directory
    std::ifstream file(currentDir + "\\" + filename); // Open file in current directory

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file." << std::endl; // Print error message if failed to open file
        return false;
    }

    std::string line;
    while (std::getline(file, line)) { // Read each line of the file
        std::istringstream iss(line); // Create a string stream from the line
        Command command;
        if (iss >> command.time >> command.num >> command.x >> command.y) { // Read command data
            commands.push_back(command); // Store command in the vector
        }
    }

    file.close(); // Close the file
    return true;
}


// Function to initialize UAVs
void initializeUAVs(Config& config, UAV*& uavs) {
    uavs = new UAV[config.N_uav];
    for (int i = 0; i < config.N_uav; ++i) {
        uavs[i].setNum(i + 1);
        uavs[i].setX(config.X0);
        uavs[i].setY(config.Y0);
        uavs[i].setAzimuth(config.Az);
        uavs[i].setZ(config.Z0);
        uavs[i].setV(config.V0);
        uavs[i].setR(config.R);
    }
}

// Function to print configuration
void printConfiguration(const Config& config) {
    std::cout << "Configuration:" << std::endl;
    std::cout << "Dt: " << config.Dt << std::endl;
    std::cout << "N_uav: " << config.N_uav << std::endl;
    std::cout << "R: " << std::fixed << std::setprecision(1) << config.R << std::endl;
    std::cout << "X0: " << std::fixed << std::setprecision(2) << config.X0 << std::endl;
    std::cout << "Y0: " << std::fixed << std::setprecision(2) << config.Y0 << std::endl;
    std::cout << "Z0: " << std::fixed << std::setprecision(1) << config.Z0 << std::endl;
    std::cout << "V0: " << std::fixed << std::setprecision(1) << config.V0 << std::endl;
    std::cout << "Az: " << std::fixed << std::setprecision(2) << config.Az << std::endl;
    std::cout << "TimeLim: " << std::fixed << std::setprecision(2) << config.TimeLim << std::endl;
}

// Function to print commands
void printCommands(const std::vector<Command>& commands) {
    std::cout << "\nCommands:" << std::endl;
    for (const auto& command : commands) {
        std::cout << "Time: " << command.time << ", Num: " << command.num
            << ", X: " << command.x << ", Y: " << command.y << std::endl;
    }
}

// Function to print UAV details
void printUAVDetails(UAV* uavs, int num_uavs) {
    std::cout << "\nUAV's Details:\n" << std::endl;
    for (int i = 0; i < num_uavs; ++i) {
        std::cout << "UAV" << uavs[i].getNum() << " Details:" << std::endl;
        std::cout << "Num: " << uavs[i].getNum() << std::endl;
        std::cout << "X: " << uavs[i].getX() << std::endl;
        std::cout << "Y: " << uavs[i].getY() << std::endl;
        std::cout << "Azimuth: " << uavs[i].getAzimuth() << std::endl;
        std::cout << "Z: " << uavs[i].getZ() << std::endl;
        std::cout << "V: " << uavs[i].getV() << std::endl;
        std::cout << "R: " << uavs[i].getR() << std::endl;
        std::cout << std::endl;
    }
}

// Writes the current UAV details (time, position, azimuth) to a specified file.
void writeUAVDetailsToFile(const std::string& filename, double currentTime, double x, double y, double azimuth) {
    std::ofstream outputFile(filename, std::ios_base::app); // Open in append mode
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return; // Exit if the file could not be opened
    }
    outputFile << std::fixed << std::setprecision(2) << currentTime << " "
        << x << " " << y << " " << azimuth << std::endl;
}


int main() {
    Config config; // Create a configuration object
    std::vector<Command> commands; // Vector to store commands
    double currentTime = 0.0; // Initialize current time
    UAV* uavs = nullptr; // Pointer to UAVs array

    //Reads SimParams file
    if (!readConfigFromFile("SimParams.ini", config)) {
        return 1;
    }
    //Reads SimCmds file
    if (!readCommandsFromFile("SimCmds.txt", commands)) {
        return 1;
    }

    // Initialize a vector to store the last executed command time for each UAV.
    std::vector<int> uavCommands(config.N_uav, -1);

    initializeUAVs(config, uavs);
    printConfiguration(config);
    printCommands(commands);

    // Sort commands vector in descending order based on time
    std::sort(commands.begin(), commands.end(), [](const Command& a, const Command& b) {
        return a.time > b.time;
        });

    printUAVDetails(uavs, config.N_uav);

    // Delete old files before the main simulation loop starts
    for (int i = 0; i < config.N_uav; ++i) {
        std::string filename = "UAV" + std::to_string(uavs[i].getNum()) + ".txt";
        std::remove(filename.c_str());
    }

    // Main simulation loop
    while (currentTime <= config.TimeLim) {

        // This loop iterates over each UAV and writes its details to a separate file.
        for (int i = 0; i < config.N_uav; ++i) {
            // Formulate the filename for each UAV
            std::string filename = "UAV" + std::to_string(uavs[i].getNum()) + ".txt";
            // Write details to the file
            writeUAVDetailsToFile(filename, currentTime, uavs[i].getX(), uavs[i].getY(), uavs[i].getAzimuth());
        }

        // Increment current time by time step
        currentTime += config.Dt;

        // Iterate through each UAV
        for (int i = 0; i < config.N_uav; ++i) {
            double latestCommandTime = 0.0; // Initialize latest command time for the UAV

            // Iterate through the list of commands for the current UAV
            for (const auto& command : commands) {
                // Check if the command is for the current UAV and its time is valid and not outdated
                if (command.num == i + 1 && command.time < currentTime && command.time > latestCommandTime) {
                    latestCommandTime= command.time;// Update the latest command time for the UAV
                    if (uavCommands[i] != command.time) {
                        uavs[i].setStandbyModeFlag(false);
                    }
                    uavCommands[i] = command.time; // Update the latest command time for the UAV
                    uavs[i].navigateToTarget(config.Dt, command.x, command.y); // Execute navigation command
                }
            }

            // If no command was executed for the UAV, perform linear flight update
            if (-1== uavCommands[i]) {
                uavs[i].linearFlightUpdate(config.Dt);
            }
        }
    }

    //deallocate memory
    delete[] uavs;
    uavs = nullptr;

    return 0;
}