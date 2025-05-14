#include "cim17/Cim17Parser.hpp"
#include "cim17/CimRdfParser.hpp"
#include <spdlog/spdlog.h>

namespace cim17 {

Cim17Parser::Cim17Parser()
    : xmlHandler_(std::make_unique<XmlHandler>()),
      profile_(CgmesProfile::UNKNOWN),
      validationEnabled_(false),
      lastError_("") {
    
    initialize();
}

Cim17Parser::~Cim17Parser() = default;

bool Cim17Parser::parseFile(const std::string& filePath) {
    if (!xmlHandler_->loadFile(filePath)) {
        lastError_ = "Failed to load XML file: " + xmlHandler_->getLastError();
        spdlog::error(lastError_);
        return false;
    }
    
    profile_ = determineProfile();
    return processCimDocument();
}

bool Cim17Parser::parseContent(const std::string& xmlContent) {
    if (!xmlHandler_->loadContent(xmlContent)) {
        lastError_ = "Failed to parse XML content: " + xmlHandler_->getLastError();
        spdlog::error(lastError_);
        return false;
    }
    
    profile_ = determineProfile();
    return processCimDocument();
}

CimModel& Cim17Parser::getModel() {
    return model_;
}

CgmesProfile Cim17Parser::getProfile() const {
    return profile_;
}

void Cim17Parser::setValidationMode(bool enableValidation) {
    validationEnabled_ = enableValidation;
    xmlHandler_ = std::make_unique<XmlHandler>(validationEnabled_);
}

std::string Cim17Parser::getLastError() const {
    return lastError_;
}

void Cim17Parser::initialize() {
    spdlog::info("Initializing Cim17Parser");
}

bool Cim17Parser::processCimDocument() {
    // Create RDF parser
    CimRdfParser rdfParser(model_);
    
    // Process the document
    if (!rdfParser.parse(*xmlHandler_)) {
        lastError_ = "Failed to parse CIM/RDF document: " + rdfParser.getLastError();
        spdlog::error(lastError_);
        return false;
    }
    
    return true;
}

CgmesProfile Cim17Parser::determineProfile() {
    // Get model profiles
    std::vector<std::string> profileUris = xmlHandler_->getModelProfiles();
    
    // Determine profile based on URIs
    for (const std::string& uri : profileUris) {
        CgmesProfile detectedProfile = CgmesProfileInfo::detectProfile(uri);
        if (detectedProfile != CgmesProfile::UNKNOWN) {
            spdlog::info("Detected CGMES profile: {}", CgmesProfileInfo::toString(detectedProfile));
            return detectedProfile;
        }
    }
    
    spdlog::warn("Could not determine CGMES profile, using UNKNOWN");
    return CgmesProfile::UNKNOWN;
}

} // namespace cim17
