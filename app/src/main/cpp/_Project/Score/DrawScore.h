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
        ScoreManager::Get().onAddScore.Subscribe([this](int addScore) { this->StartDrawAddScore(addScore); });
    }

    void OnContextRestored()
    {
        fontRenderer.Initialize(Config::GetFontPathOTF("Base").c_str(), 82);
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

        DrawAddScore();
    }

    FontRenderer fontRenderer;

private:
    Camera* const camera;

    glm::vec2 position;

    glm::vec4 color;
    glm::vec4 currentColor;

    glm::vec4 colorYellow;

    bool isDrawingAddScore;
    std::string currentAddScore;
    glm::vec2 addScorePos;
    glm::vec4 addScoreColor;

    void StartDrawAddScore(int addScore)
    {
        isDrawingAddScore = true;        
        InvokeSystem::Get().Add(Config::UI::DELAY_HIDE_ADD_SCORE_TEXT, [this]() 
            {
                this->isDrawingAddScore = false; 
            }
        );

        currentAddScore = std::to_string(addScore);

        addScorePos = glm::vec2(0.0f, -1.3f);
        glm::vec3 color = glm::vec3(
            Config::Color::COLOR_ADD_SCORE_TEXT[0],
            Config::Color::COLOR_ADD_SCORE_TEXT[1],
            Config::Color::COLOR_ADD_SCORE_TEXT[2]
        );

        addScoreColor = glm::vec4(color.x, color.y, color.z, 0.0f);
    }

    void DrawAddScore()
    {
        if (!isDrawingAddScore) return;

        addScorePos.y += Config::UI::SPEED_ADD_SCORE_TEXT * Engine::Get().GetDeltaTime();
        addScoreColor.a += std::clamp(Config::UI::SPEED_ADD_SCORE_TEXT_ALPHA * Engine::Get().GetDeltaTime(), 0.0f, 0.7f);

        fontRenderer.RenderText(
            currentAddScore,
            addScorePos,
            Config::UI::ADD_SCORE_TEXT_SIZE * glm::vec2(1.0f, 0.65f),
            addScoreColor,
            true,
            0.0f
        );
    }

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
