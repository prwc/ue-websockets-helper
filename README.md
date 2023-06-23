# Unreal Engine WebSockets Plugin

![Unreal Engine version](https://img.shields.io/badge/Unreal%20Engine-4.x-orange)
![Plugin version](https://img.shields.io/badge/Plugin%20version-1.0-blue)

This Unreal Engine plugin provides a convenient wrapper around the WebSockets module, making it easier to use and integrate WebSockets functionality into your projects. It includes a WebSocketsSubsystem that handles IWebSocket operations and offers Blueprint support.

## Features

- Easy-to-use wrapper for the WebSockets module
- Blueprint support for seamless integration into your Blueprint scripts
- Handles WebSocket connections, messaging, and events

## Requirements

- Unreal Engine 4.x or later

## Installation

1. Download the latest release of the plugin from the [Releases](https://github.com/your-repository/releases) page.
2. Extract the downloaded ZIP file.
3. Copy the `WebSocketsPlugin` folder into your Unreal Engine project's `Plugins` directory.
4. Launch Unreal Engine and open your project.
5. Enable plugin in `Plugins` Setting and restart the editor.

## Usage

### Setting up a WebSocket connection

1. Open the Blueprint editor.
2. Drag and drop the `WebSocketsSubsystem` into your Blueprint graph.
3. Configure the WebSocket URL and any desired options.
4. Call the `Connect` function on the `WebSocketsSubsystem` node.