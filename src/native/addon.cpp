#include <napi.h>
#include "discord_wrapper.h"
#include <memory>

// Wrapper class for N-API
class DiscordSocialSDK : public Napi::ObjectWrap<DiscordSocialSDK> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    DiscordSocialSDK(const Napi::CallbackInfo& info);

private:
    std::unique_ptr<DiscordWrapper::DiscordSDK> sdk_;
    Napi::FunctionReference authCallback_;

    Napi::Value Initialize(const Napi::CallbackInfo& info);
    Napi::Value Shutdown(const Napi::CallbackInfo& info);
    Napi::Value UpdateActivity(const Napi::CallbackInfo& info);
    Napi::Value ClearActivity(const Napi::CallbackInfo& info);
    Napi::Value Authorize(const Napi::CallbackInfo& info);
    Napi::Value RunCallbacks(const Napi::CallbackInfo& info);
    Napi::Value IsInitialized(const Napi::CallbackInfo& info);
};

Napi::Object DiscordSocialSDK::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "DiscordSocialSDK", {
        InstanceMethod("initialize", &DiscordSocialSDK::Initialize),
        InstanceMethod("shutdown", &DiscordSocialSDK::Shutdown),
        InstanceMethod("updateActivity", &DiscordSocialSDK::UpdateActivity),
        InstanceMethod("clearActivity", &DiscordSocialSDK::ClearActivity),
        InstanceMethod("authorize", &DiscordSocialSDK::Authorize),
        InstanceMethod("runCallbacks", &DiscordSocialSDK::RunCallbacks),
        InstanceMethod("isInitialized", &DiscordSocialSDK::IsInitialized)
    });

    Napi::FunctionReference* constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    env.SetInstanceData(constructor);

    exports.Set("DiscordSocialSDK", func);
    return exports;
}

DiscordSocialSDK::DiscordSocialSDK(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<DiscordSocialSDK>(info) {
    sdk_ = std::make_unique<DiscordWrapper::DiscordSDK>();
}

Napi::Value DiscordSocialSDK::Initialize(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsNumber()) {
        Napi::TypeError::New(env, "Client ID (number) expected").ThrowAsJavaScriptException();
        return env.Null();
    }

    int64_t clientId = info[0].As<Napi::Number>().Int64Value();
    bool success = sdk_->Initialize(clientId);

    return Napi::Boolean::New(env, success);
}

Napi::Value DiscordSocialSDK::Shutdown(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    sdk_->Shutdown();
    return env.Undefined();
}

Napi::Value DiscordSocialSDK::UpdateActivity(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsObject()) {
        Napi::TypeError::New(env, "Activity object expected").ThrowAsJavaScriptException();
        return env.Null();
    }

    Napi::Object activityObj = info[0].As<Napi::Object>();
    DiscordWrapper::Activity activity{};

    // Parse activity object
    if (activityObj.Has("state")) {
        activity.state = activityObj.Get("state").As<Napi::String>().Utf8Value();
    }
    if (activityObj.Has("details")) {
        activity.details = activityObj.Get("details").As<Napi::String>().Utf8Value();
    }
    if (activityObj.Has("largeImageKey")) {
        activity.largeImageKey = activityObj.Get("largeImageKey").As<Napi::String>().Utf8Value();
    }
    if (activityObj.Has("largeImageText")) {
        activity.largeImageText = activityObj.Get("largeImageText").As<Napi::String>().Utf8Value();
    }
    if (activityObj.Has("smallImageKey")) {
        activity.smallImageKey = activityObj.Get("smallImageKey").As<Napi::String>().Utf8Value();
    }
    if (activityObj.Has("smallImageText")) {
        activity.smallImageText = activityObj.Get("smallImageText").As<Napi::String>().Utf8Value();
    }
    if (activityObj.Has("startTimestamp")) {
        activity.startTimestamp = activityObj.Get("startTimestamp").As<Napi::Number>().Int64Value();
    }
    if (activityObj.Has("endTimestamp")) {
        activity.endTimestamp = activityObj.Get("endTimestamp").As<Napi::Number>().Int64Value();
    }
    if (activityObj.Has("partyId")) {
        activity.partyId = activityObj.Get("partyId").As<Napi::String>().Utf8Value();
    }
    if (activityObj.Has("partySize")) {
        activity.partySize = activityObj.Get("partySize").As<Napi::Number>().Int32Value();
    }
    if (activityObj.Has("partyMax")) {
        activity.partyMax = activityObj.Get("partyMax").As<Napi::Number>().Int32Value();
    }

    std::string error;
    bool success = sdk_->UpdateActivity(activity, error);

    if (!success && !error.empty()) {
        Napi::Error::New(env, error).ThrowAsJavaScriptException();
        return env.Null();
    }

    return Napi::Boolean::New(env, success);
}

Napi::Value DiscordSocialSDK::ClearActivity(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    std::string error;
    bool success = sdk_->ClearActivity(error);

    if (!success && !error.empty()) {
        Napi::Error::New(env, error).ThrowAsJavaScriptException();
        return env.Null();
    }

    return Napi::Boolean::New(env, success);
}

Napi::Value DiscordSocialSDK::Authorize(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 2 || !info[0].IsString() || !info[1].IsFunction()) {
        Napi::TypeError::New(env, "Expected (scopes: string, callback: function)").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string scopes = info[0].As<Napi::String>().Utf8Value();
    Napi::Function callback = info[1].As<Napi::Function>();
    
    // Store the callback
    authCallback_ = Napi::Persistent(callback);

    // Create a thread-safe function for the callback
    auto tsfn = Napi::ThreadSafeFunction::New(
        env,
        callback,
        "AuthCallback",
        0,
        1,
        [](Napi::Env) {}
    );

    sdk_->Authorize(scopes, [tsfn](const DiscordWrapper::AuthResult& result) {
        auto callback = [result](Napi::Env env, Napi::Function jsCallback) {
            Napi::Object resultObj = Napi::Object::New(env);
            resultObj.Set("success", Napi::Boolean::New(env, result.success));
            resultObj.Set("accessToken", Napi::String::New(env, result.accessToken));
            resultObj.Set("error", Napi::String::New(env, result.error));
            jsCallback.Call({resultObj});
        };
        tsfn.BlockingCall(callback);
        tsfn.Release();
    });

    return env.Undefined();
}

Napi::Value DiscordSocialSDK::RunCallbacks(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    sdk_->RunCallbacks();
    return env.Undefined();
}

Napi::Value DiscordSocialSDK::IsInitialized(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return Napi::Boolean::New(env, sdk_->IsInitialized());
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    return DiscordSocialSDK::Init(env, exports);
}

NODE_API_MODULE(discord_social_sdk, Init)
