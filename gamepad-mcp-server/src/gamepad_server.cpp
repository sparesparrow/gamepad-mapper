#include "gamepad_server.h"
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <chrono>
#include <thread>

namespace gamepad_mcp {

// GamepadMCPServer implementation
GamepadMCPServer::GamepadMCPServer()
    : mcp_server_(std::make_unique<MCPServer>()) {
}

GamepadMCPServer::~GamepadMCPServer() = default;

bool GamepadMCPServer::initialize(const ServerConfig& config) {
    server_config_ = config;

    // Initialize MCP server
    if (!mcp_server_->initialize(config)) {
        std::cerr << "Failed to initialize MCP server" << std::endl;
        return false;
    }

    // Register gamepad-specific tools
    register_gamepad_tools();

    // Register gamepad resources
    register_gamepad_resources();

    // Register prompts
    register_gamepad_prompts();

    std::cout << "Gamepad MCP Server initialized successfully" << std::endl;
    return true;
}

bool GamepadMCPServer::start() {
    if (!mcp_server_) {
        std::cerr << "MCP server not initialized" << std::endl;
        return false;
    }

    return mcp_server_->start();
}

void GamepadMCPServer::stop() {
    if (mcp_server_) {
        mcp_server_->stop();
    }
}

bool GamepadMCPServer::is_running() const {
    return mcp_server_ && mcp_server_->is_running();
}

// Tool registration
bool GamepadMCPServer::register_gamepad_tools() {
    if (!mcp_server_) return false;

    // Device management tools
    mcp_server_->register_tool("gamepad_list_devices", "List all connected gamepad devices", [this](const nlohmann::json& params) {
        return handle_list_devices(params);
    });

    mcp_server_->register_tool("gamepad_connect_device", "Connect to a specific gamepad device", [this](const nlohmann::json& params) {
        return handle_connect_device(params);
    });

    mcp_server_->register_tool("gamepad_disconnect_device", "Disconnect a gamepad device", [this](const nlohmann::json& params) {
        return handle_disconnect_device(params);
    });

    // Mapping tools
    mcp_server_->register_tool("gamepad_load_mapping", "Load a gamepad mapping configuration", [this](const nlohmann::json& params) {
        return handle_load_mapping(params);
    });

    mcp_server_->register_tool("gamepad_save_mapping", "Save current gamepad mapping", [this](const nlohmann::json& params) {
        return handle_save_mapping(params);
    });

    mcp_server_->register_tool("gamepad_get_mapping", "Get current mapping configuration", [this](const nlohmann::json& params) {
        return handle_get_mapping(params);
    });

    mcp_server_->register_tool("gamepad_remap_button", "Remap a gamepad button", [this](const nlohmann::json& params) {
        return handle_remap_button(params);
    });

    // Control tools
    mcp_server_->register_tool("gamepad_start_mapping", "Start gamepad input mapping", [this](const nlohmann::json& params) {
        return handle_start_mapping(params);
    });

    mcp_server_->register_tool("gamepad_stop_mapping", "Stop gamepad input mapping", [this](const nlohmann::json& params) {
        return handle_stop_mapping(params);
    });

    // Audio tools
    mcp_server_->register_tool("gamepad_enable_voice_alerts", "Enable voice alerts", [this](const nlohmann::json& params) {
        return handle_enable_voice_alerts(params);
    });

    mcp_server_->register_tool("gamepad_play_sound", "Play a sound effect", [this](const nlohmann::json& params) {
        return handle_play_sound(params);
    });

    // Bluetooth tools
    mcp_server_->register_tool("gamepad_scan_bluetooth", "Scan for Bluetooth gamepad devices", [this](const nlohmann::json& params) {
        return handle_scan_bluetooth(params);
    });

    mcp_server_->register_tool("gamepad_connect_bluetooth", "Connect to Bluetooth gamepad", [this](const nlohmann::json& params) {
        return handle_connect_bluetooth(params);
    });

    return true;
}

// Resource registration
bool GamepadMCPServer::register_gamepad_resources() {
    if (!mcp_server_) return false;

    // Device information resource
    mcp_server_->register_resource("gamepad://devices", "Connected gamepad devices", "Current device status and information", "application/json");

    // Mapping configuration resource
    mcp_server_->register_resource("gamepad://mappings", "Available mapping presets", "List of available mapping configurations", "application/json");

    // System status resource
    mcp_server_->register_resource("gamepad://status", "System status", "Current system status and capabilities", "application/json");

    // Active configuration resource
    mcp_server_->register_resource("gamepad://active-mapping", "Active mapping", "Currently active mapping configuration", "application/json");

    return true;
}

// Prompt registration
bool GamepadMCPServer::register_gamepad_prompts() {
    if (!mcp_server_) return false;

    // Gamepad setup prompt
    mcp_server_->register_prompt("gamepad_setup", "Set up gamepad for common applications", [this](const nlohmann::json& params) {
        return handle_setup_prompt(params);
    });

    // Gaming optimization prompt
    mcp_server_->register_prompt("gamepad_gaming_optimization", "Optimize gamepad for gaming applications", [this](const nlohmann::json& params) {
        return handle_gaming_optimization_prompt(params);
    });

    // Accessibility setup prompt
    mcp_server_->register_prompt("gamepad_accessibility", "Set up gamepad for accessibility features", [this](const nlohmann::json& params) {
        return handle_accessibility_prompt(params);
    });

    return true;
}

// Tool handlers implementation
nlohmann::json GamepadMCPServer::handle_list_devices(const nlohmann::json& params) {
    nlohmann::json result;
    result["devices"] = nlohmann::json::array();

    // This would integrate with gamepad-core to get actual devices
    // For now, return mock data
    result["devices"].push_back({
        {"id", 0},
        {"name", "Xbox Controller"},
        {"connected", true},
        {"type", "xbox"}
    });

    result["success"] = true;
    return result;
}

nlohmann::json GamepadMCPServer::handle_connect_device(const nlohmann::json& params) {
    nlohmann::json result;

    if (!params.contains("device_id")) {
        result["error"] = "device_id parameter required";
        result["success"] = false;
        return result;
    }

    int device_id = params["device_id"];

    // This would integrate with gamepad-core to connect device
    result["device_id"] = device_id;
    result["connected"] = true;
    result["success"] = true;

    return result;
}

nlohmann::json GamepadMCPServer::handle_disconnect_device(const nlohmann::json& params) {
    nlohmann::json result;

    if (!params.contains("device_id")) {
        result["error"] = "device_id parameter required";
        result["success"] = false;
        return result;
    }

    int device_id = params["device_id"];

    // This would integrate with gamepad-core to disconnect device
    result["device_id"] = device_id;
    result["disconnected"] = true;
    result["success"] = true;

    return result;
}

nlohmann::json GamepadMCPServer::handle_load_mapping(const nlohmann::json& params) {
    nlohmann::json result;

    if (!params.contains("mapping_name")) {
        result["error"] = "mapping_name parameter required";
        result["success"] = false;
        return result;
    }

    std::string mapping_name = params["mapping_name"];

    // This would integrate with gamepad-config to load mapping
    result["mapping_name"] = mapping_name;
    result["loaded"] = true;
    result["success"] = true;

    return result;
}

nlohmann::json GamepadMCPServer::handle_save_mapping(const nlohmann::json& params) {
    nlohmann::json result;

    if (!params.contains("mapping_name")) {
        result["error"] = "mapping_name parameter required";
        result["success"] = false;
        return result;
    }

    std::string mapping_name = params["mapping_name"];

    // This would integrate with gamepad-config to save mapping
    result["mapping_name"] = mapping_name;
    result["saved"] = true;
    result["success"] = true;

    return result;
}

nlohmann::json GamepadMCPServer::handle_get_mapping(const nlohmann::json& params) {
    nlohmann::json result;

    // This would integrate with gamepad-config to get current mapping
    result["mappings"] = {
        {"A", {"type", "keyboard", "key", "space"}},
        {"B", {"type", "keyboard", "key", "escape"}},
        {"X", {"type", "mouse", "button", "left"}},
        {"Y", {"type", "mouse", "button", "right"}}
    };
    result["success"] = true;

    return result;
}

nlohmann::json GamepadMCPServer::handle_remap_button(const nlohmann::json& params) {
    nlohmann::json result;

    // Required parameters
    if (!params.contains("button") || !params.contains("input_type")) {
        result["error"] = "button and input_type parameters required";
        result["success"] = false;
        return result;
    }

    std::string button = params["button"];
    std::string input_type = params["input_type"];

    // This would integrate with gamepad-core to remap button
    result["button"] = button;
    result["input_type"] = input_type;
    result["remapped"] = true;
    result["success"] = true;

    return result;
}

nlohmann::json GamepadMCPServer::handle_start_mapping(const nlohmann::json& params) {
    nlohmann::json result;

    // This would integrate with gamepad-core to start mapping
    result["mapping_started"] = true;
    result["success"] = true;

    return result;
}

nlohmann::json GamepadMCPServer::handle_stop_mapping(const nlohmann::json& params) {
    nlohmann::json result;

    // This would integrate with gamepad-core to stop mapping
    result["mapping_stopped"] = true;
    result["success"] = true;

    return result;
}

nlohmann::json GamepadMCPServer::handle_enable_voice_alerts(const nlohmann::json& params) {
    nlohmann::json result;

    bool enable = params.value("enable", true);

    // This would integrate with gamepad-audio
    result["voice_alerts_enabled"] = enable;
    result["success"] = true;

    return result;
}

nlohmann::json GamepadMCPServer::handle_play_sound(const nlohmann::json& params) {
    nlohmann::json result;

    if (!params.contains("sound_name")) {
        result["error"] = "sound_name parameter required";
        result["success"] = false;
        return result;
    }

    std::string sound_name = params["sound_name"];

    // This would integrate with gamepad-audio
    result["sound_name"] = sound_name;
    result["played"] = true;
    result["success"] = true;

    return result;
}

nlohmann::json GamepadMCPServer::handle_scan_bluetooth(const nlohmann::json& params) {
    nlohmann::json result;

    // This would integrate with gamepad-bluetooth
    result["devices_found"] = 2;
    result["devices"] = nlohmann::json::array();
    result["devices"].push_back({
        {"name", "Xbox Wireless Controller"},
        {"address", "AA:BB:CC:DD:EE:FF"},
        {"rssi", -45}
    });
    result["devices"].push_back({
        {"name", "DualSense Wireless Controller"},
        {"address", "11:22:33:44:55:66"},
        {"rssi", -52}
    });
    result["success"] = true;

    return result;
}

nlohmann::json GamepadMCPServer::handle_connect_bluetooth(const nlohmann::json& params) {
    nlohmann::json result;

    if (!params.contains("device_address")) {
        result["error"] = "device_address parameter required";
        result["success"] = false;
        return result;
    }

    std::string device_address = params["device_address"];

    // This would integrate with gamepad-bluetooth
    result["device_address"] = device_address;
    result["connected"] = true;
    result["success"] = true;

    return result;
}

// Prompt handlers
nlohmann::json GamepadMCPServer::handle_setup_prompt(const nlohmann::json& params) {
    nlohmann::json result;

    std::string application = params.value("application", "gaming");

    result["prompt"] = "Setting up gamepad for " + application + " application. "
                      "I'll configure the optimal button mappings and sensitivity settings.";

    result["steps"] = nlohmann::json::array();
    result["steps"].push_back("Scan for connected gamepad devices");
    result["steps"].push_back("Load " + application + " mapping preset");
    result["steps"].push_back("Test button responsiveness");
    result["steps"].push_back("Adjust sensitivity if needed");

    return result;
}

nlohmann::json GamepadMCPServer::handle_gaming_optimization_prompt(const nlohmann::json& params) {
    nlohmann::json result;

    result["prompt"] = "Optimizing gamepad for gaming with low-latency input and "
                      "responsive controls. This will configure the controller for "
                      "maximum gaming performance.";

    result["optimizations"] = nlohmann::json::array();
    result["optimizations"].push_back("Enable UInput backend for lowest latency");
    result["optimizations"].push_back("Load gaming-optimized mapping preset");
    result["optimizations"].push_back("Configure rapid-fire settings");
    result["optimizations"].push_back("Set up macro support");

    return result;
}

nlohmann::json GamepadMCPServer::handle_accessibility_prompt(const nlohmann::json& params) {
    nlohmann::json result;

    result["prompt"] = "Setting up gamepad for accessibility features with large buttons, "
                      "voice feedback, and simplified controls.";

    result["features"] = nlohmann::json::array();
    result["features"].push_back("Enable voice alerts for all button presses");
    result["features"].push_back("Load accessibility mapping preset");
    result["features"].push_back("Configure large virtual buttons");
    result["features"].push_back("Set up keyboard navigation mode");

    return result;
}

} // namespace gamepad_mcp