#include "cim17/CimModel.hpp"
#include "cim17/CimEquipment.hpp"
#include <spdlog/spdlog.h>
#include <typeinfo>
#include <stdexcept>

namespace cim17 {

CimModel::CimModel() = default;
CimModel::~CimModel() = default;

void CimModel::clear() {
    objects_.clear();
    typeIndex_.clear();
    modelId_ = "";
    modelDescription_ = "";
    modelVersion_ = "";
    modelingAuthoritySet_ = "";
}

bool CimModel::hasObject(const std::string& id) const {
    return objects_.find(id) != objects_.end();
}

size_t CimModel::getObjectCount() const {
    return objects_.size();
}

void CimModel::setModelId(const std::string& modelId) {
    modelId_ = modelId;
}

std::string CimModel::getModelId() const {
    return modelId_;
}

void CimModel::setModelDescription(const std::string& description) {
    modelDescription_ = description;
}

std::string CimModel::getModelDescription() const {
    return modelDescription_;
}

void CimModel::setModelVersion(const std::string& version) {
    modelVersion_ = version;
}

std::string CimModel::getModelVersion() const {
    return modelVersion_;
}

void CimModel::setModelingAuthoritySet(const std::string& modelingAuthoritySet) {
    modelingAuthoritySet_ = modelingAuthoritySet;
}

std::string CimModel::getModelingAuthoritySet() const {
    return modelingAuthoritySet_;
}

bool CimModel::resolveReferences() {
    bool success = true;
    
    success &= resolveBaseReferences();
    success &= resolveEquipmentReferences();
    
    return success;
}

bool CimModel::resolveBaseReferences() {
    // This function would resolve basic references like Terminal-Equipment connections
    // In a real implementation, more detailed reference resolution would be required
    
    return true;
}

bool CimModel::resolveEquipmentReferences() {
    // This function would resolve equipment-specific references
    // For example connecting transformers to their ends, lines to their terminals, etc.
    
    return true;
}

// Template implementations
template<typename T>
bool CimModel::addObject(std::shared_ptr<T> object) {
    static_assert(std::is_base_of<CimIdentifiedObject, T>::value, 
                  "T must inherit from CimIdentifiedObject");
    
    if (!object) {
        spdlog::error("Attempted to add null object to model");
        return false;
    }
    
    std::string id = object->getMRID();
    
    // Check if an object with this ID already exists
    if (hasObject(id)) {
        spdlog::warn("Object with ID {} already exists in model", id);
        return false;
    }
    
    // Add to objects map
    objects_[id] = object;
    
    // Add to type index
    std::type_index typeIdx = std::type_index(typeid(T));
    typeIndex_[typeIdx].push_back(id);
    
    spdlog::debug("Added object of type {} with ID {}", object->getTypeName(), id);
    
    return true;
}

template<typename T>
std::shared_ptr<T> CimModel::getObject(const std::string& id) const {
    auto it = objects_.find(id);
    if (it != objects_.end()) {
        // Try to cast to the requested type
        std::shared_ptr<T> typedObj = std::dynamic_pointer_cast<T>(it->second);
        if (typedObj) {
            return typedObj;
        }
    }
    
    return nullptr;
}

template<typename T>
std::vector<std::shared_ptr<T>> CimModel::getObjectsByType() const {
    std::vector<std::shared_ptr<T>> result;
    
    // Find the type index
    std::type_index typeIdx = std::type_index(typeid(T));
    auto it = typeIndex_.find(typeIdx);
    
    // If not found, return empty vector
    if (it == typeIndex_.end()) {
        return result;
    }
    
    // Reserve space for efficiency
    result.reserve(it->second.size());
    
    // Get all objects of this type
    for (const std::string& id : it->second) {
        auto objIt = objects_.find(id);
        if (objIt != objects_.end()) {
            std::shared_ptr<T> typedObj = std::dynamic_pointer_cast<T>(objIt->second);
            if (typedObj) {
                result.push_back(typedObj);
            }
        }
    }
    
    return result;
}

template<typename T>
size_t CimModel::getTypeCount() const {
    std::type_index typeIdx = std::type_index(typeid(T));
    auto it = typeIndex_.find(typeIdx);
    
    if (it != typeIndex_.end()) {
        return it->second.size();
    }
    
    return 0;
}

// Explicit template instantiations for common types
template bool CimModel::addObject<CimIdentifiedObject>(std::shared_ptr<CimIdentifiedObject>);
template bool CimModel::addObject<CimEquipment>(std::shared_ptr<CimEquipment>);
template bool CimModel::addObject<Terminal>(std::shared_ptr<Terminal>);
template bool CimModel::addObject<ACLineSegment>(std::shared_ptr<ACLineSegment>);
template bool CimModel::addObject<PowerTransformer>(std::shared_ptr<PowerTransformer>);
template bool CimModel::addObject<SynchronousMachine>(std::shared_ptr<SynchronousMachine>);
template bool CimModel::addObject<SolarGeneratingUnit>(std::shared_ptr<SolarGeneratingUnit>);
template bool CimModel::addObject<PowerElectronicsConnection>(std::shared_ptr<PowerElectronicsConnection>);

template std::shared_ptr<CimIdentifiedObject> CimModel::getObject<CimIdentifiedObject>(const std::string&) const;
template std::shared_ptr<CimEquipment> CimModel::getObject<CimEquipment>(const std::string&) const;
template std::shared_ptr<Terminal> CimModel::getObject<Terminal>(const std::string&) const;
template std::shared_ptr<ACLineSegment> CimModel::getObject<ACLineSegment>(const std::string&) const;
template std::shared_ptr<PowerTransformer> CimModel::getObject<PowerTransformer>(const std::string&) const;
template std::shared_ptr<SynchronousMachine> CimModel::getObject<SynchronousMachine>(const std::string&) const;
template std::shared_ptr<SolarGeneratingUnit> CimModel::getObject<SolarGeneratingUnit>(const std::string&) const;
template std::shared_ptr<PowerElectronicsConnection> CimModel::getObject<PowerElectronicsConnection>(const std::string&) const;

template std::vector<std::shared_ptr<CimIdentifiedObject>> CimModel::getObjectsByType<CimIdentifiedObject>() const;
template std::vector<std::shared_ptr<CimEquipment>> CimModel::getObjectsByType<CimEquipment>() const;
template std::vector<std::shared_ptr<Terminal>> CimModel::getObjectsByType<Terminal>() const;
template std::vector<std::shared_ptr<ACLineSegment>> CimModel::getObjectsByType<ACLineSegment>() const;
template std::vector<std::shared_ptr<PowerTransformer>> CimModel::getObjectsByType<PowerTransformer>() const;
template std::vector<std::shared_ptr<SynchronousMachine>> CimModel::getObjectsByType<SynchronousMachine>() const;
template std::vector<std::shared_ptr<SolarGeneratingUnit>> CimModel::getObjectsByType<SolarGeneratingUnit>() const;
template std::vector<std::shared_ptr<PowerElectronicsConnection>> CimModel::getObjectsByType<PowerElectronicsConnection>() const;

template size_t CimModel::getTypeCount<CimIdentifiedObject>() const;
template size_t CimModel::getTypeCount<CimEquipment>() const;
template size_t CimModel::getTypeCount<Terminal>() const;
template size_t CimModel::getTypeCount<ACLineSegment>() const;
template size_t CimModel::getTypeCount<PowerTransformer>() const;
template size_t CimModel::getTypeCount<SynchronousMachine>() const;
template size_t CimModel::getTypeCount<SolarGeneratingUnit>() const;
template size_t CimModel::getTypeCount<PowerElectronicsConnection>() const;

} // namespace cim17
