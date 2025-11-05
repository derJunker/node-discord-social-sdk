# Discord Social SDK Integration

This directory is where you should place the Discord Social SDK files.

## Setup Instructions

1. Download the Discord Social SDK from Discord's developer portal
2. Extract the SDK archive
3. Place the extracted `discord_social_sdk` folder in this directory

## Expected Structure

After setup, your directory structure should look like:

```
lib/
├── README.md (this file)
└── discord_social_sdk/
    ├── include/
    │   └── discordpp.h
    ├── lib/
    │   └── release/
    │       ├── discord_partner_sdk.lib (Windows)
    │       ├── libdiscord_partner_sdk.so (Linux)
    │       └── libdiscord_partner_sdk.dylib (macOS)
    └── bin/
        └── release/
            ├── discord_partner_sdk.dll (Windows)
            ├── libdiscord_partner_sdk.so (Linux)
            └── libdiscord_partner_sdk.dylib (macOS)
```

## Platform-Specific Notes

### Windows
- Requires: `discord_partner_sdk.lib` and `discord_partner_sdk.dll`

### Linux
- Requires: `libdiscord_partner_sdk.so`

### macOS
- Requires: `libdiscord_partner_sdk.dylib`

## Building Without the SDK

If you don't have the Discord Social SDK, the project will still build but without Discord integration features. The SDK methods will return errors indicating that the SDK is not available.

## Getting the SDK

Contact Discord or your Discord partner manager to obtain access to the Discord Social SDK.
