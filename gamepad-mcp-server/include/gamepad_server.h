#pragma once

#include "mcp_server.h"
#include <memory>
#include <nlohmann/json.hpp>
#include <string>

namespace gamepad_mcp {

/**
 * @brief Gamepad MCP Server
 *
 * Provides MCP (Model Context Protocol) interface for gamepad control and integration.
 * This server exposes gamepad functionality through MCP tools, resources, and prompts
 * for seamless integration with AI assistants and automation systems.
 */
class GamepadMCPServer {
public:
    /**
     * @brief Constructor
     */
    GamepadMCPServer();

    /**
     * @brief Destructor
     */
    ~GamepadMCPServer();

    /**
     * @brief Initialize the MCP server
     * @param config Server configuration
     * @return true if initialization successful, false otherwise
     */
    bool initialize(const ServerConfig& config);

    /**
     * @brief Start the MCP server
     * @return true if server started successfully, false otherwise
     */
    bool start();

    /**
     * @brief Stop the MCP server
     */
    void stop();

    /**
     * @brief Check if server is running
     * @return true if running, false otherwise
     */
    bool is_running() const;

    /**
     * @brief Get server information
     * @return Server information structure
     */
    ServerInfo get_server_info() const;

private:
    /**
     * @brief Register gamepad-specific MCP tools
     * @return true if registration successful, false otherwise
     */
    bool register_gamepad_tools();

    /**
     * @brief Register gamepad-related MCP resources
     * @return true if registration successful, false otherwise
     */
    bool register_gamepad_resources();

    /**
     * @brief Register gamepad configuration prompts
     * @return true if registration successful, false otherwise
     */
    bool register_gamepad_prompts();

    // Tool handlers
    nlohmann::json handle_list_devices(const nlohmann::json& params);
    nlohmann::json handle_connect_device(const nlohmann::json& params);
    nlohmann::json handle_disconnect_device(const nlohmann::json& params);
    nlohmann::json handle_load_mapping(const nlohmann::json& params);
    nlohmann::json handle_save_mapping(const nlohmann::json& params);
    nlohmann::json handle_get_mapping(const nlohmann::json& params);
    nlohmann::json handle_remap_button(const nlohmann::json& params);
    nlohmann::json handle_start_mapping(const nlohmann::json& params);
    nlohmann::json handle_stop_mapping(const nlohmann::json& params);
    nlohmann::json handle_enable_voice_alerts(const nlohmann::json& params);
    nlohmann::json handle_play_sound(const nlohmann::json& params);
    nlohmann::json handle_scan_bluetooth(const nlohmann::json& params);
    nlohmann::json handle_connect_bluetooth(const nlohmann::json& params);

    // Prompt handlers
    nlohmann::json handle_setup_prompt(const nlohmann::json& params);
    nlohmann::json handle_gaming_optimization_prompt(const nlohmann::json& params);
    nlohmann::json handle_accessibility_prompt(const nlohmann::json& params);

    std::unique_ptr<MCPServer> mcp_server_;
    ServerConfig server_config_;
};

// Convenience typedef
using GamepadServer = GamepadMCPServer;

} // namespace gamepad_mcp