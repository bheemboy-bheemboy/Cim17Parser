#include "cim17/XmlHandler.hpp"
#include <pugixml.hpp>
#include <spdlog/spdlog.h>

namespace cim17 {

XmlHandler::XmlHandler(bool validateXml)
    : xmlDoc_(std::make_unique<pugi::xml_document>()),
      validateXml_(validateXml),
      lastError_("") {
}

XmlHandler::~XmlHandler() = default;

bool XmlHandler::loadFile(const std::string& filePath) {
    pugi::xml_parse_result result = xmlDoc_->load_file(filePath.c_str());
    
    if (!result) {
        lastError_ = std::string("XML parse error: ") + result.description() + 
                    " at offset " + std::to_string(result.offset);
        spdlog::error(lastError_);
        return false;
    }
    
    parseNamespaces();
    return true;
}

bool XmlHandler::loadContent(const std::string& content) {
    pugi::xml_parse_result result = xmlDoc_->load_string(content.c_str());
    
    if (!result) {
        lastError_ = std::string("XML parse error: ") + result.description() + 
                    " at offset " + std::to_string(result.offset);
        spdlog::error(lastError_);
        return false;
    }
    
    parseNamespaces();
    return true;
}

pugi::xml_node XmlHandler::getRootNode() const {
    return xmlDoc_->document_element();
}

std::string XmlHandler::getNamespaceUri(const std::string& prefix) const {
    auto it = namespaces_.find(prefix);
    if (it != namespaces_.end()) {
        return it->second;
    }
    return "";
}

std::unordered_map<std::string, std::string> XmlHandler::getNamespaces() const {
    return namespaces_;
}

std::vector<std::string> XmlHandler::getModelProfiles() const {
    std::vector<std::string> profiles;
    
    // Look for FullModel element
    pugi::xml_node fullModelNode = xmlDoc_->document_element().find_child_by_name("FullModel");
    if (!fullModelNode) {
        // Try with md:FullModel
        fullModelNode = xmlDoc_->document_element().find_child_by_name("md:FullModel");
    }
    
    if (fullModelNode) {
        // Look for Model.profile elements
        for (pugi::xml_node profileNode : fullModelNode.children("Model.profile")) {
            profiles.push_back(profileNode.text().as_string());
        }
        
        // Try with md:Model.profile
        for (pugi::xml_node profileNode : fullModelNode.children("md:Model.profile")) {
            profiles.push_back(profileNode.text().as_string());
        }
    }
    
    return profiles;
}

std::string XmlHandler::getLastError() const {
    return lastError_;
}

void XmlHandler::parseNamespaces() {
    namespaces_.clear();
    
    // Get root element
    pugi::xml_node root = xmlDoc_->document_element();
    if (!root) {
        return;
    }
    
    // Iterate over attributes
    for (pugi::xml_attribute attr : root.attributes()) {
        std::string name = attr.name();
        std::string value = attr.value();
        
        // Check if attribute is a namespace declaration
        if (name == "xmlns") {
            // Default namespace
            namespaces_[""] = value;
        } else if (name.find("xmlns:") == 0) {
            // Prefixed namespace
            std::string prefix = name.substr(6); // Remove "xmlns:"
            namespaces_[prefix] = value;
        }
    }
    
    spdlog::debug("Parsed {} namespaces", namespaces_.size());
}

} // namespace cim17
