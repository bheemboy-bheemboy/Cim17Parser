cmake_minimum_required(VERSION 3.15)
project(cim17parser VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include Conan generated files
if(EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    conan_basic_setup()
elseif(EXISTS "${CMAKE_BINARY_DIR}/conan_paths.cmake")
    include("${CMAKE_BINARY_DIR}/conan_paths.cmake")
endif()

# Find package dependencies
find_package(pugixml REQUIRED)
find_package(fmt REQUIRED)
find_package(Boost REQUIRED)
find_package(spdlog REQUIRED)
find_package(GTest REQUIRED)

# Configure compile options
add_compile_options(-Wall -Wextra -Wpedantic)

# Source files
set(SOURCES
    src/Cim17Parser.cpp
    src/CgmesProfile.cpp
    src/XmlHandler.cpp
    src/CimModel.cpp
    src/CimEquipment.cpp
    src/CimRdfParser.cpp
)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Add library target
add_library(cim17parser ${SOURCES})
set_target_properties(cim17parser PROPERTIES OUTPUT_NAME "cim17parser")

# Link dependencies
target_link_libraries(cim17parser
    PRIVATE
    pugixml::pugixml
    fmt::fmt
    spdlog::spdlog
    Boost::boost
)

# Create main executable
add_executable(cim17parser-app src/main.cpp)
target_link_libraries(cim17parser-app PRIVATE cim17parser)

# Add tests
enable_testing()
add_executable(cim17parser-tests test/test_main.cpp test/test_parser.cpp)
target_link_libraries(cim17parser-tests PRIVATE cim17parser GTest::gtest GTest::gtest_main)
add_test(NAME cim17parser-tests COMMAND cim17parser-tests)

# Installation
install(TARGETS cim17parser
    EXPORT cim17parser-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/
    DESTINATION include
)

install(EXPORT cim17parser-targets
    FILE cim17parser-targets.cmake
    NAMESPACE cim17parser::
    DESTINATION lib/cmake/cim17parser
)
