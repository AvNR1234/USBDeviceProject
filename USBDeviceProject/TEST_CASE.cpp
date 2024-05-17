#include <iostream>
#include <fstream>
#include <cstdio> // For popen
#include "catch.hpp" // Include your testing framework, e.g., Catch2

std::string exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
            result += buffer;
        }
    }
    catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

TEST_CASE("Test Data Reading") {
    // Generate sample data file
    std::ofstream outfile("test_data.txt");
    outfile << "1 2 3 4 5";
    outfile.close();

    // Run your application
    std::string command = "./build/your_application_executable"; // Update with your executable name
    std::string output = exec(command.c_str());

    // Verify output
    REQUIRE(output == "Received data from USB: 1\nReceived data from USB: 2\nReceived data from USB: 3\nReceived data from USB: 4\nReceived data from USB: 5\n");
}