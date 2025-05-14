#include "cim17/CgmesProfile.hpp"
#include <unordered_map>
#include <algorithm>

namespace cim17 {

// Namespace URIs for CGMES v3 profiles
static const std::unordered_map<CgmesProfile, std::string> PROFILE_URIS = {
    {CgmesProfile::EQ, "http://entsoe.eu/CIM/EquipmentCore/3/1"},
    {CgmesProfile::TP, "http://entsoe.eu/CIM/Topology/3/1"},
    {CgmesProfile::SSH, "http://entsoe.eu/CIM/SteadyStateHypothesis/3/1"},
    {CgmesProfile::SV, "http://entsoe.eu/CIM/StateVariables/3/1"},
    {CgmesProfile::DY, "http://entsoe.eu/CIM/Dynamics/3/1"},
    {CgmesProfile::GL, "http://entsoe.eu/CIM/GeographicalLocation/3/1"},
    {CgmesProfile::DL, "http://entsoe.eu/CIM/DiagramLayout/3/1"},
    {CgmesProfile::EQBD, "http://entsoe.eu/CIM/EquipmentBoundary/3/1"},
    {CgmesProfile::TPBD, "http://entsoe.eu/CIM/TopologyBoundary/3/1"},
    {CgmesProfile::OP, "http://entsoe.eu/CIM/Operation/3/1"},
    {CgmesProfile::SC, "http://entsoe.eu/CIM/ShortCircuit/3/1"}
};

// Profile name strings
static const std::unordered_map<CgmesProfile, std::string> PROFILE_NAMES = {
    {CgmesProfile::UNKNOWN, "UNKNOWN"},
    {CgmesProfile::EQ, "Equipment"},
    {CgmesProfile::TP, "Topology"},
    {CgmesProfile::SSH, "SteadyStateHypothesis"},
    {CgmesProfile::SV, "StateVariables"},
    {CgmesProfile::DY, "Dynamics"},
    {CgmesProfile::GL, "GeographicalLocation"},
    {CgmesProfile::DL, "DiagramLayout"},
    {CgmesProfile::EQBD, "EquipmentBoundary"},
    {CgmesProfile::TPBD, "TopologyBoundary"},
    {CgmesProfile::OP, "Operation"},
    {CgmesProfile::SC, "ShortCircuit"}
};

std::string CgmesProfileInfo::toString(CgmesProfile profile) {
    auto it = PROFILE_NAMES.find(profile);
    if (it != PROFILE_NAMES.end()) {
        return it->second;
    }
    return "UNKNOWN";
}

CgmesProfile CgmesProfileInfo::fromString(const std::string& profileStr) {
    for (const auto& entry : PROFILE_NAMES) {
        if (entry.second == profileStr) {
            return entry.first;
        }
    }
    return CgmesProfile::UNKNOWN;
}

std::string CgmesProfileInfo::getNamespaceUri(CgmesProfile profile) {
    auto it = PROFILE_URIS.find(profile);
    if (it != PROFILE_URIS.end()) {
        return it->second;
    }
    return "";
}

bool CgmesProfileInfo::matchesProfile(const std::string& uri, CgmesProfile profile) {
    auto it = PROFILE_URIS.find(profile);
    if (it != PROFILE_URIS.end()) {
        return it->second == uri;
    }
    return false;
}

CgmesProfile CgmesProfileInfo::detectProfile(const std::string& uri) {
    for (const auto& entry : PROFILE_URIS) {
        if (entry.second == uri) {
            return entry.first;
        }
    }
    return CgmesProfile::UNKNOWN;
}

std::vector<CgmesProfile> CgmesProfileInfo::getAllProfiles() {
    std::vector<CgmesProfile> profiles;
    profiles.reserve(PROFILE_URIS.size());
    
    for (const auto& entry : PROFILE_URIS) {
        profiles.push_back(entry.first);
    }
    
    return profiles;
}

} // namespace cim17
