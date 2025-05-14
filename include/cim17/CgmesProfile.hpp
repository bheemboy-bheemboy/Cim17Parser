#pragma once

#include <string>
#include <vector>

namespace cim17 {

/**
 * @enum CgmesProfile
 * @brief Enumeration of CGMES profiles defined in CGMES v3
 */
enum class CgmesProfile {
    UNKNOWN,     ///< Unknown profile
    EQ,          ///< Equipment profile
    TP,          ///< Topology profile
    SSH,         ///< Steady State Hypothesis profile
    SV,          ///< State Variables profile
    DY,          ///< Dynamics profile
    GL,          ///< Geographical Location profile
    DL,          ///< Diagram Layout profile
    EQBD,        ///< Equipment Boundary profile
    TPBD,        ///< Topology Boundary profile
    OP,          ///< Operation profile (new in CGMES v3)
    SC           ///< Short Circuit profile (new in CGMES v3)
};

/**
 * @class CgmesProfileInfo
 * @brief Class to handle CGMES profile information
 */
class CgmesProfileInfo {
public:
    /**
     * @brief Convert profile enum to string
     * 
     * @param profile Profile enum value
     * @return String representation of the profile
     */
    static std::string toString(CgmesProfile profile);
    
    /**
     * @brief Convert profile string to enum
     * 
     * @param profileStr Profile string
     * @return Profile enum value
     */
    static CgmesProfile fromString(const std::string& profileStr);
    
    /**
     * @brief Get the namespace URI for a profile
     * 
     * @param profile Profile enum value
     * @return Namespace URI
     */
    static std::string getNamespaceUri(CgmesProfile profile);
    
    /**
     * @brief Check if a namespace URI matches a profile
     * 
     * @param uri Namespace URI
     * @param profile Profile to check
     * @return True if the URI matches the profile
     */
    static bool matchesProfile(const std::string& uri, CgmesProfile profile);
    
    /**
     * @brief Determine profile from namespace URI
     * 
     * @param uri Namespace URI
     * @return Detected profile, UNKNOWN if not recognized
     */
    static CgmesProfile detectProfile(const std::string& uri);
    
    /**
     * @brief Get all profiles used in CGMES v3
     * 
     * @return Vector of all profile enum values
     */
    static std::vector<CgmesProfile> getAllProfiles();
};

} // namespace cim17
