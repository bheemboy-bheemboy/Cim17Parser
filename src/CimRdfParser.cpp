#include "cim17/CimRdfParser.hpp"
#include "cim17/CimModel.hpp"
#include "cim17/CimEquipment.hpp"
#include "cim17/XmlHandler.hpp"
#include <pugixml.hpp>
#include <spdlog/spdlog.h>
#include <string>
#include <charconv>

namespace cim17 {

CimRdfParser::CimRdfParser(CimModel& model)
    : model_(model), lastError_("") {
    
    // Initialize factory methods for creating CIM objects
    initializeFactories();
}

CimRdfParser::~CimRdfParser() = default;

bool CimRdfParser::parse(const XmlHandler& xmlHandler) {
    pugi::xml_node rootNode = xmlHandler.getRootNode();
    if (!rootNode) {
        lastError_ = "Empty XML document";
        spdlog::error(lastError_);
        return false;
    }
    
    spdlog::info("Starting to parse CIM/RDF document");
    
    // Clear model before parsing
    model_.clear();
    
    // Find FullModel element
    pugi::xml_node fullModelNode = rootNode.find_child([](const pugi::xml_node& node) {
        std::string name = node.name();
        return name == "FullModel" || name == "md:FullModel";
    });
    
    if (fullModelNode) {
        // Parse model metadata
        if (!parseModelHeader(fullModelNode)) {
            return false;
        }
    }
    
    // Process all rdf:Description nodes
    bool success = true;
    for (pugi::xml_node descNode : rootNode.children("rdf:Description")) {
        success &= processRdfDescription(descNode);
    }
    
    // Extract references between objects
    success &= extractReferences(rootNode);
    
    // Resolve references
    if (success) {
        success &= model_.resolveReferences();
    }
    
    if (success) {
        spdlog::info("Successfully parsed CIM/RDF document, created {} objects", model_.getObjectCount());
    } else {
        spdlog::error("Failed to parse CIM/RDF document: {}", lastError_);
    }
    
    return success;
}

std::string CimRdfParser::getLastError() const {
    return lastError_;
}

bool CimRdfParser::parseModelHeader(const pugi::xml_node& fullModelNode) {
    // Extract model ID from rdf:about or rdf:ID attribute
    std::string modelId = extractMRID(fullModelNode);
    if (modelId.empty()) {
        lastError_ = "Missing model ID (rdf:about or rdf:ID) in FullModel element";
        spdlog::error(lastError_);
        return false;
    }
    
    model_.setModelId(modelId);
    
    // Extract model description
    std::string description = getChildElementText(fullModelNode, "Model.description");
    if (!description.empty()) {
        model_.setModelDescription(description);
    } else {
        // Try with namespace prefix
        description = getChildElementText(fullModelNode, "md:Model.description");
        if (!description.empty()) {
            model_.setModelDescription(description);
        }
    }
    
    // Extract model version
    std::string version = getChildElementText(fullModelNode, "Model.version");
    if (!version.empty()) {
        model_.setModelVersion(version);
    } else {
        // Try with namespace prefix
        version = getChildElementText(fullModelNode, "md:Model.version");
        if (!version.empty()) {
            model_.setModelVersion(version);
        }
    }
    
    // Extract modeling authority set
    std::string modelingAuthoritySet = getChildElementText(fullModelNode, "Model.modelingAuthoritySet");
    if (!modelingAuthoritySet.empty()) {
        model_.setModelingAuthoritySet(modelingAuthoritySet);
    } else {
        // Try with namespace prefix
        modelingAuthoritySet = getChildElementText(fullModelNode, "md:Model.modelingAuthoritySet");
        if (!modelingAuthoritySet.empty()) {
            model_.setModelingAuthoritySet(modelingAuthoritySet);
        }
    }
    
    return true;
}

bool CimRdfParser::processRdfDescription(const pugi::xml_node& node) {
    // Get the rdf:ID or rdf:about attribute
    std::string rdfId = extractMRID(node);
    if (rdfId.empty()) {
        spdlog::warn("RDF Description without ID or about attribute, skipping");
        return true; // Not a fatal error
    }
    
    // Determine the RDF type (class)
    std::string rdfType;
    pugi::xml_node typeNode = node.child("rdf:type");
    if (typeNode) {
        pugi::xml_attribute resourceAttr = typeNode.attribute("rdf:resource");
        if (resourceAttr) {
            rdfType = resourceAttr.value();
        }
    }
    
    if (rdfType.empty()) {
        spdlog::warn("RDF Description with ID {} has no type, skipping", rdfId);
        return true; // Not a fatal error
    }
    
    // Process based on type
    if (rdfType.find("Terminal") != std::string::npos) {
        return processTerminal(node, rdfId);
    } else if (rdfType.find("ACLineSegment") != std::string::npos) {
        return processACLineSegment(node, rdfId);
    } else if (rdfType.find("PowerTransformer") != std::string::npos) {
        return processPowerTransformer(node, rdfId);
    } else if (rdfType.find("SynchronousMachine") != std::string::npos) {
        return processSynchronousMachine(node, rdfId);
    } else if (rdfType.find("SolarGeneratingUnit") != std::string::npos) {
        return processSolarGeneratingUnit(node, rdfId);
    } else if (rdfType.find("PowerElectronicsConnection") != std::string::npos) {
        return processPowerElectronicsConnection(node, rdfId);
    } else {
        // Generic equipment or unknown type
        return processEquipment(node, rdfId, rdfType);
    }
}

bool CimRdfParser::processEquipment(const pugi::xml_node& node, const std::string& rdfId, const std::string& rdfType) {
    // Check for factory method for this type
    auto it = factories_.find(rdfType);
    if (it == factories_.end()) {
        spdlog::warn("No factory for RDF type {}, skipping", rdfType);
        return true; // Not a fatal error
    }
    
    // Create object using factory method
    std::shared_ptr<CimIdentifiedObject> object = it->second(rdfId);
    if (!object) {
        spdlog::error("Failed to create object of type {}", rdfType);
        return false;
    }
    
    // Set name if present
    std::string name = getChildElementText(node, "IdentifiedObject.name");
    if (!name.empty()) {
        object->setName(name);
    }
    
    // Set description if present
    std::string description = getChildElementText(node, "IdentifiedObject.description");
    if (!description.empty()) {
        object->setDescription(description);
    }
    
    // Set alias name if present
    std::string aliasName = getChildElementText(node, "IdentifiedObject.aliasName");
    if (!aliasName.empty()) {
        object->setAliasName(aliasName);
    }
    
    // For equipment, check if it's in service
    std::shared_ptr<CimEquipment> equipment = std::dynamic_pointer_cast<CimEquipment>(object);
    if (equipment) {
        std::string inServiceStr = getChildElementText(node, "Equipment.inService");
        if (!inServiceStr.empty()) {
            equipment->setInService(stringToBool(inServiceStr, true));
        }
        
        std::string normallyInServiceStr = getChildElementText(node, "Equipment.normallyInService");
        if (!normallyInServiceStr.empty()) {
            equipment->setNormallyInService(stringToBool(normallyInServiceStr, true));
        }
        
        std::string aggregateStr = getChildElementText(node, "Equipment.aggregate");
        if (!aggregateStr.empty()) {
            equipment->setAggregate(stringToBool(aggregateStr, false));
        }
    }
    
    // Add to model
    return model_.addObject(object);
}

bool CimRdfParser::processTerminal(const pugi::xml_node& node, const std::string& rdfId) {
    // Create terminal object
    auto terminal = std::make_shared<Terminal>(rdfId);
    
    // Set name if present
    std::string name = getChildElementText(node, "IdentifiedObject.name");
    if (!name.empty()) {
        terminal->setName(name);
    }
    
    // Set description if present
    std::string description = getChildElementText(node, "IdentifiedObject.description");
    if (!description.empty()) {
        terminal->setDescription(description);
    }
    
    // Note: Connection to equipment will be done in reference resolution
    
    // Add to model
    return model_.addObject(terminal);
}

bool CimRdfParser::processACLineSegment(const pugi::xml_node& node, const std::string& rdfId) {
    // Create line segment object
    auto line = std::make_shared<ACLineSegment>(rdfId);
    
    // Set name if present
    std::string name = getChildElementText(node, "IdentifiedObject.name");
    if (!name.empty()) {
        line->setName(name);
    }
    
    // Set description if present
    std::string description = getChildElementText(node, "IdentifiedObject.description");
    if (!description.empty()) {
        line->setDescription(description);
    }
    
    // Set line parameters
    std::string rStr = getChildElementText(node, "ACLineSegment.r");
    if (!rStr.empty()) {
        line->setR(stringToDouble(rStr, 0.0));
    }
    
    std::string xStr = getChildElementText(node, "ACLineSegment.x");
    if (!xStr.empty()) {
        line->setX(stringToDouble(xStr, 0.0));
    }
    
    std::string bchStr = getChildElementText(node, "ACLineSegment.bch");
    if (!bchStr.empty()) {
        line->setBch(stringToDouble(bchStr, 0.0));
    }
    
    std::string lengthStr = getChildElementText(node, "Conductor.length");
    if (!lengthStr.empty()) {
        line->setLength(stringToDouble(lengthStr, 0.0));
    }
    
    // Check if in service
    std::string inServiceStr = getChildElementText(node, "Equipment.inService");
    if (!inServiceStr.empty()) {
        line->setInService(stringToBool(inServiceStr, true));
    }
    
    // Add to model
    return model_.addObject(line);
}

bool CimRdfParser::processPowerTransformer(const pugi::xml_node& node, const std::string& rdfId) {
    // Create transformer object
    auto transformer = std::make_shared<PowerTransformer>(rdfId);
    
    // Set name if present
    std::string name = getChildElementText(node, "IdentifiedObject.name");
    if (!name.empty()) {
        transformer->setName(name);
    }
    
    // Set description if present
    std::string description = getChildElementText(node, "IdentifiedObject.description");
    if (!description.empty()) {
        transformer->setDescription(description);
    }
    
    // Set transformer type
    std::string type = getChildElementText(node, "PowerTransformer.transformerType");
    if (!type.empty()) {
        transformer->setTransformerType(type);
    }
    
    // Check if in service
    std::string inServiceStr = getChildElementText(node, "Equipment.inService");
    if (!inServiceStr.empty()) {
        transformer->setInService(stringToBool(inServiceStr, true));
    }
    
    // Add to model
    return model_.addObject(transformer);
}

bool CimRdfParser::processSynchronousMachine(const pugi::xml_node& node, const std::string& rdfId) {
    // Create synchronous machine object
    auto machine = std::make_shared<SynchronousMachine>(rdfId);
    
    // Set name if present
    std::string name = getChildElementText(node, "IdentifiedObject.name");
    if (!name.empty()) {
        machine->setName(name);
    }
    
    // Set description if present
    std::string description = getChildElementText(node, "IdentifiedObject.description");
    if (!description.empty()) {
        machine->setDescription(description);
    }
    
    // Set rated apparent power
    std::string ratedSStr = getChildElementText(node, "SynchronousMachine.ratedS");
    if (!ratedSStr.empty()) {
        machine->setRatedS(stringToDouble(ratedSStr, 0.0));
    }
    
    // Set rated voltage
    std::string ratedUStr = getChildElementText(node, "SynchronousMachine.ratedU");
    if (!ratedUStr.empty()) {
        machine->setRatedU(stringToDouble(ratedUStr, 0.0));
    }
    
    // Set machine type
    std::string type = getChildElementText(node, "SynchronousMachine.type");
    if (!type.empty()) {
        machine->setType(type);
    }
    
    // Check if in service
    std::string inServiceStr = getChildElementText(node, "Equipment.inService");
    if (!inServiceStr.empty()) {
        machine->setInService(stringToBool(inServiceStr, true));
    }
    
    // Add to model
    return model_.addObject(machine);
}

bool CimRdfParser::processSolarGeneratingUnit(const pugi::xml_node& node, const std::string& rdfId) {
    // Create solar generating unit object
    auto solarUnit = std::make_shared<SolarGeneratingUnit>(rdfId);
    
    // Set name if present
    std::string name = getChildElementText(node, "IdentifiedObject.name");
    if (!name.empty()) {
        solarUnit->setName(name);
    }
    
    // Set description if present
    std::string description = getChildElementText(node, "IdentifiedObject.description");
    if (!description.empty()) {
        solarUnit->setDescription(description);
    }
    
    // Set maximum capacity
    std::string maxCapacityStr = getChildElementText(node, "SolarGeneratingUnit.maxCapacity");
    if (!maxCapacityStr.empty()) {
        solarUnit->setMaxCapacity(stringToDouble(maxCapacityStr, 0.0));
    }
    
    // Check if in service
    std::string inServiceStr = getChildElementText(node, "Equipment.inService");
    if (!inServiceStr.empty()) {
        solarUnit->setInService(stringToBool(inServiceStr, true));
    }
    
    // Add to model
    return model_.addObject(solarUnit);
}

bool CimRdfParser::processPowerElectronicsConnection(const pugi::xml_node& node, const std::string& rdfId) {
    // Create power electronics connection object
    auto peConnection = std::make_shared<PowerElectronicsConnection>(rdfId);
    
    // Set name if present
    std::string name = getChildElementText(node, "IdentifiedObject.name");
    if (!name.empty()) {
        peConnection->setName(name);
    }
    
    // Set description if present
    std::string description = getChildElementText(node, "IdentifiedObject.description");
    if (!description.empty()) {
        peConnection->setDescription(description);
    }
    
    // Set maximum active power
    std::string maxPStr = getChildElementText(node, "PowerElectronicsConnection.maxP");
    if (!maxPStr.empty()) {
        peConnection->setMaxP(stringToDouble(maxPStr, 0.0));
    }
    
    // Set maximum reactive power
    std::string maxQStr = getChildElementText(node, "PowerElectronicsConnection.maxQ");
    if (!maxQStr.empty()) {
        peConnection->setMaxQ(stringToDouble(maxQStr, 0.0));
    }
    
    // Check if in service
    std::string inServiceStr = getChildElementText(node, "Equipment.inService");
    if (!inServiceStr.empty()) {
        peConnection->setInService(stringToBool(inServiceStr, true));
    }
    
    // Add to model
    return model_.addObject(peConnection);
}

bool CimRdfParser::extractReferences(const pugi::xml_node& rootNode) {
    // This would extract and set up references between objects
    // For example, connecting terminals to equipment
    // In a full implementation, this would be much more complex
    
    // Process all Terminal references
    for (pugi::xml_node descNode : rootNode.children("rdf:Description")) {
        // Check if node is a Terminal
        pugi::xml_node typeNode = descNode.child("rdf:type");
        if (!typeNode) {
            continue;
        }
        
        pugi::xml_attribute resourceAttr = typeNode.attribute("rdf:resource");
        if (!resourceAttr) {
            continue;
        }
        
        std::string rdfType = resourceAttr.value();
        if (rdfType.find("Terminal") == std::string::npos) {
            continue;
        }
        
        // Get Terminal ID
        std::string terminalId = extractMRID(descNode);
        if (terminalId.empty()) {
            continue;
        }
        
        // Find the Terminal object
        auto terminal = model_.getObject<Terminal>(terminalId);
        if (!terminal) {
            continue;
        }
        
        // Look for ConductingEquipment reference
        pugi::xml_node ceNode = descNode.child("Terminal.ConductingEquipment");
        if (!ceNode) {
            continue;
        }
        
        pugi::xml_attribute ceResourceAttr = ceNode.attribute("rdf:resource");
        if (!ceResourceAttr) {
            continue;
        }
        
        std::string equipmentId = ceResourceAttr.value();
        if (equipmentId.empty()) {
            continue;
        }
        
        // Remove any URI prefix (e.g., "#")
        if (equipmentId[0] == '#') {
            equipmentId = equipmentId.substr(1);
        }
        
        // Find the equipment object
        auto equipment = model_.getObject<CimEquipment>(equipmentId);
        if (!equipment) {
            continue;
        }
        
        // Set the reference
        terminal->setConnectedEquipment(equipment);
    }
    
    return true;
}

void CimRdfParser::initializeFactories() {
    // Register factory methods for creating different types of CIM objects
    factories_["http://iec.ch/TC57/2016/CIM-schema-cim17#Terminal"] = 
        [](const std::string& id) { return std::make_shared<Terminal>(id); };
    
    factories_["http://iec.ch/TC57/2016/CIM-schema-cim17#ACLineSegment"] = 
        [](const std::string& id) { return std::make_shared<ACLineSegment>(id); };
    
    factories_["http://iec.ch/TC57/2016/CIM-schema-cim17#PowerTransformer"] = 
        [](const std::string& id) { return std::make_shared<PowerTransformer>(id); };
    
    factories_["http://iec.ch/TC57/2016/CIM-schema-cim17#SynchronousMachine"] = 
        [](const std::string& id) { return std::make_shared<SynchronousMachine>(id); };
    
    factories_["http://iec.ch/TC57/2016/CIM-schema-cim17#SolarGeneratingUnit"] = 
        [](const std::string& id) { return std::make_shared<SolarGeneratingUnit>(id); };
    
    factories_["http://iec.ch/TC57/2016/CIM-schema-cim17#PowerElectronicsConnection"] = 
        [](const std::string& id) { return std::make_shared<PowerElectronicsConnection>(id); };
}

std::string CimRdfParser::extractMRID(const pugi::xml_node& node, const std::string& defaultId) {
    std::string id;
    
    // Check for rdf:ID attribute
    pugi::xml_attribute idAttr = node.attribute("rdf:ID");
    if (idAttr) {
        id = idAttr.value();
    } else {
        // Check for rdf:about attribute
        pugi::xml_attribute aboutAttr = node.attribute("rdf:about");
        if (aboutAttr) {
            id = aboutAttr.value();
            
            // Remove URI prefix (e.g., "#" or "urn:uuid:")
            if (id.find("#") == 0) {
                id = id.substr(1);
            } else if (id.find("urn:uuid:") == 0) {
                id = id.substr(9);
            }
        }
    }
    
    return id.empty() ? defaultId : id;
}

std::string CimRdfParser::getAttributeValue(const pugi::xml_node& node, const std::string& attrName) {
    pugi::xml_attribute attr = node.attribute(attrName.c_str());
    if (attr) {
        return attr.value();
    }
    return "";
}

std::string CimRdfParser::getChildElementText(const pugi::xml_node& node, const std::string& elementName) {
    pugi::xml_node childNode = node.child(elementName.c_str());
    if (childNode) {
        return childNode.text().as_string();
    }
    return "";
}

double CimRdfParser::stringToDouble(const std::string& value, double defaultValue) {
    try {
        return std::stod(value);
    } catch (const std::exception&) {
        return defaultValue;
    }
}

bool CimRdfParser::stringToBool(const std::string& value, bool defaultValue) {
    if (value == "true" || value == "1" || value == "yes") {
        return true;
    } else if (value == "false" || value == "0" || value == "no") {
        return false;
    }
    return defaultValue;
}

} // namespace cim17
