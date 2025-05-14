#include "cim17/Cim17Parser.hpp"
#include <spdlog/spdlog.h>
#include <iostream>
#include <fstream>
#include <string>

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " <cgmes_file.xml>" << std::endl;
    std::cout << "Parse a CGMES v3 / CIM17 XML file and display basic information about the model." << std::endl;
}

void displayModelInfo(const cim17::CimModel& model) {
    std::cout << "\nModel Information:" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Model ID: " << model.getModelId() << std::endl;
    std::cout << "Description: " << model.getModelDescription() << std::endl;
    std::cout << "Version: " << model.getModelVersion() << std::endl;
    std::cout << "Modeling Authority Set: " << model.getModelingAuthoritySet() << std::endl;
    std::cout << "Total Objects: " << model.getObjectCount() << std::endl;
    
    // Display counts by type
    std::cout << "\nObject Counts by Type:" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Terminals: " << model.getTypeCount<cim17::Terminal>() << std::endl;
    std::cout << "AC Line Segments: " << model.getTypeCount<cim17::ACLineSegment>() << std::endl;
    std::cout << "Power Transformers: " << model.getTypeCount<cim17::PowerTransformer>() << std::endl;
    std::cout << "Synchronous Machines: " << model.getTypeCount<cim17::SynchronousMachine>() << std::endl;
    std::cout << "Solar Generating Units: " << model.getTypeCount<cim17::SolarGeneratingUnit>() << std::endl;
    std::cout << "Power Electronics Connections: " << model.getTypeCount<cim17::PowerElectronicsConnection>() << std::endl;
}

int main(int argc, char* argv[]) {
    // Configure logging
    spdlog::set_level(spdlog::level::info);
    
    // Check arguments
    if (argc != 2) {
        printUsage(argv[0]);
        return 1;
    }
    
    std::string filename = argv[1];
    
    // Create parser
    cim17::Cim17Parser parser;
    
    // Parse file
    std::cout << "Parsing file: " << filename << std::endl;
    bool success = parser.parseFile(filename);
    
    if (!success) {
        std::cerr << "Error parsing file: " << parser.getLastError() << std::endl;
        return 1;
    }
    
    // Get model and profile
    cim17::CimModel& model = parser.getModel();
    cim17::CgmesProfile profile = parser.getProfile();
    
    std::cout << "Successfully parsed CGMES file with profile: " 
              << cim17::CgmesProfileInfo::toString(profile) << std::endl;
    
    // Display model information
    displayModelInfo(model);
    
    return 0;
}
