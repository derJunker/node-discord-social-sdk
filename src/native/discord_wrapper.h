#ifndef DISCORD_WRAPPER_H
#define DISCORD_WRAPPER_H

#include <string>
#include <functional>
#include <memory>

#ifdef USE_DISCORD_SDK
#include "discordpp.h"
#endif

namespace DiscordWrapper {

// Activity data structure
struct Activity {
    std::string state;
    std::string details;
    std::string largeImageKey;
    std::string largeImageText;
    std::string smallImageKey;
    std::string smallImageText;
    int64_t startTimestamp;
    int64_t endTimestamp;
    std::string partyId;
    int partySize;
    int partyMax;
};

// Authorization result structure
struct AuthResult {
    bool success;
    std::string accessToken;
    std::string error;
};

class DiscordSDK {
public:
    DiscordSDK();
    ~DiscordSDK();

    // Initialize the SDK
    bool Initialize(int64_t clientId);
    
    // Shutdown the SDK
    void Shutdown();

    // Update rich presence activity
    bool UpdateActivity(const Activity& activity, std::string& error);

    // Clear the current activity
    bool ClearActivity(std::string& error);

    // Authorize user and get OAuth2 token
    void Authorize(const std::string& scopes, std::function<void(const AuthResult&)> callback);

    // Run callbacks (should be called periodically)
    void RunCallbacks();

    // Check if SDK is initialized
    bool IsInitialized() const { return initialized_; }

private:
    bool initialized_;
    
#ifdef USE_DISCORD_SDK
    std::unique_ptr<discord::Core> core_;
#endif
};

} // namespace DiscordWrapper

#endif // DISCORD_WRAPPER_H
