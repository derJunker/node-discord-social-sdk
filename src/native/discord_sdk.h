#ifndef DISCORD_SDK_H
#define DISCORD_SDK_H

#include <string>

namespace DiscordSDK {

class SocialSDK {
public:
    SocialSDK();
    ~SocialSDK();

    // Example methods
    std::string getUserInfo(const std::string& userId);
    bool sendMessage(const std::string& channelId, const std::string& message);
    int getOnlineUsers();
};

} // namespace DiscordSDK

#endif // DISCORD_SDK_H
