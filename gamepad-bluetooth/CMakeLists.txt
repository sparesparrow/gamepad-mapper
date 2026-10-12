cmake_minimum_required(VERSION 3.20)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Check if we're using Conan toolchain
if(EXISTS "${CMAKE_TOOLCHAIN_FILE}")
    message(STATUS "Using Conan toolchain: ${CMAKE_TOOLCHAIN_FILE}")
endif()

project(GamepadBluetooth VERSION 1.0.0 LANGUAGES CXX)

# Add Conan-generated files to paths
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}")

# Options
option(WITH_BLUETOOTH_STUBS "Use Bluetooth stubs for testing without hardware" OFF)

# Find required packages
find_package(PkgConfig REQUIRED)

# Bluetooth libraries
find_package(PkgConfig REQUIRED)
pkg_check_modules(BLUEZ bluez)
if(NOT BLUEZ_FOUND)
    # Fallback: try to find libbluetooth directly
    find_library(BLUEZ_LIBRARIES bluetooth)
    if(BLUEZ_LIBRARIES)
        set(BLUEZ_FOUND TRUE)
        set(BLUEZ_LIBRARIES ${BLUEZ_LIBRARIES})
        set(BLUEZ_INCLUDE_DIRS "")
    endif()
endif()

if(NOT BLUEZ_FOUND AND NOT WITH_BLUETOOTH_STUBS)
    message(WARNING "Bluetooth libraries not found. Use WITH_BLUETOOTH_STUBS=ON for testing.")
    set(WITH_BLUETOOTH_STUBS ON)
endif()

# Find nlohmann_json
find_package(nlohmann_json REQUIRED)

# Include directories
include_directories(include)
include_directories(${CMAKE_BINARY_DIR}/include)

# Source files
set(BLUETOOTH_SOURCES
    src/bluetooth_scanner.cpp
    src/bluetooth_connector.cpp
    src/hid_parser.cpp
)

# Add stubs if enabled
if(WITH_BLUETOOTH_STUBS)
    list(APPEND BLUETOOTH_SOURCES src/bluetooth_stubs.cpp)
    add_definitions(-DWITH_BLUETOOTH_STUBS)
endif()

# Create library
add_library(gamepad_bluetooth ${BLUETOOTH_SOURCES})

# Link libraries
target_link_libraries(gamepad_bluetooth PRIVATE nlohmann_json::nlohmann_json)

# Bluetooth libraries
if(BLUEZ_FOUND AND NOT WITH_BLUETOOTH_STUBS)
    target_include_directories(gamepad_bluetooth PRIVATE ${BLUEZ_INCLUDE_DIRS})
    target_link_libraries(gamepad_bluetooth PRIVATE ${BLUEZ_LIBRARIES})
    target_compile_definitions(gamepad_bluetooth PRIVATE WITH_BLUEZ)
endif()

# Set include directories for the library
target_include_directories(gamepad_bluetooth
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# System libraries
if(UNIX AND NOT APPLE)
    target_link_libraries(gamepad_bluetooth PRIVATE pthread udev)
endif()

# Install targets
install(TARGETS gamepad_bluetooth
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/ DESTINATION include)

# Testing (optional)
option(BUILD_TESTS "Build tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    find_package(GTest REQUIRED)
    add_subdirectory(tests)
endif()

# Documentation
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_INPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/docs)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
    )
endif()