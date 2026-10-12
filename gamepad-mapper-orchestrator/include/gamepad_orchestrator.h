#pragma once

#include <memory>
#include <string>
#include <vector>
#include <atomic>
#include <nlohmann/json.hpp>

namespace gamepad {

// Forward declarations for component interfaces
namespace core {
    class GamepadMapper;
    class ControllerManager;
    class MultiControllerManager;
}

namespace input_backends {
    class InputBackendManager;
}

namespace mcp {
    class GamepadMCPServer;
}

namespace bluetooth {
    class BluetoothScanner;
    class BluetoothConnector;
}

namespace audio {
    class AudioBackend;
    class VoiceAlerts;
}

namespace config {
    class ConfigManager;
    class PresetManager;
}

namespace kde {
    class KDEConnectClient;
}

/**
 * @brief Main orchestrator for the gamepad-mapper ecosystem
 *
 * Coordinates all components of the gamepad-mapper system including:
 * - Gamepad device management and mapping
 * - Input backend selection and simulation
 * - MCP server for AI integration
 * - Bluetooth device connectivity
 * - Audio feedback and voice alerts
 * - Configuration management
 * - KDE desktop integration
 */
class GamepadOrchestrator {
public:
    /**
     * @brief Orchestrator configuration
     */
    struct Config {
        // Core settings
        bool enable_mcp_server = true;
        bool enable_bluetooth = true;
        bool enable_audio = true;
        bool enable_kde_integration = false;

        // MCP server settings
        std::string mcp_host = "localhost";
        int mcp_port = 3001;

        // Audio settings
        bool enable_voice_alerts = false;
        float master_volume = 1.0f;

        // Configuration paths
        std::string config_dir = "/etc/gamepad-mapper";
        std::string preset_dir = "/usr/share/gamepad-mapper/presets";
        std::string log_file = "/var/log/gamepad-mapper.log";

        // Performance settings
        int max_devices = 4;
        int scan_interval_ms = 100;
    };

    /**
     * @brief Constructor
     */
    GamepadOrchestrator();

    /**
     * @brief Destructor
     */
    ~GamepadOrchestrator();

    /**
     * @brief Initialize the orchestrator with configuration
     * @param config Orchestrator configuration
     * @return true if initialization successful, false otherwise
     */
    bool initialize(const Config& config);

    /**
     * @brief Start all enabled components
     * @return true if started successfully, false otherwise
     */
    bool start();

    /**
     * @brief Stop all components
     */
    void stop();

    /**
     * @brief Check if orchestrator is running
     * @return true if running, false otherwise
     */
    bool is_running() const;

    /**
     * @brief Load a preset configuration
     * @param preset_name Name of the preset to load
     * @return true if loaded successfully, false otherwise
     */
    bool load_preset(const std::string& preset_name);

    /**
     * @brief Save current configuration as preset
     * @param preset_name Name to save preset as
     * @return true if saved successfully, false otherwise
     */
    bool save_preset(const std::string& preset_name);

    /**
     * @brief Scan for gamepad devices
     * @return Number of devices found
     */
    int scan_devices();

    /**
     * @brief Get system status as JSON
     * @return System status information
     */
    nlohmann::json get_system_status() const;

    /**
     * @brief Enable/disable voice alerts
     * @param enable true to enable, false to disable
     * @return true if operation successful, false otherwise
     */
    bool set_voice_alerts_enabled(bool enable);

    /**
     * @brief Set master volume
     * @param volume Volume level (0.0 to 1.0)
     * @return true if set successfully, false otherwise
     */
    bool set_master_volume(float volume);

    /**
     * @brief Get last error message
     * @return Last error message
     */
    std::string get_last_error() const;

    /**
     * @brief Get orchestrator version
     * @return Version string
     */
    static std::string get_version();

private:
    Config config_;
    std::atomic<bool> running_{false};
    std::string last_error_;

    // Component pointers (will be initialized based on configuration)
    std::unique_ptr<core::GamepadMapper> gamepad_mapper_;
    std::unique_ptr<core::ControllerManager> controller_manager_;
    std::unique_ptr<core::MultiControllerManager> multi_controller_manager_;
    std::unique_ptr<input_backends::InputBackendManager> input_backend_manager_;
    std::unique_ptr<mcp::GamepadMCPServer> mcp_server_;
    std::unique_ptr<bluetooth::BluetoothScanner> bluetooth_scanner_;
    std::unique_ptr<bluetooth::BluetoothConnector> bluetooth_connector_;
    std::unique_ptr<audio::AudioBackend> audio_backend_;
    std::unique_ptr<audio::VoiceAlerts> voice_alerts_;
    std::unique_ptr<config::ConfigManager> config_manager_;
    std::unique_ptr<config::PresetManager> preset_manager_;
    std::unique_ptr<kde::KDEConnectClient> kde_client_;

    /**
     * @brief Initialize core components
     * @return true if successful, false otherwise
     */
    bool initialize_core_components();

    /**
     * @brief Initialize input backends
     * @return true if successful, false otherwise
     */
    bool initialize_input_backends();

    /**
     * @brief Initialize MCP server if enabled
     * @return true if successful, false otherwise
     */
    bool initialize_mcp_server();

    /**
     * @brief Initialize Bluetooth components if enabled
     * @return true if successful, false otherwise
     */
    bool initialize_bluetooth();

    /**
     * @brief Initialize audio components if enabled
     * @return true if successful, false otherwise
     */
    bool initialize_audio();

    /**
     * @brief Initialize configuration management
     * @return true if successful, false otherwise
     */
    bool initialize_config_management();

    /**
     * @brief Initialize KDE integration if enabled
     * @return true if successful, false otherwise
     */
    bool initialize_kde_integration();

    /**
     * @brief Set last error message
     * @param error Error message
     */
    void set_error(const std::string& error);

    /**
     * @brief Check component availability
     * @param component_name Name of component to check
     * @return true if available, false otherwise
     */
    bool is_component_available(const std::string& component_name) const;

    /**
     * @brief Load default configuration if none exists
     * @return true if loaded successfully, false otherwise
     */
    bool load_default_config();

    /**
     * @brief Connect component signals/callbacks
     */
    void connect_component_signals();

    /**
     * @brief Start background monitoring tasks
     */
    void start_monitoring_tasks();

    /**
     * @brief Stop background monitoring tasks
     */
    void stop_monitoring_tasks();
};

// Global orchestrator instance for easy access
extern std::unique_ptr<GamepadOrchestrator> g_orchestrator;

} // namespace gamepad