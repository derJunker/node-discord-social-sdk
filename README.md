# node-discord-social-sdk

A Node.js native addon built with CMake and N-API for Discord social SDK integration. This project demonstrates how to create a C++ native module that can be accessed from JavaScript/TypeScript.

## Features

- ✅ CMake-based build system
- ✅ N-API for stable ABI across Node.js versions
- ✅ Full TypeScript support with type definitions
- ✅ Cross-platform compatibility
- ✅ Object-oriented API design

## Prerequisites

- Node.js (v14 or higher)
- CMake (v3.15 or higher)
- C++ compiler (GCC, Clang, or MSVC)
- Python (for node-gyp)

## Installation

```bash
npm install
```

This will automatically build the native addon using CMake.

## Building

To build the project:

```bash
# Build native addon and TypeScript
npm run build

# Build only native addon
npm run build:native

# Build only TypeScript
npm run build:ts

# Clean build artifacts
npm run clean
```

## Setup

### Discord SDK Integration

To use the full Discord Social SDK features, you need to:

1. Download the Discord Social SDK from Discord's developer portal
2. Extract it into the `lib/discord_social_sdk` directory with the following structure:
   ```
   lib/
   └── discord_social_sdk/
       ├── include/
       │   └── discordpp.h
       ├── lib/
       │   └── release/
       │       └── (platform-specific libraries)
       └── bin/
           └── release/
               └── (platform-specific binaries)
   ```

The project will build with or without the Discord SDK. If the SDK is not found, it will build without Discord integration.

## Usage

### JavaScript

```javascript
const { DiscordSocialSDK } = require('node-discord-social-sdk');

const sdk = new DiscordSocialSDK();

// Initialize with your Discord application's client ID
const clientId = 123456789012345678; // Replace with your client ID
if (sdk.initialize(clientId)) {
    console.log('Discord SDK initialized!');

    // Update rich presence
    sdk.updateActivity({
        state: 'Playing Solo',
        details: 'In a match',
        largeImageKey: 'game_logo',
        largeImageText: 'My Game',
        startTimestamp: Date.now()
    });

    // Clear activity
    sdk.clearActivity();

    // Authorize user
    sdk.authorize('identify email', (result) => {
        if (result.success) {
            console.log('Access Token:', result.accessToken);
        } else {
            console.error('Auth failed:', result.error);
        }
    });

    // Run callbacks periodically (e.g., in game loop)
    setInterval(() => {
        sdk.runCallbacks();
    }, 100);

    // Cleanup on exit
    process.on('exit', () => {
        sdk.shutdown();
    });
}
```

### TypeScript

```typescript
import { DiscordSocialSDK, Activity, AuthResult } from 'node-discord-social-sdk';

const sdk: DiscordSocialSDK = new DiscordSocialSDK();

// Initialize with your Discord application's client ID
const clientId: number = 123456789012345678; // Replace with your client ID
if (sdk.initialize(clientId)) {
    console.log('Discord SDK initialized!');

    // Update rich presence with type safety
    const activity: Activity = {
        state: 'Playing Solo',
        details: 'In a match',
        largeImageKey: 'game_logo',
        largeImageText: 'My Game',
        startTimestamp: Date.now(),
        partyId: 'party123',
        partySize: 1,
        partyMax: 4
    };
    sdk.updateActivity(activity);

    // Clear activity
    sdk.clearActivity();

    // Authorize user with callback
    sdk.authorize('identify email', (result: AuthResult) => {
        if (result.success) {
            console.log('Access Token:', result.accessToken);
        } else {
            console.error('Auth failed:', result.error);
        }
    });

    // Run callbacks periodically
    setInterval(() => {
        sdk.runCallbacks();
    }, 100);

    // Cleanup on exit
    process.on('exit', () => {
        sdk.shutdown();
    });
}
```

## API

### `DiscordSocialSDK`

#### `initialize(clientId: number): boolean`

Initialize the Discord SDK with your application's client ID.

- **Parameters:**
  - `clientId` (number): Your Discord application's client ID
- **Returns:** `true` if initialization was successful

#### `shutdown(): void`

Shutdown the Discord SDK and clean up resources.

#### `updateActivity(activity: Activity): boolean`

Update the rich presence activity displayed on Discord.

- **Parameters:**
  - `activity` (Activity): Activity object with the following optional properties:
    - `state` (string): The user's current party status
    - `details` (string): What the player is currently doing
    - `largeImageKey` (string): Name of the large image asset
    - `largeImageText` (string): Text displayed when hovering over large image
    - `smallImageKey` (string): Name of the small image asset
    - `smallImageText` (string): Text displayed when hovering over small image
    - `startTimestamp` (number): Unix timestamp (ms) for start of activity
    - `endTimestamp` (number): Unix timestamp (ms) for end of activity
    - `partyId` (string): ID of the player's party
    - `partySize` (number): Current size of the party
    - `partyMax` (number): Maximum size of the party
- **Returns:** `true` if activity was updated successfully

#### `clearActivity(): boolean`

Clear the current rich presence activity.

- **Returns:** `true` if activity was cleared successfully

#### `authorize(scopes: string, callback: (result: AuthResult) => void): void`

Authorize the user and get an OAuth2 token.

- **Parameters:**
  - `scopes` (string): Space-separated OAuth2 scopes (e.g., "identify email")
  - `callback` (function): Callback function that receives an AuthResult object with:
    - `success` (boolean): Whether authorization was successful
    - `accessToken` (string): The OAuth2 access token (if successful)
    - `error` (string): Error message (if failed)

#### `runCallbacks(): void`

Run Discord SDK callbacks. This should be called periodically (e.g., every 100ms) to process SDK events.

#### `isInitialized(): boolean`

Check if the SDK is initialized.

- **Returns:** `true` if the SDK is initialized

## Testing

```bash
npm test
```

## Project Structure

```
.
├── CMakeLists.txt           # CMake build configuration
├── package.json             # Node.js package configuration
├── tsconfig.json           # TypeScript configuration
├── src/
│   ├── native/             # C++ source files
│   │   ├── addon.cpp       # N-API bindings
│   │   ├── discord_sdk.h   # SDK header
│   │   └── discord_sdk.cpp # SDK implementation
│   └── typescript/         # TypeScript source files
│       └── index.ts        # TypeScript wrapper
├── test/
│   └── test.js            # Test file
└── dist/                   # Compiled TypeScript output
```

## Development

The native C++ code is located in `src/native/` and the TypeScript wrapper is in `src/typescript/`. The build process:

1. CMake compiles the C++ code into a `.node` binary
2. TypeScript is compiled to JavaScript in the `dist/` folder
3. The TypeScript wrapper loads the native binary and provides a typed interface

## License

MIT