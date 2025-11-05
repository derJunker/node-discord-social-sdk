#ifdef USE_DISCORD_SDK
#define DISCORDPP_IMPLEMENTATION
#endif

#include "discord_wrapper.h"
#include <iostream>

namespace DiscordWrapper {

DiscordSDK::DiscordSDK() : initialized_(false), clientId_(0) {
}

DiscordSDK::~DiscordSDK() {
    Shutdown();
}

bool DiscordSDK::Initialize(const std::string& clientId) {
#ifdef USE_DISCORD_SDK
    try {
        // Convert clientId string to uint64_t
        clientId_ = std::stoull(clientId);
        client_ = std::make_shared<discordpp::Client>();
        
        // Add log callback
        client_->AddLogCallback([](auto message, auto severity) {
            std::cout << "[Discord SDK] " << message << std::endl;
        }, discordpp::LoggingSeverity::Info);
        
        // Set status changed callback
        client_->SetStatusChangedCallback([](discordpp::Client::Status status, discordpp::Client::Error error, int32_t errorDetail) {
            if (status == discordpp::Client::Status::Ready) {
                std::cout << "[Discord SDK] Client is ready!" << std::endl;
            } else if (error != discordpp::Client::Error::None) {
                std::cerr << "[Discord SDK] Connection Error: " << discordpp::Client::ErrorToString(error) << std::endl;
            }
        });
        
        initialized_ = true;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "[Discord SDK] Exception during initialization: " << e.what() << std::endl;
        return false;
    }
#else
    std::cerr << "[Discord SDK] Not available. Built without SDK support." << std::endl;
    return false;
#endif
}

void DiscordSDK::Shutdown() {
    if (initialized_) {
#ifdef USE_DISCORD_SDK
        client_.reset();
#endif
        initialized_ = false;
    }
}

bool DiscordSDK::UpdateActivity(const Activity& activity, std::string& error) {
#ifdef USE_DISCORD_SDK
    if (!initialized_ || !client_) {
        error = "Discord SDK not initialized";
        return false;
    }

    try {
        discordpp::Activity discordActivity;
        
        // Set activity type (default to Playing)
        discordActivity.SetType(static_cast<discordpp::ActivityTypes>(activity.activityType));
        
        // Set activity details
        if (!activity.state.empty()) {
            discordActivity.SetState(activity.state);
        }
        if (!activity.details.empty()) {
            discordActivity.SetDetails(activity.details);
        }

        // Update rich presence
        client_->UpdateRichPresence(discordActivity, [&error](discordpp::ClientResult result) {
            if (!result.Successful()) {
                error = "Failed to update rich presence";
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
    if (!initialized_ || !client_) {
        error = "Discord SDK not initialized";
        return false;
    }

    try {
        // Clear by setting empty activity
        discordpp::Activity emptyActivity;
        client_->UpdateRichPresence(emptyActivity, [&error](discordpp::ClientResult result) {
            if (!result.Successful()) {
                error = "Failed to clear activity";
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

void DiscordSDK::Authorize(std::function<void(const AuthResult&)> callback) {
#ifdef USE_DISCORD_SDK
    if (!initialized_ || !client_) {
        AuthResult result;
        result.success = false;
        result.error = "Discord SDK not initialized";
        callback(result);
        return;
    }

    try {
        // Generate OAuth2 code verifier
        auto codeVerifier = client_->CreateAuthorizationCodeVerifier();
        
        // Set up authorization arguments
        discordpp::AuthorizationArgs args{};
        args.SetClientId(clientId_);
        args.SetScopes(discordpp::Client::GetDefaultPresenceScopes());
        args.SetCodeChallenge(codeVerifier.Challenge());
        
        // Begin authentication process
        client_->Authorize(args, [this, callback, codeVerifier](auto result, auto code, auto redirectUri) {
            if (!result.Successful()) {
                AuthResult authResult;
                authResult.success = false;
                authResult.error = "Authorization failed";
                callback(authResult);
                return;
            }
            
            // Exchange auth code for access token
            client_->GetToken(clientId_, code, codeVerifier.Verifier(), redirectUri,
                [this, callback](discordpp::ClientResult result,
                std::string accessToken,
                std::string refreshToken,
                discordpp::AuthorizationTokenType tokenType,
                int32_t expiresIn,
                std::string scope) {
                    
                    if (!result.Successful()) {
                        AuthResult authResult;
                        authResult.success = false;
                        authResult.error = "Failed to get token";
                        callback(authResult);
                        return;
                    }
                    
                    // Update the token and connect
                    client_->UpdateToken(tokenType, accessToken, [this, callback, accessToken, refreshToken, expiresIn](discordpp::ClientResult result) {
                        if (result.Successful()) {
                            client_->Connect();
                            
                            AuthResult authResult;
                            authResult.success = true;
                            authResult.accessToken = accessToken;
                            authResult.refreshToken = refreshToken;
                            authResult.expiresIn = expiresIn;
                            callback(authResult);
                        } else {
                            AuthResult authResult;
                            authResult.success = false;
                            authResult.error = "Failed to update token";
                            callback(authResult);
                        }
                    });
                });
        });
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
    discordpp::RunCallbacks();
#endif
}

} // namespace DiscordWrapper
