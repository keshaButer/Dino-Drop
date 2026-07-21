#pragma once
#include <glm/glm.hpp>
#include "../Engine/engine.h"
#include "../SpriteRenderer/sprite_renderer.h"
#include "../Event/Event.h"
#include "../FontRenderer/FontRenderer.h"
#include "../Resources/resource_manager.h"
#include "../Invoke/InvokeSystem.h"
#include "../Audio/AudioManager.h"
#include "../Core/Config.h"

class Button
{
public:
    Button(glm::vec2 _centerWorld, float _width, float _height, std::string _text, SpriteRenderer* _spriteRenderer, FontRenderer* _textRenderer, float _textSize) : 
        centerWorld(_centerWorld),
        width(_width),
        height(_height),
        text(_text),
        spriteRenderer(_spriteRenderer),
        textRenderer(_textRenderer),
        textSize(_textSize)
    {
        min = glm::vec2(centerWorld.x - width * 0.5f, centerWorld.y - height * 0.5f);
        max = glm::vec2(centerWorld.x + width * 0.5f, centerWorld.y + height * 0.5f);

        texture = ResourceManager::Get().GetTexture(Config::TextureNames::MENU_BUTTON_BG);

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

        if (isPressed)
        {
            currentColor = lerp(currentColor, pressedColor, factor);
            currentScale = lerp(currentScale, pressedScale, factor);
        }
        else
        {
            currentColor = lerp(currentColor, defaultColor, factor);
            currentScale = lerp(currentScale, defaultScale, factor);
        }
    }

    void Draw()
    {
        if (!isActive) return;

        spriteRenderer->Draw(centerWorld, glm::vec2(width, height) * currentScale, 0, currentColor, texture);
        textRenderer->RenderText(text, centerWorld, textSize * currentScale, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    Event<> onPress;

private:
    Texture* texture;
    glm::vec2 centerWorld;

    glm::vec4 currentColor;
    glm::vec4 defaultColor = glm::vec4(1.0f);
    glm::vec4 pressedColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

    float width;
    float height;
    float acceleration = 0.4f;
    float currentScale = 1.0f;
    float defaultScale = 1.0f;
    float pressedScale = 0.9f;
    float textSize;
    glm::vec2 min, max;
    std::string text;
    bool isPressed = false;

    FontRenderer* const textRenderer;
    SpriteRenderer* const spriteRenderer;

    void HandleInput(int32_t action, float touchX, float touchY)
    {
        if (!isActive) return;

        if (intersects(touchX, touchY))
        {
            Engine::Get().isInputHandled = true;

            if (action == AMOTION_EVENT_ACTION_DOWN)
            {
                isPressed = true;

                AudioManager::Get().PlayAudioClip(Config::Sound::MOVE_TETROMINO, false, 1.0f, 1.0f);
            }
            
            if (action == AMOTION_EVENT_ACTION_UP)
            {
                isPressed = false;
                InvokeSystem::Get().Add(Config::Gameplay::BUTTON_PERFORM_DELAY, [this]() { onPress.Invoke(); });

                AudioManager::Get().PlayAudioClip(Config::Sound::MOVE_TETROMINO, false, 1.0f, 0.8f);
            }
        }
        else
        {
            isPressed = false;
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

    glm::vec4 lerp(glm::vec4 start, glm::vec4 end, float acceleration)
    {
        return start + (end - start) * acceleration;
    }

    float lerp(float start, float end, float acceleration)
    {
        return start + (end - start) * acceleration;
    }
};
