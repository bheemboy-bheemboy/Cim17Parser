#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

// Forward declaration of pugixml parser class
namespace pugi {
    class xml_document;
    class xml_node;
}

namespace cim17 {

/**
 * @class XmlHandler
 * @brief Handler class for XML processing using pugixml
 */
class XmlHandler {
public:
    /**
     * @brief Constructor
     * 
     * @param validateXml Whether to validate the XML against schema
     */
    explicit XmlHandler(bool validateXml = false);
    
    /**
     * @brief Destructor
     */
    ~XmlHandler();
    
    /**
     * @brief Load XML from file
     * 
     * @param filePath Path to the XML file
     * @return True if loading was successful, false otherwise
     */
    bool loadFile(const std::string& filePath);
    
    /**
     * @brief Load XML from string
     * 
     * @param content XML content as string
     * @return True if loading was successful, false otherwise
     */
    bool loadContent(const std::string& content);
    
    /**
     * @brief Get the root element of the XML document
     * 
     * @return Root node of the XML document
     */
    pugi::xml_node getRootNode() const;
    
    /**
     * @brief Get namespace URI for a prefix
     * 
     * @param prefix Namespace prefix
     * @return Namespace URI or empty string if not found
     */
    std::string getNamespaceUri(const std::string& prefix) const;
    
    /**
     * @brief Get all namespaces in the document
     * 
     * @return Map of namespace prefixes to URIs
     */
    std::unordered_map<std::string, std::string> getNamespaces() const;
    
    /**
     * @brief Get all model profiles defined in the document
     * 
     * @return Vector of profile URIs
     */
    std::vector<std::string> getModelProfiles() const;
    
    /**
     * @brief Get the last error message
     * 
     * @return Error message
     */
    std::string getLastError() const;

private:
    // XML document
    std::unique_ptr<pugi::xml_document> xmlDoc_;
    
    // Validation flag
    bool validateXml_;
    
    // Last error message
    std::string lastError_;
    
    // Map of namespace prefixes to URIs
    std::unordered_map<std::string, std::string> namespaces_;
    
    // Parse and store namespaces
    void parseNamespaces();
};

} // namespace cim17
