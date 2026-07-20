#pragma once
#include <android/log.h>
#include <android_native_app_glue.h>
#include <EGL/egl.h>
#include <android/asset_manager.h>
#include <cstdarg>
#include <chrono>
#include <glm/glm.hpp>
#include <random>
#include "../Event/Event.h"

class Engine
{
public:
    static Engine& Get() { static Engine instance; return instance; }    

    static int32_t InputCallback(struct android_app* app, AInputEvent* event);
    Event<int32_t, float, float> OnUIInput;
    Event<int32_t, float, float> OnGameplayInput;
    bool isInputHandled = false;

    void PrintInfo(const char* fmt, ...);
    void PrintError(const char* fmt, ...);
    void PrintDebug(const char* fmt, ...);

    AAssetManager* GetAssetManager() { return app->activity->assetManager; }

    int Initialize();
    void SetApp(android_app* _app);
    void Update();
    void SwapBuffers();
    int GetWindowWidth();
    int GetWindowHeight();
    float GetTime() const { return totalTime; }
    float GetDeltaTime() const { return deltaTime; }
    float GetUnscaledDeltaTime() const { return unscaledDeltaTime; }
    bool IsContextReady() const { return isContextReady; }
    float RandomRange(float a, float b);
    glm::vec2 WorldToScreen(glm::vec2 worldPos, const glm::mat4& cameraView);
    glm::vec2 ScreenToWorld(glm::vec2 pixelPos);

private:
    bool isContextReady = false;
    android_app* app;

    float unscaledDeltaTime = 0.0f;
    float deltaTime = 0.0f;
    float totalTime = 0.0f;
    bool isFirstFrame = true;
    std::chrono::high_resolution_clock::time_point lastTime;

    EGLDisplay display = EGL_NO_DISPLAY;
    EGLSurface surface = EGL_NO_SURFACE;
    EGLContext context = EGL_NO_CONTEXT;

    std::mt19937 g_RandomEngine;

    const char* LogTag = "Testrix";
    void HandleEvents();
    void UpdateTime();
};
