cmake_minimum_required(VERSION 3.20)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Check if we're using Conan toolchain
if(EXISTS "${CMAKE_TOOLCHAIN_FILE}")
    message(STATUS "Using Conan toolchain: ${CMAKE_TOOLCHAIN_FILE}")
endif()

project(GamepadMCPServer VERSION 1.0.0 LANGUAGES CXX)

# Add Conan-generated files to paths
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}")

# Options
option(WITH_GAMEPAD_CORE "Include gamepad-core integration" ON)
option(WITH_BLUETOOTH "Include Bluetooth support" ON)
option(WITH_AUDIO "Include audio support" ON)
option(WITH_CONFIG "Include configuration management" ON)

# Find required packages
find_package(nlohmann_json REQUIRED)
find_package(jsoncpp REQUIRED)

# System libraries
if(UNIX AND NOT APPLE)
    find_package(Threads REQUIRED)
endif()

# Include directories
include_directories(include)
include_directories(${CMAKE_BINARY_DIR}/include)
include_directories(external/TinyMCP/Source)
include_directories(external/TinyMCP/Source/External/jsoncpp/include)

# Add TinyMCP subdirectory
add_subdirectory(external/TinyMCP EXCLUDE_FROM_ALL)

# Source files
set(MCP_SOURCES
    src/mcp_server.cpp
    src/gamepad_server.cpp
    src/mcp_prompts_client.cpp
)

# Create library
add_library(gamepad_mcp_server ${MCP_SOURCES})

# Link dependencies
target_link_libraries(gamepad_mcp_server
    PRIVATE
        tinymcp
        nlohmann_json::nlohmann_json
        jsoncpp_lib
)

# Set include directories for the library
target_include_directories(gamepad_mcp_server
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        external/TinyMCP/Source
        external/TinyMCP/Source/External/jsoncpp/include
)

# System libraries
if(UNIX AND NOT APPLE)
    target_link_libraries(gamepad_mcp_server PRIVATE Threads::Threads)
endif()

# Conditional dependencies
if(WITH_GAMEPAD_CORE)
    # This would link to gamepad-core when available
    target_compile_definitions(gamepad_mcp_server PRIVATE WITH_GAMEPAD_CORE)
endif()

if(WITH_BLUETOOTH)
    target_compile_definitions(gamepad_mcp_server PRIVATE WITH_BLUETOOTH)
endif()

if(WITH_AUDIO)
    target_compile_definitions(gamepad_mcp_server PRIVATE WITH_AUDIO)
endif()

if(WITH_CONFIG)
    target_compile_definitions(gamepad_mcp_server PRIVATE WITH_CONFIG)
endif()

# Create executable
add_executable(gamepad-mcp-server-app src/main.cpp)
target_link_libraries(gamepad-mcp-server-app PRIVATE gamepad_mcp_server)

# Install targets
install(TARGETS gamepad_mcp_server gamepad-mcp-server-app
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/ DESTINATION include)

# Install MCP configuration
install(FILES mcp-config.json
    DESTINATION share/gamepad-mcp-server/config
)

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