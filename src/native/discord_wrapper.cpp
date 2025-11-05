#include "discord_wrapper.h"
#include <iostream>

namespace DiscordWrapper {

DiscordSDK::DiscordSDK() : initialized_(false) {
}

DiscordSDK::~DiscordSDK() {
    Shutdown();
}

bool DiscordSDK::Initialize(int64_t clientId) {
#ifdef USE_DISCORD_SDK
    try {
        auto result = discord::Core::Create(clientId, DiscordCreateFlags_Default, &core_);
        if (result != discord::Result::Ok) {
            std::cerr << "Failed to create Discord Core: " << static_cast<int>(result) << std::endl;
            return false;
        }
        initialized_ = true;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Exception during Discord SDK initialization: " << e.what() << std::endl;
        return false;
    }
#else
    std::cerr << "Discord SDK not available. Built without SDK support." << std::endl;
    return false;
#endif
}

void DiscordSDK::Shutdown() {
    if (initialized_) {
#ifdef USE_DISCORD_SDK
        core_.reset();
#endif
        initialized_ = false;
    }
}

bool DiscordSDK::UpdateActivity(const Activity& activity, std::string& error) {
#ifdef USE_DISCORD_SDK
    if (!initialized_) {
        error = "Discord SDK not initialized";
        return false;
    }

    try {
        discord::Activity discordActivity{};
        
        // Set activity details
        if (!activity.state.empty()) {
            discordActivity.SetState(activity.state.c_str());
        }
        if (!activity.details.empty()) {
            discordActivity.SetDetails(activity.details.c_str());
        }

        // Set timestamps
        if (activity.startTimestamp > 0 || activity.endTimestamp > 0) {
            auto& timestamps = discordActivity.GetTimestamps();
            if (activity.startTimestamp > 0) {
                timestamps.SetStart(activity.startTimestamp);
            }
            if (activity.endTimestamp > 0) {
                timestamps.SetEnd(activity.endTimestamp);
            }
        }

        // Set assets (images)
        if (!activity.largeImageKey.empty() || !activity.smallImageKey.empty()) {
            auto& assets = discordActivity.GetAssets();
            if (!activity.largeImageKey.empty()) {
                assets.SetLargeImage(activity.largeImageKey.c_str());
            }
            if (!activity.largeImageText.empty()) {
                assets.SetLargeText(activity.largeImageText.c_str());
            }
            if (!activity.smallImageKey.empty()) {
                assets.SetSmallImage(activity.smallImageKey.c_str());
            }
            if (!activity.smallImageText.empty()) {
                assets.SetSmallText(activity.smallImageText.c_str());
            }
        }

        // Set party info
        if (!activity.partyId.empty() && activity.partySize > 0) {
            auto& party = discordActivity.GetParty();
            party.GetId() = activity.partyId.c_str();
            auto& size = party.GetSize();
            size.SetCurrentSize(activity.partySize);
            if (activity.partyMax > 0) {
                size.SetMaxSize(activity.partyMax);
            }
        }

        // Update the activity
        core_->ActivityManager().UpdateActivity(discordActivity, [&error](discord::Result result) {
            if (result != discord::Result::Ok) {
                error = "Failed to update activity: " + std::to_string(static_cast<int>(result));
            }
        });

        return true;
    } catch (const std::exception& e) {
        error = std::string("Exception during activity update: ") + e.what();
        return false;
    }
#else
    error = "Discord SDK not available";
    return false;
#endif
}

bool DiscordSDK::ClearActivity(std::string& error) {
#ifdef USE_DISCORD_SDK
    if (!initialized_) {
        error = "Discord SDK not initialized";
        return false;
    }

    try {
        core_->ActivityManager().ClearActivity([&error](discord::Result result) {
            if (result != discord::Result::Ok) {
                error = "Failed to clear activity: " + std::to_string(static_cast<int>(result));
            }
        });
        return true;
    } catch (const std::exception& e) {
        error = std::string("Exception during activity clear: ") + e.what();
        return false;
    }
#else
    error = "Discord SDK not available";
    return false;
#endif
}

void DiscordSDK::Authorize(const std::string& scopes, std::function<void(const AuthResult&)> callback) {
#ifdef USE_DISCORD_SDK
    if (!initialized_) {
        AuthResult result;
        result.success = false;
        result.error = "Discord SDK not initialized";
        callback(result);
        return;
    }

    try {
        core_->UserManager().OnOAuth2Token.Connect([callback](discord::Result result, discord::OAuth2Token const& token) {
            AuthResult authResult;
            if (result == discord::Result::Ok) {
                authResult.success = true;
                authResult.accessToken = token.GetAccessToken();
            } else {
                authResult.success = false;
                authResult.error = "Authorization failed: " + std::to_string(static_cast<int>(result));
            }
            callback(authResult);
        });

        core_->UserManager().GetOAuth2Token();
    } catch (const std::exception& e) {
        AuthResult result;
        result.success = false;
        result.error = std::string("Exception during authorization: ") + e.what();
        callback(result);
    }
#else
    AuthResult result;
    result.success = false;
    result.error = "Discord SDK not available";
    callback(result);
#endif
}

void DiscordSDK::RunCallbacks() {
#ifdef USE_DISCORD_SDK
    if (initialized_ && core_) {
        core_->RunCallbacks();
    }
#endif
}

} // namespace DiscordWrapper
