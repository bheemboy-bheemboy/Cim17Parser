#pragma once

#include <memory>
#include <string>
#include <map>
#include <vector>
#include <functional>

// Forward declaration of pugixml classes
namespace pugi {
    class xml_node;
}

namespace cim17 {

// Forward declarations
class CimModel;
class CimIdentifiedObject;
class XmlHandler;

/**
 * @class CimRdfParser
 * @brief Parser for CIM/RDF XML data
 * 
 * This class is responsible for parsing CIM/RDF XML documents and
 * creating the corresponding CIM objects.
 */
class CimRdfParser {
public:
    /**
     * @brief Constructor
     * 
     * @param model CIM model to populate
     */
    explicit CimRdfParser(CimModel& model);
    
    /**
     * @brief Destructor
     */
    ~CimRdfParser();
    
    /**
     * @brief Parse the XML document
     * 
     * @param xmlHandler XML handler with loaded document
     * @return True if parsing was successful, false otherwise
     */
    bool parse(const XmlHandler& xmlHandler);
    
    /**
     * @brief Get the last error message
     * 
     * @return Error message
     */
    std::string getLastError() const;

private:
    // Reference to the CIM model
    CimModel& model_;
    
    // Last error message
    std::string lastError_;
    
    // Map of RDF type to factory function
    using ObjectFactory = std::function<std::shared_ptr<CimIdentifiedObject>(const std::string&)>;
    std::map<std::string, ObjectFactory> factories_;
    
    // Parse model metadata
    bool parseModelHeader(const pugi::xml_node& fullModelNode);
    
    // Process RDF description node
    bool processRdfDescription(const pugi::xml_node& node);
    
    // Process equipment class
    bool processEquipment(const pugi::xml_node& node, const std::string& rdfId, const std::string& rdfType);
    
    // Process terminal class
    bool processTerminal(const pugi::xml_node& node, const std::string& rdfId);
    
    // Process line segment class
    bool processACLineSegment(const pugi::xml_node& node, const std::string& rdfId);
    
    // Process transformer class
    bool processPowerTransformer(const pugi::xml_node& node, const std::string& rdfId);
    
    // Process synchronous machine class
    bool processSynchronousMachine(const pugi::xml_node& node, const std::string& rdfId);
    
    // Process solar generating unit class
    bool processSolarGeneratingUnit(const pugi::xml_node& node, const std::string& rdfId);
    
    // Process power electronics connection class
    bool processPowerElectronicsConnection(const pugi::xml_node& node, const std::string& rdfId);
    
    // Extract references
    bool extractReferences(const pugi::xml_node& node);
    
    // Initialize factory methods
    void initializeFactories();
    
    // Helper to extract MRID from rdf:ID or rdf:about
    std::string extractMRID(const pugi::xml_node& node, const std::string& defaultId = "");
    
    // Helper to extract attribute value
    std::string getAttributeValue(const pugi::xml_node& node, const std::string& attrName);
    
    // Helper to extract text content of a child element
    std::string getChildElementText(const pugi::xml_node& node, const std::string& elementName);
    
    // Helper to convert a string to double
    double stringToDouble(const std::string& value, double defaultValue = 0.0);
    
    // Helper to convert a string to bool
    bool stringToBool(const std::string& value, bool defaultValue = false);
};

} // namespace cim17
