#include "cim17/CimEquipment.hpp"

namespace cim17 {

// CimIdentifiedObject implementation
CimIdentifiedObject::CimIdentifiedObject(const std::string& mRID)
    : mRID_(mRID),
      name_(""),
      description_(""),
      aliasName_("") {
}

std::string CimIdentifiedObject::getMRID() const {
    return mRID_;
}

void CimIdentifiedObject::setName(const std::string& name) {
    name_ = name;
}

std::string CimIdentifiedObject::getName() const {
    return name_;
}

void CimIdentifiedObject::setDescription(const std::string& description) {
    description_ = description;
}

std::string CimIdentifiedObject::getDescription() const {
    return description_;
}

void CimIdentifiedObject::setAliasName(const std::string& aliasName) {
    aliasName_ = aliasName;
}

std::string CimIdentifiedObject::getAliasName() const {
    return aliasName_;
}

// CimEquipment implementation
CimEquipment::CimEquipment(const std::string& mRID)
    : CimIdentifiedObject(mRID),
      inService_(true),
      normallyInService_(true),
      aggregate_(false) {
}

bool CimEquipment::isInService() const {
    return inService_;
}

void CimEquipment::setInService(bool inService) {
    inService_ = inService;
}

void CimEquipment::setNormallyInService(bool normallyInService) {
    normallyInService_ = normallyInService;
}

bool CimEquipment::isNormallyInService() const {
    return normallyInService_;
}

void CimEquipment::setAggregate(bool aggregate) {
    aggregate_ = aggregate;
}

bool CimEquipment::isAggregate() const {
    return aggregate_;
}

// Terminal implementation
Terminal::Terminal(const std::string& mRID)
    : CimIdentifiedObject(mRID) {
}

void Terminal::setConnectedEquipment(std::shared_ptr<CimEquipment> equipment) {
    connectedEquipment_ = equipment;
}

std::shared_ptr<CimEquipment> Terminal::getConnectedEquipment() const {
    return connectedEquipment_.lock();
}

// ACLineSegment implementation
ACLineSegment::ACLineSegment(const std::string& mRID)
    : CimEquipment(mRID),
      r_(0.0),
      x_(0.0),
      bch_(0.0),
      length_(0.0) {
}

void ACLineSegment::setR(double r) {
    r_ = r;
}

double ACLineSegment::getR() const {
    return r_;
}

void ACLineSegment::setX(double x) {
    x_ = x;
}

double ACLineSegment::getX() const {
    return x_;
}

void ACLineSegment::setBch(double bch) {
    bch_ = bch;
}

double ACLineSegment::getBch() const {
    return bch_;
}

void ACLineSegment::setLength(double length) {
    length_ = length;
}

double ACLineSegment::getLength() const {
    return length_;
}

// PowerTransformer implementation
PowerTransformer::PowerTransformer(const std::string& mRID)
    : CimEquipment(mRID),
      transformerType_("") {
}

void PowerTransformer::setTransformerType(const std::string& type) {
    transformerType_ = type;
}

std::string PowerTransformer::getTransformerType() const {
    return transformerType_;
}

// SynchronousMachine implementation
SynchronousMachine::SynchronousMachine(const std::string& mRID)
    : CimEquipment(mRID),
      ratedS_(0.0),
      ratedU_(0.0),
      type_("") {
}

void SynchronousMachine::setRatedS(double ratedS) {
    ratedS_ = ratedS;
}

double SynchronousMachine::getRatedS() const {
    return ratedS_;
}

void SynchronousMachine::setRatedU(double ratedU) {
    ratedU_ = ratedU;
}

double SynchronousMachine::getRatedU() const {
    return ratedU_;
}

void SynchronousMachine::setType(const std::string& type) {
    type_ = type;
}

std::string SynchronousMachine::getType() const {
    return type_;
}

// SolarGeneratingUnit implementation
SolarGeneratingUnit::SolarGeneratingUnit(const std::string& mRID)
    : CimEquipment(mRID),
      maxCapacity_(0.0) {
}

void SolarGeneratingUnit::setMaxCapacity(double maxCapacity) {
    maxCapacity_ = maxCapacity;
}

double SolarGeneratingUnit::getMaxCapacity() const {
    return maxCapacity_;
}

// PowerElectronicsConnection implementation
PowerElectronicsConnection::PowerElectronicsConnection(const std::string& mRID)
    : CimEquipment(mRID),
      maxP_(0.0),
      maxQ_(0.0) {
}

void PowerElectronicsConnection::setMaxP(double maxP) {
    maxP_ = maxP;
}

double PowerElectronicsConnection::getMaxP() const {
    return maxP_;
}

void PowerElectronicsConnection::setMaxQ(double maxQ) {
    maxQ_ = maxQ;
}

double PowerElectronicsConnection::getMaxQ() const {
    return maxQ_;
}

} // namespace cim17
