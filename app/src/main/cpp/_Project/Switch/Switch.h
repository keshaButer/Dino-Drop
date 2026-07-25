#pragma once
#include <glm/glm.hpp>
#include "../Engine/engine.h"
#include "../SpriteRenderer/sprite_renderer.h"
#include "../Event/Event.h"
#include "../GameSettings/SettingsManager.h"
#include "../Resources/resource_manager.h"
#include "../FontRenderer/FontRenderer.h"
#include "../Audio/AudioManager.h"
#include "../Core/Config.h"

class Switch
{
public:
    Switch(glm::vec2 _centerWorld, glm::vec2 size, SpriteRenderer* _spriteRenderer, FontRenderer* _fontRenderer, bool& _targetFlag, std::string _text, float _textDistance = 1.2f) : 
        centerWorld(_centerWorld),
        tumblerPos(_centerWorld),
        width(size.x),
        height(size.y),
        spriteRenderer(_spriteRenderer),
        fontRenderer(_fontRenderer),
        text(_text),
        textDistance(_textDistance),
        bgTexture(ResourceManager::Get().GetTexture(Config::TextureNames::SWITCH_BG)),
        tumblerTexture(ResourceManager::Get().GetTexture(Config::TextureNames::SWITCH_TUMBLER)),
        targetFlag(_targetFlag)
    {
        min = glm::vec2(centerWorld.x - width * 0.5f, centerWorld.y - height * 0.5f);
        max = glm::vec2(centerWorld.x + width * 0.5f, centerWorld.y + height * 0.5f);

        horizontalOffset = width * 0.15f;

        textPos = centerWorld - glm::vec2(textDistance, 0.0f);

        isEnabled = targetFlag; 
        currentColor = isEnabled ? enabledColor : disabledColor;

        Engine::Get().OnUIInput.Subscribe(
            [this](int32_t action, float x, float y){ this->HandleInput(action, x, y); }
        );
    }

    bool isActive = true;

    void Update(float deltaTime)
    {
        if (!isActive) return;

        float factor = acceleration * Engine::Get().GetUnscaledDeltaTime() * 100.0f;
        factor = glm::clamp(factor, 0.0f, 1.0f);

        if (isEnabled)
        {
            tumblerPos = lerp(tumblerPos, centerWorld + glm::vec2(horizontalOffset, 0.0f), factor);
            currentColor = lerp(currentColor, enabledColor, factor);
        }
        else 
        {
            tumblerPos = lerp(tumblerPos, centerWorld - glm::vec2(horizontalOffset, 0.0f), factor);
            currentColor = lerp(currentColor, disabledColor, factor);
        }
    }

    void Draw()
    {
        if (!isActive) return;

        spriteRenderer->Draw(centerWorld, glm::vec2(width, height) * currentScale, 0, glm::vec4(1.0f), bgTexture);
        spriteRenderer->Draw(tumblerPos, glm::vec2(height * 0.8f) * currentScale, 0, currentColor, tumblerTexture);
        fontRenderer->RenderText(text, textPos, currentScale * 0.5f, glm::vec4(1.0f), false);
    }

    Event<> onSwitch;
    bool isEnabled;

private:
    Texture* const bgTexture;
    Texture* const tumblerTexture;
    glm::vec2 centerWorld;
    glm::vec2 tumblerPos;
    glm::vec2 textPos;

    bool& targetFlag;

    glm::vec4 currentColor = glm::vec4(1.0f);
    glm::vec4 disabledColor = glm::vec4(1.0f);
    glm::vec4 enabledColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

    float width;
    float height;
    float horizontalOffset;
    float acceleration = 0.2f;
    float currentScale = 1.0f;
    float textDistance;
    glm::vec2 min, max;

    SpriteRenderer* const spriteRenderer;
    FontRenderer* const fontRenderer;
    std::string const text;

    void HandleInput(int32_t action, float touchX, float touchY)
    {
        if (!isActive) return;

        if (intersects(touchX, touchY))
        {
            Engine::Get().isInputHandled = true;

            if (action == AMOTION_EVENT_ACTION_DOWN)
            {
                isEnabled = !isEnabled;
                targetFlag = isEnabled;

                SettingsManager::Get().SaveSettings();

                onSwitch.Invoke();
                AudioManager::Get().PlayAudioClip(Config::Sound::BUTTON_PRESS, false, 1.0f, 1.0f);
            }
        }
    }

    bool intersects(float touchX, float touchY)
    {
        int pixelX = touchX;
        int pixelY = Engine::Get().GetWindowHeight() - touchY;

        glm::vec2 worldTouch = Engine::Get().ScreenToWorld(glm::vec2(pixelX, pixelY));

        if (worldTouch.x > max.x || worldTouch.y > max.y) return false;
        if (worldTouch.x < min.x || worldTouch.y < min.y) return false;

        return true;
    }

    glm::vec2 lerp(glm::vec2 start, glm::vec2 end, float acceleration)
    {
        return start + (end - start) * acceleration;
    }

    glm::vec4 lerp(glm::vec4 start, glm::vec4 end, float acceleration)
    {
        return start + (end - start) * acceleration;
    }

    float lerp(float start, float end, float acceleration)
    {
        return start + (end - start) * acceleration;
    }
};
