#include <napi.h>
#include "discord_sdk.h"

// Wrapper class for N-API
class DiscordSocialSDK : public Napi::ObjectWrap<DiscordSocialSDK> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    DiscordSocialSDK(const Napi::CallbackInfo& info);

private:
    DiscordSDK::SocialSDK sdk_;

    Napi::Value GetUserInfo(const Napi::CallbackInfo& info);
    Napi::Value SendMessage(const Napi::CallbackInfo& info);
    Napi::Value GetOnlineUsers(const Napi::CallbackInfo& info);
};

Napi::Object DiscordSocialSDK::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "DiscordSocialSDK", {
        InstanceMethod("getUserInfo", &DiscordSocialSDK::GetUserInfo),
        InstanceMethod("sendMessage", &DiscordSocialSDK::SendMessage),
        InstanceMethod("getOnlineUsers", &DiscordSocialSDK::GetOnlineUsers)
    });

    Napi::FunctionReference* constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    env.SetInstanceData(constructor);

    exports.Set("DiscordSocialSDK", func);
    return exports;
}

DiscordSocialSDK::DiscordSocialSDK(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<DiscordSocialSDK>(info) {
    Napi::Env env = info.Env();

    if (info.Length() > 0 && !info[0].IsUndefined()) {
        // Optional: Handle constructor arguments
    }
}

Napi::Value DiscordSocialSDK::GetUserInfo(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string userId = info[0].As<Napi::String>().Utf8Value();
    std::string result = sdk_.getUserInfo(userId);

    return Napi::String::New(env, result);
}

Napi::Value DiscordSocialSDK::SendMessage(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 2 || !info[0].IsString() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Two strings expected").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string channelId = info[0].As<Napi::String>().Utf8Value();
    std::string message = info[1].As<Napi::String>().Utf8Value();
    bool result = sdk_.sendMessage(channelId, message);

    return Napi::Boolean::New(env, result);
}

Napi::Value DiscordSocialSDK::GetOnlineUsers(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    int result = sdk_.getOnlineUsers();
    return Napi::Number::New(env, result);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    return DiscordSocialSDK::Init(env, exports);
}

NODE_API_MODULE(discord_social_sdk, Init)
