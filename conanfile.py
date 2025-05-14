from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout
from conan.tools.build import check_min_cppstd


class Cim17ParserConan(ConanFile):
    name = "cim17parser"
    version = "0.1.0"
    
    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    
    # Sources are located in the same place as this recipe
    exports_sources = "CMakeLists.txt", "src/*", "include/*", "test/*"
    
    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC
    
    def layout(self):
        cmake_layout(self)
    
    def validate(self):
        check_min_cppstd(self, "17")
    
    def requirements(self):
        # XML parsing libraries
        self.requires("pugixml/1.13")
        self.requires("fmt/9.1.0")
        
        # Utility libraries
        self.requires("boost/1.81.0")
        self.requires("spdlog/1.11.0")
        
        # Testing
        self.requires("gtest/1.13.0")
    
    def build_requirements(self):
        self.tool_requires("cmake/3.25.0")
    
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        
        # Run tests if they exist
        if self.should_test:
            cmake.test()
    
    def package(self):
        cmake = CMake(self)
        cmake.install()
    
    def package_info(self):
        self.cpp_info.libs = ["cim17parser"]
