#include "gamepad_server.h"
#include <iostream>
#include <csignal>
#include <atomic>
#include <thread>
#include <chrono>

using namespace gamepad_mcp;

// Global flag for graceful shutdown
std::atomic<bool> running{true};

// Signal handler for graceful shutdown
void signal_handler(int signal) {
    std::cout << "\nReceived signal " << signal << ", shutting down..." << std::endl;
    running = false;
}

int main(int argc, char* argv[]) {
    // Register signal handlers
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    std::cout << "Gamepad MCP Server v1.0.0" << std::endl;
    std::cout << "Starting MCP server..." << std::endl;

    // Server configuration
    ServerConfig config;
    config.host = "localhost";
    config.port = 3001;
    config.enable_cors = true;
    config.log_level = "info";

    // Create and initialize MCP server
    GamepadMCPServer server;

    if (!server.initialize(config)) {
        std::cerr << "Failed to initialize MCP server" << std::endl;
        return 1;
    }

    // Start the server
    if (!server.start()) {
        std::cerr << "Failed to start MCP server" << std::endl;
        return 1;
    }

    std::cout << "MCP server started successfully on " << config.host << ":" << config.port << std::endl;
    std::cout << "Available MCP tools:" << std::endl;
    std::cout << "  - gamepad_list_devices" << std::endl;
    std::cout << "  - gamepad_connect_device" << std::endl;
    std::cout << "  - gamepad_load_mapping" << std::endl;
    std::cout << "  - gamepad_remap_button" << std::endl;
    std::cout << "  - gamepad_enable_voice_alerts" << std::endl;
    std::cout << "  - gamepad_scan_bluetooth" << std::endl;
    std::cout << "Press Ctrl+C to stop..." << std::endl;

    // Main loop
    while (running && server.is_running()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Periodic health check could go here
        // For example, check if all integrated components are still available
    }

    std::cout << "Stopping MCP server..." << std::endl;
    server.stop();

    std::cout << "Server shutdown complete." << std::endl;
    return 0;
}