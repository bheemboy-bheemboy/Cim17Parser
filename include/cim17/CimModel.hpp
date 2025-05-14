#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <functional>

namespace cim17 {

// Forward declarations
class CimEquipment;
class CimIdentifiedObject;

/**
 * @class CimModel
 * @brief Class representing a CIM model that contains all parsed CIM objects
 */
class CimModel {
public:
    /**
     * @brief Constructor
     */
    CimModel();
    
    /**
     * @brief Destructor
     */
    ~CimModel();
    
    /**
     * @brief Clear all data in the model
     */
    void clear();
    
    /**
     * @brief Add an object to the model
     * 
     * @tparam T Type of the object (must inherit from CimIdentifiedObject)
     * @param object Object to add
     * @return True if the object was added successfully, false if an object with the same ID already exists
     */
    template<typename T>
    bool addObject(std::shared_ptr<T> object);
    
    /**
     * @brief Get an object by ID
     * 
     * @tparam T Type of the object to retrieve
     * @param id Object ID (mRID)
     * @return Shared pointer to the object, or nullptr if not found
     */
    template<typename T>
    std::shared_ptr<T> getObject(const std::string& id) const;
    
    /**
     * @brief Get all objects of a certain type
     * 
     * @tparam T Type of objects to retrieve
     * @return Vector of shared pointers to objects of type T
     */
    template<typename T>
    std::vector<std::shared_ptr<T>> getObjectsByType() const;
    
    /**
     * @brief Check if an object with the given ID exists
     * 
     * @param id Object ID (mRID)
     * @return True if an object with this ID exists
     */
    bool hasObject(const std::string& id) const;
    
    /**
     * @brief Get the number of objects in the model
     * 
     * @return Total number of objects
     */
    size_t getObjectCount() const;
    
    /**
     * @brief Get the number of objects of a specific type
     * 
     * @tparam T Type to count
     * @return Number of objects of type T
     */
    template<typename T>
    size_t getTypeCount() const;
    
    /**
     * @brief Resolve references between objects
     * 
     * This method resolves references between objects in the model,
     * e.g., connecting terminals to equipment, buses to lines, etc.
     * 
     * @return True if all references were resolved successfully
     */
    bool resolveReferences();
    
    /**
     * @brief Set the model ID (FullModel.rdf:about)
     * 
     * @param modelId Model ID
     */
    void setModelId(const std::string& modelId);
    
    /**
     * @brief Get the model ID
     * 
     * @return Model ID
     */
    std::string getModelId() const;
    
    /**
     * @brief Set the model description
     * 
     * @param description Model description
     */
    void setModelDescription(const std::string& description);
    
    /**
     * @brief Get the model description
     * 
     * @return Model description
     */
    std::string getModelDescription() const;
    
    /**
     * @brief Set model version
     * 
     * @param version Model version
     */
    void setModelVersion(const std::string& version);
    
    /**
     * @brief Get model version
     * 
     * @return Model version
     */
    std::string getModelVersion() const;
    
    /**
     * @brief Set modeling authority set
     * 
     * @param modelingAuthoritySet Modeling authority set
     */
    void setModelingAuthoritySet(const std::string& modelingAuthoritySet);
    
    /**
     * @brief Get modeling authority set
     * 
     * @return Modeling authority set
     */
    std::string getModelingAuthoritySet() const;

private:
    // Map of object ID to object instance (for all types)
    std::unordered_map<std::string, std::shared_ptr<CimIdentifiedObject>> objects_;
    
    // Map of type index to vector of object IDs
    std::unordered_map<std::type_index, std::vector<std::string>> typeIndex_;
    
    // Model metadata
    std::string modelId_;
    std::string modelDescription_;
    std::string modelVersion_;
    std::string modelingAuthoritySet_;
    
    // Reference resolution functions
    bool resolveBaseReferences();
    bool resolveEquipmentReferences();
};

// Template implementations
template<typename T>
bool CimModel::addObject(std::shared_ptr<T> object) {
    // Implementation details will be in the CPP file
    return false;
}

template<typename T>
std::shared_ptr<T> CimModel::getObject(const std::string& id) const {
    // Implementation details will be in the CPP file
    return nullptr;
}

template<typename T>
std::vector<std::shared_ptr<T>> CimModel::getObjectsByType() const {
    // Implementation details will be in the CPP file
    return {};
}

template<typename T>
size_t CimModel::getTypeCount() const {
    // Implementation details will be in the CPP file
    return 0;
}

} // namespace cim17
