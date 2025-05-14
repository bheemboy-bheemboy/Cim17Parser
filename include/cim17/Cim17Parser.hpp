#pragma once

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <optional>

#include "cim17/CimModel.hpp"
#include "cim17/XmlHandler.hpp"
#include "cim17/CgmesProfile.hpp"

namespace cim17 {

/**
 * @class Cim17Parser
 * @brief Main parser class for CGMES v3/CIM17 XML files
 * 
 * This class is responsible for parsing CGMES v3 XML files following the CIM17 model,
 * and creating appropriate objects from the parsed data.
 */
class Cim17Parser {
public:
    /**
     * @brief Default constructor
     */
    Cim17Parser();
    
    /**
     * @brief Destructor
     */
    ~Cim17Parser();

    /**
     * @brief Parse a CGMES/CIM17 XML file
     * 
     * @param filePath Path to the XML file
     * @return True if parsing was successful, false otherwise
     */
    bool parseFile(const std::string& filePath);
    
    /**
     * @brief Parse CGMES/CIM17 XML content from a string
     * 
     * @param xmlContent XML content as string
     * @return True if parsing was successful, false otherwise
     */
    bool parseContent(const std::string& xmlContent);
    
    /**
     * @brief Get the CIM model created from parsing
     * 
     * @return Reference to the CIM model
     */
    CimModel& getModel();
    
    /**
     * @brief Get the CGMES profile of the parsed file
     * 
     * @return Profile type
     */
    CgmesProfile getProfile() const;
    
    /**
     * @brief Set validation mode for XML parsing
     * 
     * @param enableValidation If true, XML validation will be performed
     */
    void setValidationMode(bool enableValidation);
    
    /**
     * @brief Get the last error message if parsing failed
     * 
     * @return Error message
     */
    std::string getLastError() const;

private:
    // XML handler
    std::unique_ptr<XmlHandler> xmlHandler_;
    
    // CIM model to store parsed objects
    CimModel model_;
    
    // Current CGMES profile
    CgmesProfile profile_;
    
    // Validation mode flag
    bool validationEnabled_;
    
    // Last error message
    std::string lastError_;
    
    // Initialize parser
    void initialize();
    
    // Process XML document to CIM model
    bool processCimDocument();
    
    // Determine CGMES profile from the document
    CgmesProfile determineProfile();
};

} // namespace cim17
