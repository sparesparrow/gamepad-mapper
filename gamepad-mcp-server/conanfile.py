#!/usr/bin/env python3
"""
Gamepad MCP Server Conan Package Recipe
MCP server integration for gamepad control and AI interaction
"""

from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake


class GamepadMCPServerConan(ConanFile):
    name = "gamepad-mcp-server"
    version = "1.0.0"
    description = "MCP server for gamepad control and AI integration"
    homepage = "https://github.com/sparesparrow/gamepad-mcp-server"
    url = "https://github.com/sparesparrow/gamepad-mcp-server"
    license = "MIT"
    topics = ("mcp", "gamepad", "ai", "automation", "server")

    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "with_gamepad_core": [True, False],
        "with_bluetooth": [True, False],
        "with_audio": [True, False],
        "with_config": [True, False],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "with_gamepad_core": True,
        "with_bluetooth": True,
        "with_audio": True,
        "with_config": True,
    }

    generators = "CMakeDeps", "CMakeToolchain"
    test_requires = "gtest/1.14.0"

    def requirements(self):
        """Add dependencies"""
        self.requires("nlohmann_json/3.11.2")
        self.requires("jsoncpp/1.9.5")
        self.requires("boost/1.83.0")
        self.requires("openssl/3.2.0")

        # Optional component dependencies
        if self.options.with_gamepad_core:
            # gamepad-core would be added here when available
            pass

    def configure(self):
        """Configure package options"""
        if self.options.shared:
            del self.options.fPIC

    def layout(self):
        """Define the layout for generators"""
        self.folders.generators = "build"

    def export_sources(self):
        """Export source files"""
        self.copy("CMakeLists.txt", src=".", dst=".")
        self.copy("src/**", src=".", dst=".")
        self.copy("include/**", src=".", dst=".")
        self.copy("external/**", src=".", dst=".")
        self.copy("mcp-config.json", src=".", dst=".")
        self.copy("cmake/**", src=".", dst=".")

    def export_test_sources(self):
        """Export test sources"""
        self.copy("tests/**", src=".", dst=".")

    def build(self):
        """Build using CMake"""
        cmake = CMake(self)

        # Define CMake options based on Conan options
        cmake_defines = {}
        if self.options.with_gamepad_core:
            cmake_defines["WITH_GAMEPAD_CORE"] = "ON"
        if self.options.with_bluetooth:
            cmake_defines["WITH_BLUETOOTH"] = "ON"
        if self.options.with_audio:
            cmake_defines["WITH_AUDIO"] = "ON"
        if self.options.with_config:
            cmake_defines["WITH_CONFIG"] = "ON"

        cmake.configure(variables=cmake_defines)
        cmake.build()

    def package(self):
        """Package the built artifacts"""
        cmake = CMake(self)
        cmake.install()

        # Copy configuration files
        self.copy("mcp-config.json", src=self.source_folder, dst="res")

        # Copy license
        self.copy("LICENSE", src=self.source_folder, dst="licenses")

    def test(self):
        """Run tests"""
        if self.conf.get("tools.cmake.cmaketoolchain:generator", default=False, check_type=str):
            cmake = CMake(self)
            cmake.configure()
            cmake.build()
            cmake.test()

    def package_info(self):
        """Package info"""
        self.cpp_info.libs = ["gamepad_mcp_server"]

        # Required libraries
        self.cpp_info.requires = [
            "nlohmann_json::nlohmann_json",
            "jsoncpp::jsoncpp",
            "boost::boost",
            "openssl::openssl"
        ]

        # System libraries
        if self.settings.os == "Linux":
            self.cpp_info.system_libs.extend(["pthread", "dl"])

        # Define components for different features
        self.cpp_info.components["core"].libs = ["gamepad_mcp_server"]
        self.cpp_info.components["core"].requires = [
            "nlohmann_json::nlohmann_json",
            "jsoncpp::jsoncpp"
        ]

        if self.options.with_gamepad_core:
            self.cpp_info.components["gamepad_integration"].libs = []
            # gamepad-core requirement would go here

        # Executable
        self.cpp_info.apps = ["bin/gamepad-mcp-server-app"]