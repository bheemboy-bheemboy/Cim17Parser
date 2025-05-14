#pragma once

#include <string>
#include <memory>
#include <optional>

namespace cim17 {

/**
 * @class CimIdentifiedObject
 * @brief Base class for all CIM objects with an identifier
 */
class CimIdentifiedObject {
public:
    /**
     * @brief Constructor
     * 
     * @param mRID Master Resource Identifier
     */
    explicit CimIdentifiedObject(const std::string& mRID);
    
    /**
     * @brief Virtual destructor
     */
    virtual ~CimIdentifiedObject() = default;
    
    /**
     * @brief Get the master resource identifier (mRID)
     * 
     * @return mRID value
     */
    std::string getMRID() const;
    
    /**
     * @brief Set the name of the object
     * 
     * @param name Name
     */
    void setName(const std::string& name);
    
    /**
     * @brief Get the name of the object
     * 
     * @return Name, or empty string if not set
     */
    std::string getName() const;
    
    /**
     * @brief Set the description of the object
     * 
     * @param description Description
     */
    void setDescription(const std::string& description);
    
    /**
     * @brief Get the description of the object
     * 
     * @return Description, or empty string if not set
     */
    std::string getDescription() const;
    
    /**
     * @brief Set the alias name of the object
     * 
     * @param aliasName Alias name
     */
    void setAliasName(const std::string& aliasName);
    
    /**
     * @brief Get the alias name of the object
     * 
     * @return Alias name, or empty string if not set
     */
    std::string getAliasName() const;
    
    /**
     * @brief Get the type name of the object (for RTTI)
     * 
     * @return Type name
     */
    virtual std::string getTypeName() const = 0;

private:
    // Master resource identifier (unique ID)
    std::string mRID_;
    
    // Name of the object
    std::string name_;
    
    // Description of the object
    std::string description_;
    
    // Alias name of the object
    std::string aliasName_;
};

/**
 * @class CimEquipment
 * @brief Base class for power system equipment objects
 */
class CimEquipment : public CimIdentifiedObject {
public:
    /**
     * @brief Constructor
     * 
     * @param mRID Master Resource Identifier
     */
    explicit CimEquipment(const std::string& mRID);
    
    /**
     * @brief Check if the equipment is in service
     * 
     * @return True if the equipment is in service
     */
    bool isInService() const;
    
    /**
     * @brief Set the in-service state of the equipment
     * 
     * @param inService True if the equipment is in service
     */
    void setInService(bool inService);
    
    /**
     * @brief Set the normal state of the equipment
     * 
     * @param normallyInService True if the equipment is normally in service
     */
    void setNormallyInService(bool normallyInService);
    
    /**
     * @brief Check if the equipment is normally in service
     * 
     * @return True if the equipment is normally in service
     */
    bool isNormallyInService() const;
    
    /**
     * @brief Set aggregate state of the equipment
     * 
     * @param aggregate True if the equipment is an aggregate
     */
    void setAggregate(bool aggregate);
    
    /**
     * @brief Check if the equipment is an aggregate
     * 
     * @return True if the equipment is an aggregate
     */
    bool isAggregate() const;

private:
    // In-service state
    bool inService_ = true;
    
    // Normal service state
    bool normallyInService_ = true;
    
    // Aggregate flag
    bool aggregate_ = false;
};

// Forward declare some key CIM17 classes

/**
 * @class Terminal
 * @brief Class representing a terminal connection point
 */
class Terminal : public CimIdentifiedObject {
public:
    explicit Terminal(const std::string& mRID);
    std::string getTypeName() const override { return "Terminal"; }
    
    // Terminal-specific methods would go here
    void setConnectedEquipment(std::shared_ptr<CimEquipment> equipment);
    std::shared_ptr<CimEquipment> getConnectedEquipment() const;

private:
    std::weak_ptr<CimEquipment> connectedEquipment_;
};

/**
 * @class ACLineSegment
 * @brief Class representing an AC transmission line segment
 */
class ACLineSegment : public CimEquipment {
public:
    explicit ACLineSegment(const std::string& mRID);
    std::string getTypeName() const override { return "ACLineSegment"; }
    
    // AC line specific getters/setters
    void setR(double r);
    double getR() const;
    
    void setX(double x);
    double getX() const;
    
    void setBch(double bch);
    double getBch() const;
    
    void setLength(double length);
    double getLength() const;

private:
    double r_ = 0.0;        // Resistance
    double x_ = 0.0;        // Reactance
    double bch_ = 0.0;      // Susceptance
    double length_ = 0.0;   // Length in km
};

/**
 * @class PowerTransformer
 * @brief Class representing a power transformer
 */
class PowerTransformer : public CimEquipment {
public:
    explicit PowerTransformer(const std::string& mRID);
    std::string getTypeName() const override { return "PowerTransformer"; }
    
    // PowerTransformer specific getters/setters
    void setTransformerType(const std::string& type);
    std::string getTransformerType() const;

private:
    std::string transformerType_;
};

/**
 * @class SynchronousMachine
 * @brief Class representing a synchronous generator or motor
 */
class SynchronousMachine : public CimEquipment {
public:
    explicit SynchronousMachine(const std::string& mRID);
    std::string getTypeName() const override { return "SynchronousMachine"; }
    
    // SynchronousMachine specific getters/setters
    void setRatedS(double ratedS);
    double getRatedS() const;
    
    void setRatedU(double ratedU);
    double getRatedU() const;
    
    void setType(const std::string& type);
    std::string getType() const;

private:
    double ratedS_ = 0.0;   // Rated apparent power
    double ratedU_ = 0.0;   // Rated voltage
    std::string type_;      // Machine type
};

/**
 * @class SolarGeneratingUnit
 * @brief Class representing a solar power generation unit (new in CIM17)
 */
class SolarGeneratingUnit : public CimEquipment {
public:
    explicit SolarGeneratingUnit(const std::string& mRID);
    std::string getTypeName() const override { return "SolarGeneratingUnit"; }
    
    // SolarGeneratingUnit specific getters/setters
    void setMaxCapacity(double maxCapacity);
    double getMaxCapacity() const;

private:
    double maxCapacity_ = 0.0;
};

/**
 * @class PowerElectronicsConnection
 * @brief Class representing a power electronics connection point (new in CIM17)
 */
class PowerElectronicsConnection : public CimEquipment {
public:
    explicit PowerElectronicsConnection(const std::string& mRID);
    std::string getTypeName() const override { return "PowerElectronicsConnection"; }
    
    // PowerElectronicsConnection specific getters/setters
    void setMaxP(double maxP);
    double getMaxP() const;
    
    void setMaxQ(double maxQ);
    double getMaxQ() const;

private:
    double maxP_ = 0.0;  // Maximum active power
    double maxQ_ = 0.0;  // Maximum reactive power
};

} // namespace cim17
