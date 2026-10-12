# Gamepad MCP Server

Model Context Protocol (MCP) server for gamepad control and AI integration in the gamepad-mapper ecosystem.

## Overview

The Gamepad MCP Server provides a standardized interface for AI assistants to interact with gamepad devices and the gamepad-mapper system. It exposes gamepad functionality through MCP tools, resources, and prompts, enabling seamless integration with AI tools like Claude Desktop.

## Features

- **MCP Protocol Support**: Full implementation of Model Context Protocol
- **Gamepad Control Tools**: AI-accessible tools for device management and control
- **Resource Exposure**: Device status, mappings, and system information via MCP resources
- **Configuration Prompts**: Guided setup prompts for different use cases
- **Multi-Component Integration**: Bridges AI tools with gamepad ecosystem components
- **RESTful HTTP Interface**: HTTP/1.1 support with JSON-RPC 2.0
- **Server-Sent Events**: Real-time streaming for live updates

## MCP Tools Provided

### Device Management
- `gamepad_list_devices` - List all connected gamepad devices
- `gamepad_connect_device` - Connect to a specific gamepad device
- `gamepad_disconnect_device` - Disconnect a gamepad device

### Mapping Control
- `gamepad_load_mapping` - Load a gamepad mapping configuration
- `gamepad_save_mapping` - Save current mapping configuration
- `gamepad_get_mapping` - Get current mapping configuration
- `gamepad_remap_button` - Remap a gamepad button to keyboard/mouse input

### System Control
- `gamepad_start_mapping` - Start gamepad input mapping
- `gamepad_stop_mapping` - Stop gamepad input mapping

### Audio Integration
- `gamepad_enable_voice_alerts` - Enable/disable voice alerts
- `gamepad_play_sound` - Play a sound effect

### Bluetooth Support
- `gamepad_scan_bluetooth` - Scan for Bluetooth gamepad devices
- `gamepad_connect_bluetooth` - Connect to a Bluetooth gamepad

## MCP Resources

- `gamepad://devices` - Connected gamepad device information
- `gamepad://mappings` - Available mapping presets
- `gamepad://status` - System status and capabilities
- `gamepad://active-mapping` - Currently active mapping configuration

## MCP Prompts

- `gamepad_setup` - Guided setup for common applications
- `gamepad_gaming_optimization` - Optimize for gaming performance
- `gamepad_accessibility` - Configure for accessibility features

## Dependencies

- **TinyMCP**: MCP protocol implementation (included as submodule)
- **nlohmann_json**: JSON parsing and serialization
- **jsoncpp**: JSON processing
- **Boost**: Networking and utility libraries
- **OpenSSL**: Secure communications
- **C++17**: Modern C++ standard

## Building

### Using Conan

```bash
# Install dependencies
conan install . --build missing \
    -o with_gamepad_core=True \
    -o with_bluetooth=True \
    -o with_audio=True \
    -o with_config=True

# Build with CMake
cmake --preset conan-release
cmake --build build --config Release

# Install
cmake --install build
```

### Manual Build

```bash
# Install system dependencies
sudo apt-get install libboost-all-dev libssl-dev

# Build TinyMCP first
cd external/TinyMCP
mkdir build && cd build
cmake .. && make

# Build main project
cd ../../..
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
sudo make install
```

## Usage

### Starting the Server

```bash
# Basic startup
./gamepad-mcp-server-app

# With custom configuration
GAMEPAD_MCP_HOST=0.0.0.0 GAMEPAD_MCP_PORT=8080 ./gamepad-mcp-server-app
```

### Claude Desktop Integration

Add to your Claude Desktop configuration:

```json
{
  "mcpServers": {
    "gamepad": {
      "command": "gamepad-mcp-server-app",
      "env": {
        "GAMEPAD_MCP_HOST": "localhost",
        "GAMEPAD_MCP_PORT": "3001"
      }
    }
  }
}
```

### AI Interaction Examples

Once configured, you can interact with gamepads through natural language:

- "List all connected gamepad devices"
- "Load the gaming mapping preset"
- "Remap A button to spacebar"
- "Connect to my Xbox controller"
- "Enable voice alerts for button presses"
- "Scan for Bluetooth gamepads"

## Configuration

### Environment Variables

- `GAMEPAD_MCP_HOST`: Server host (default: "localhost")
- `GAMEPAD_MCP_PORT`: Server port (default: "3001")
- `GAMEPAD_LOG_LEVEL`: Logging level (default: "info")

### Configuration File

The server uses `mcp-config.json` for detailed configuration:

```json
{
  "gamepad": {
    "default_mapping": "gaming",
    "auto_connect_devices": true,
    "enable_bluetooth": true,
    "enable_voice_alerts": false
  },
  "integrations": {
    "gamepad_core_enabled": true,
    "bluetooth_enabled": true,
    "audio_enabled": true
  }
}
```

## API Reference

### HTTP Endpoints

- `POST /mcp` - MCP JSON-RPC endpoint
- `GET /health` - Health check endpoint
- `GET /status` - Server status information

### JSON-RPC Methods

All MCP tools are available as JSON-RPC methods:

```json
{
  "jsonrpc": "2.0",
  "id": 1,
  "method": "tools/call",
  "params": {
    "name": "gamepad_list_devices",
    "arguments": {}
  }
}
```

## Integration Architecture

The MCP server acts as the central hub for the gamepad ecosystem:

```
AI Assistant (Claude) ── MCP ── Gamepad MCP Server
                                      │
                    ┌─────────────────┼─────────────────┐
                    │                 │                 │
            gamepad-core     gamepad-bluetooth    gamepad-audio
                    │                 │                 │
            input-backends   gamepad-config    kde-integration
```

## Component Integration

The server integrates with other ecosystem components:

- **gamepad-core**: Device management and input processing
- **input-backends**: Platform-specific input simulation
- **gamepad-bluetooth**: Wireless device connectivity
- **gamepad-audio**: Voice alerts and sound effects
- **gamepad-config**: Configuration management
- **kde-integration**: Desktop environment features

## Testing

```bash
# Build with tests
cmake -DBUILD_TESTS=ON ..
make

# Run tests
ctest --output-on-failure

# Test MCP protocol
curl -X POST http://localhost:3001/mcp \
  -H "Content-Type: application/json" \
  -d '{"jsonrpc":"2.0","id":1,"method":"tools/list"}'
```

## Security Considerations

- **Local Access Only**: Server binds to localhost by default
- **No Authentication**: Designed for local AI assistant integration
- **Input Validation**: All inputs are validated before processing
- **Resource Limits**: Configurable limits on concurrent operations

## Troubleshooting

### Common Issues

1. **Port Already in Use**
   ```bash
   # Find process using port
   lsof -i :3001

   # Change port
   GAMEPAD_MCP_PORT=3002 ./gamepad-mcp-server-app
   ```

2. **Component Integration Failed**
   ```bash
   # Check component availability
   ls /usr/local/lib/libgamepad_core.so

   # Rebuild with integration disabled
   cmake .. -DWITH_GAMEPAD_CORE=OFF
   ```

3. **MCP Connection Failed**
   ```bash
   # Test server directly
   curl http://localhost:3001/health

   # Check Claude Desktop logs
   tail -f ~/.config/claude-desktop/logs/*.log
   ```

## Development

### Adding New MCP Tools

1. Implement tool handler in `GamepadMCPServer`
2. Register tool in `register_gamepad_tools()`
3. Add documentation to README

### Extending Resources

1. Add resource registration in `register_gamepad_resources()`
2. Implement dynamic content updates
3. Update API documentation

## Contributing

1. Fork the repository
2. Create a feature branch
3. Add tests for new functionality
4. Update documentation
5. Submit a pull request

## License

MIT License - see LICENSE file for details.

## Related Projects

- [gamepad-core](https://github.com/sparesparrow/gamepad-core) - Core mapping logic
- [TinyMCP](https://github.com/sparesparrow/TinyMCP) - MCP protocol implementation
- [MCPServer.cpp](https://github.com/sparesparrow/MCPServer.cpp) - C++ MCP server reference