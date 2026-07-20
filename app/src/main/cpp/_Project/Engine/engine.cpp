#include "engine.h"
#include <cstdarg>
#include <GLES3/gl3.h>
#include <chrono>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Resources/resource_manager.h"
#include "../Audio/AudioManager.h"
#include "../Pause/PauseManager.h"
#include <chrono>

int Engine::Initialize()
{
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY) 
    {
        PrintError("eglGetDisplay failed");
        return -1;
    }

    if (eglInitialize(display, NULL, NULL) != EGL_TRUE) 
    {
        PrintError("eglInitialize failed");
        return -1;
    }

    const EGLint attribs[] = { EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL_NONE };
    EGLConfig config;
    EGLint num_configs;
    if (eglChooseConfig(display, attribs, &config, 1, &num_configs) != EGL_TRUE) 
    {
        PrintError("eglChooseConfig failed");
        return -1;
    }

    surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)app->window, NULL);
    if (surface == EGL_NO_SURFACE) 
    {
        PrintError("eglCreateWindowSurface failed");
        return -1;
    }

    const EGLint ctx_attribs[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, ctx_attribs);
    if (context == EGL_NO_CONTEXT) 
    {
        PrintError("eglCreateContext failed");
        return -1;
    }

    if (eglMakeCurrent(display, surface, surface, context) != EGL_TRUE) 
    {
        PrintError("eglMakeCurrent failed");
        return -1;
    }

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) 
        PrintError("OpenGL error after init: 0x%x", err);
    else 
        PrintDebug("OpenGL initialized successfully");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    isContextReady = true;

    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    g_RandomEngine.seed(static_cast<unsigned int>(seed));

    ResourceManager::Get().Initialize();
    AudioManager::Get().Initialize();

    return 0;
}

float Engine::RandomRange(float a, float b)
{
    std::uniform_real_distribution<float> volumeDist(a, b);
    return volumeDist(g_RandomEngine);
}

void Engine::Update()
{
    UpdateTime();
    HandleEvents();

    if (app->window != NULL && display == EGL_NO_DISPLAY) 
    {
        Initialize();
    }

    if (app->window == NULL && display != EGL_NO_DISPLAY) 
    {
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglDestroyContext(display, context);
        eglDestroySurface(display, surface);
        eglTerminate(display);
        display = EGL_NO_DISPLAY;
        isContextReady = false; 
        
        AudioManager::Get().Exit();

        PrintInfo("OpenGL destroyed");
    }
}

glm::vec2 Engine::WorldToScreen(glm::vec2 worldPos, const glm::mat4& cameraView)
{
    float width = static_cast<float>(GetWindowWidth());
    float height = static_cast<float>(GetWindowHeight());
    float aspect = width / height;

    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f / aspect, 1.0f / aspect);

    glm::vec4 clipSpace = projection * cameraView * glm::vec4(worldPos, 0.0f, 1.0f);
    
    glm::vec3 ndc = glm::vec3(clipSpace) / clipSpace.w;

    float screenX = ((ndc.x + 1.0f) * 0.5f) * width;
    float screenY = ((ndc.y + 1.0f) * 0.5f) * height;

    return glm::vec2(screenX, screenY);
}

glm::vec2 Engine::ScreenToWorld(glm::vec2 pixelPos)
{
    int width = GetWindowWidth();
    int height = GetWindowHeight();
    float aspect = static_cast<float>(width) / static_cast<float>(height);
    float borderHeight = 1.0f / aspect;

    float worldX = (pixelPos.x / width) * 2.0f - 1.0f;
    float worldY = (pixelPos.y / height) * 2.0f * borderHeight - borderHeight;

    return glm::vec2(worldX, worldY);
}


void Engine::SwapBuffers()
{
    eglSwapBuffers(display, surface);
}

void Engine::HandleEvents()
{
    int events;
    android_poll_source* source;
    while (ALooper_pollOnce(0, NULL, &events, (void**)&source) >= 0) 
    {
        if (source) source->process(app, source);
    }
}

void Engine::PrintInfo(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    __android_log_vprint(ANDROID_LOG_INFO, LogTag, fmt, args);
    va_end(args);
}

void Engine::PrintError(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    __android_log_vprint(ANDROID_LOG_ERROR, LogTag, fmt, args);
    va_end(args);
}

void Engine::PrintDebug(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    __android_log_vprint(ANDROID_LOG_DEBUG, LogTag, fmt, args);
    va_end(args);
}

void Engine::SetApp(android_app* _app) 
{
    app = _app;
    app->onInputEvent = Engine::InputCallback;
}

int Engine::GetWindowWidth()
{
    if (app && app->window)
    {
        return ANativeWindow_getWidth(app->window);
    }
    PrintError("Can not give window width, app is not initialized");
    return -1;
}

int Engine::GetWindowHeight()
{
    if (app && app->window)
    {
        return ANativeWindow_getHeight(app->window);
    }
    PrintError("Can not give window height, app is not initialized");
    return -1;
}

void Engine::UpdateTime()
{
    auto currentTime = std::chrono::high_resolution_clock::now();

    if (isFirstFrame)
    {
        lastTime = currentTime;
        isFirstFrame = false;
        return;
    }

    std::chrono::duration<float> elapsed = currentTime - lastTime;

    unscaledDeltaTime = elapsed.count();
    if (unscaledDeltaTime > 0.1f) unscaledDeltaTime = 0.1f;

    if (PauseManager::Get().IsPaused())
    {
        deltaTime = 0.0f;
    }
    else 
    {
        deltaTime = unscaledDeltaTime;
        totalTime += deltaTime;
    }

    lastTime = currentTime;
}

int32_t Engine::InputCallback(struct android_app* app, AInputEvent* event)
{
    if (AInputEvent_getType(event) != AINPUT_EVENT_TYPE_MOTION) return 0;

    int32_t action = AMotionEvent_getAction(event);
    int32_t actionCode = action & AMOTION_EVENT_ACTION_MASK;

    float touchX = AMotionEvent_getX(event, 0);
    float touchY = AMotionEvent_getY(event, 0);

    Engine& engine = Engine::Get();
    engine.isInputHandled = false;
    engine.OnUIInput.Invoke(actionCode, touchX, touchY);

    if (!engine.isInputHandled)
    {
        engine.OnGameplayInput.Invoke(actionCode, touchX, touchY);
    }

    return 1;
}
