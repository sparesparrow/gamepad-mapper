#!/usr/bin/env python3
"""
Gamepad Bluetooth Conan Package Recipe
Bluetooth HID device support for gamepad connectivity
"""

from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake


class GamepadBluetoothConan(ConanFile):
    name = "gamepad-bluetooth"
    version = "1.0.0"
    description = "Bluetooth HID device support for wireless gamepad connectivity"
    homepage = "https://github.com/sparesparrow/gamepad-bluetooth"
    url = "https://github.com/sparesparrow/gamepad-bluetooth"
    license = "MIT"
    topics = ("bluetooth", "hid", "gamepad", "wireless", "linux")

    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "with_bluetooth_stubs": [True, False],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "with_bluetooth_stubs": False,
    }

    generators = "CMakeDeps", "CMakeToolchain"
    test_requires = "gtest/1.14.0"

    def requirements(self):
        """Add dependencies"""
        self.requires("nlohmann_json/3.11.2")

    def configure(self):
        """Configure package options"""
        if self.options.shared:
            del self.options.fPIC

        # Bluetooth stubs are for testing without hardware
        if self.options.with_bluetooth_stubs:
            # Remove system Bluetooth dependencies when using stubs
            pass

    def layout(self):
        """Define the layout for generators"""
        self.folders.generators = "build"

    def export_sources(self):
        """Export source files"""
        self.copy("CMakeLists.txt", src=".", dst=".")
        self.copy("src/**", src=".", dst=".")
        self.copy("include/**", src=".", dst=".")
        self.copy("cmake/**", src=".", dst=".")

    def export_test_sources(self):
        """Export test sources"""
        self.copy("tests/**", src=".", dst=".")

    def build(self):
        """Build using CMake"""
        cmake = CMake(self)

        # Define CMake options
        cmake_defines = {}
        if self.options.with_bluetooth_stubs:
            cmake_defines["WITH_BLUETOOTH_STUBS"] = "ON"

        cmake.configure(variables=cmake_defines)
        cmake.build()

    def package(self):
        """Package the built artifacts"""
        cmake = CMake(self)
        cmake.install()

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
        self.cpp_info.libs = ["gamepad_bluetooth"]
        self.cpp_info.requires = ["nlohmann_json::nlohmann_json"]

        # System libraries for Linux
        if self.settings.os == "Linux":
            if not self.options.with_bluetooth_stubs:
                self.cpp_info.system_libs.extend(["bluetooth", "udev"])
            self.cpp_info.system_libs.extend(["pthread"])

        # Define components
        self.cpp_info.components["core"].libs = ["gamepad_bluetooth"]
        self.cpp_info.components["core"].requires = ["nlohmann_json::nlohmann_json"]

        if not self.options.with_bluetooth_stubs:
            self.cpp_info.components["bluetooth"].libs = []
            self.cpp_info.components["bluetooth"].system_libs = ["bluetooth", "udev"]