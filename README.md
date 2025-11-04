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

## Usage

### JavaScript

```javascript
const { DiscordSocialSDK } = require('node-discord-social-sdk');

const sdk = new DiscordSocialSDK();

// Get user information
const userInfo = sdk.getUserInfo('user123');
console.log(userInfo);

// Send a message
const success = sdk.sendMessage('channel456', 'Hello World!');
console.log('Message sent:', success);

// Get online users count
const onlineUsers = sdk.getOnlineUsers();
console.log('Online users:', onlineUsers);
```

### TypeScript

```typescript
import { DiscordSocialSDK } from 'node-discord-social-sdk';

const sdk = new DiscordSocialSDK();

// Get user information
const userInfo: string = sdk.getUserInfo('user123');
console.log(userInfo);

// Send a message
const success: boolean = sdk.sendMessage('channel456', 'Hello World!');
console.log('Message sent:', success);

// Get online users count
const onlineUsers: number = sdk.getOnlineUsers();
console.log('Online users:', onlineUsers);
```

## API

### `DiscordSocialSDK`

#### `getUserInfo(userId: string): string`

Get user information by user ID.

- **Parameters:**
  - `userId` (string): The Discord user ID
- **Returns:** User information string

#### `sendMessage(channelId: string, message: string): boolean`

Send a message to a Discord channel.

- **Parameters:**
  - `channelId` (string): The Discord channel ID
  - `message` (string): The message to send
- **Returns:** `true` if message was sent successfully

#### `getOnlineUsers(): number`

Get the number of online users.

- **Returns:** The count of online users

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