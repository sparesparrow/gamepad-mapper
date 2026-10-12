cmake_minimum_required(VERSION 3.20)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Check if we're using Conan toolchain
if(EXISTS "${CMAKE_TOOLCHAIN_FILE}")
    message(STATUS "Using Conan toolchain: ${CMAKE_TOOLCHAIN_FILE}")
endif()

project(GamepadAudio VERSION 1.0.0 LANGUAGES CXX)

# Add Conan-generated files to paths
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}")

# Options for audio backends
option(WITH_ALSA "Enable ALSA audio backend" ON)
option(WITH_PULSEAUDIO "Enable PulseAudio backend" ON)
option(WITH_VOICE_ALERTS "Enable voice alerts (requires espeak)" ON)

# Find required packages
find_package(PkgConfig REQUIRED)

# Audio backends
if(WITH_ALSA)
    pkg_check_modules(ALSA alsa)
    if(ALSA_FOUND)
        message(STATUS "ALSA audio backend enabled")
    else()
        message(WARNING "ALSA not found. ALSA backend will be disabled.")
        set(WITH_ALSA OFF)
    endif()
endif()

if(WITH_PULSEAUDIO)
    pkg_check_modules(PULSEAUDIO libpulse)
    if(PULSEAUDIO_FOUND)
        message(STATUS "PulseAudio backend enabled")
    else()
        message(WARNING "PulseAudio not found. PulseAudio backend will be disabled.")
        set(WITH_PULSEAUDIO OFF)
    endif()
endif()

# Voice alerts
if(WITH_VOICE_ALERTS)
    find_program(ESPEAK_EXECUTABLE espeak)
    if(ESPEAK_EXECUTABLE)
        message(STATUS "Found espeak: ${ESPEAK_EXECUTABLE}")
    else()
        message(WARNING "espeak not found. Voice alerts will be disabled.")
        set(WITH_VOICE_ALERTS OFF)
    endif()
endif()

# Find nlohmann_json
find_package(nlohmann_json REQUIRED)

# Include directories
include_directories(include)
include_directories(${CMAKE_BINARY_DIR}/include)

# Source files
set(AUDIO_SOURCES
    src/audio_backend.cpp
    src/voice_alerts.cpp
)

# Create library
add_library(gamepad_audio ${AUDIO_SOURCES})

# Link libraries
target_link_libraries(gamepad_audio PRIVATE nlohmann_json::nlohmann_json)

# Audio backend linking
if(WITH_ALSA)
    target_include_directories(gamepad_audio PRIVATE ${ALSA_INCLUDE_DIRS})
    target_link_libraries(gamepad_audio PRIVATE ${ALSA_LIBRARIES})
    target_compile_definitions(gamepad_audio PRIVATE WITH_ALSA)
endif()

if(WITH_PULSEAUDIO)
    target_include_directories(gamepad_audio PRIVATE ${PULSEAUDIO_INCLUDE_DIRS})
    target_link_libraries(gamepad_audio PRIVATE ${PULSEAUDIO_LIBRARIES})
    target_compile_definitions(gamepad_audio PRIVATE WITH_PULSEAUDIO)
endif()

if(WITH_VOICE_ALERTS)
    target_compile_definitions(gamepad_audio PRIVATE WITH_VOICE_ALERTS)
endif()

# Set include directories for the library
target_include_directories(gamepad_audio
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# System libraries
if(UNIX AND NOT APPLE)
    target_link_libraries(gamepad_audio PRIVATE pthread dl)
endif()

# Install targets
install(TARGETS gamepad_audio
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/ DESTINATION include)

# Install audio resources (if any)
# install(DIRECTORY sounds/ DESTINATION share/gamepad-audio/sounds)

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