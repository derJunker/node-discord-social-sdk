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
    int activityType; // 0 = Playing, 1 = Streaming, etc.
};

// Authorization result structure
struct AuthResult {
    bool success;
    std::string accessToken;
    std::string refreshToken;
    std::string error;
    int32_t expiresIn;
};

class DiscordSDK {
public:
    DiscordSDK();
    ~DiscordSDK();

    // Initialize the SDK with client ID
    bool Initialize(uint64_t clientId);
    
    // Shutdown the SDK
    void Shutdown();

    // Update rich presence activity
    bool UpdateActivity(const Activity& activity, std::string& error);

    // Clear the current activity
    bool ClearActivity(std::string& error);

    // Authorize user and get OAuth2 token
    void Authorize(std::function<void(const AuthResult&)> callback);

    // Run callbacks (should be called periodically)
    void RunCallbacks();

    // Check if SDK is initialized
    bool IsInitialized() const { return initialized_; }

private:
    bool initialized_;
    uint64_t clientId_;
    
#ifdef USE_DISCORD_SDK
    std::shared_ptr<discordpp::Client> client_;
#endif
};

} // namespace DiscordWrapper

#endif // DISCORD_WRAPPER_H
