#pragma once
#include "../Score/ScoreManager.h"
#include "../FontRenderer/FontRenderer.h"
#include "../Board/board.h"
#include "../Invoke/InvokeSystem.h"
#include "../Camera/camera.h"

class DrawScore
{
public:
    DrawScore(Board* board, Camera* camera) : 
        camera(camera),
        fontRenderer(Config::GetFontPathOTF("Base").c_str(), 82),
        position(Engine::Get().WorldToScreen(
            glm::vec2(
                Config::Gameplay::BOARD_OFFSET_X,
                (Config::Gameplay::BOARD_OFFSET_Y + 20.5f * Config::Gameplay::CELL_SIZE)
            ),
            camera->GetViewMatrix())
        ),
        color(
            glm::vec4(
                Config::UI::SCORE_TEXT_COLOR[0],
                Config::UI::SCORE_TEXT_COLOR[1],
                Config::UI::SCORE_TEXT_COLOR[2],
                Config::UI::SCORE_TEXT_COLOR[3]
            )
        ),
        colorYellow(glm::vec4(1, 1, 0, 1)),
        currentColor(color)
    { 
        board->OnLinesCleard.Subscribe([this](int lines) { this->Animate(); });
    }

    void Draw()
    {
        std::string text = "Score:" + std::to_string(ScoreManager::Get().GetCurrentScore());

        glm::vec2 pos = glm::vec2(
            Config::Gameplay::BOARD_OFFSET_X + 2.0f * Config::Gameplay::CELL_SIZE,
            (Config::Gameplay::BOARD_OFFSET_Y + (Config::Gameplay::BOARD_HEIGHT + 1.0f) * Config::Gameplay::CELL_SIZE)
        );

        fontRenderer.RenderText(
            text,
            pos,
            Config::UI::SCORE_TEXT_SCALE,
            currentColor
        );
    }

private:
    FontRenderer fontRenderer;
    Camera* const camera;

    glm::vec2 position;

    glm::vec4 color;
    glm::vec4 currentColor;

    glm::vec4 colorYellow;

    void Animate()
    {
        float interval = Config::UI::SCORE_TEXT_ANIMATION_INTERVAL;
        for (int i = 0; i < Config::UI::SCORE_TEXT_ANIMATION_COUNT; i++)
        {
            float delay = interval * i + interval;
            glm::vec4 col = i % 2 == 0 ? colorYellow : color;
            InvokeSystem::Get().Add(delay, [=]() { currentColor = col; });
        }

        float delay = interval * Config::UI::SCORE_TEXT_ANIMATION_COUNT + interval;
        InvokeSystem::Get().Add(delay, [=]() { currentColor = color; });
    }
};
