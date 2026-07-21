#include "LockBar.h"
#include "../Tetrominos/activeTetromino.h"
#include "../SpriteRenderer/sprite_renderer.h"
#include "../Shaders/ShaderManager.h"

LockBar::LockBar(ActiveTetromino* tetromino) : activeTetromino(tetromino)
{
    sprite = std::make_unique<SpriteRenderer>(ShaderManager::Get().GetShader(Config::ShaderNames::LOCK_DELAY));
}

void LockBar::Update(float deltaTime)
{
    float actualProgress = activeTetromino->GetLockDelayTimer() / Config::Gameplay::LOCK_DELAY;
    Engine::Get().PrintDebug("ACTUAL PROGRESS IS: %f", actualProgress);
    progress = lerp(progress, actualProgress, std::clamp(5.0f * deltaTime, 0.0f, 1.0f));
    progress = std::clamp(progress, 0.0f, 1.0f);
    Engine::Get().PrintDebug("PROGRESS IS: %f", progress);
}

void LockBar::Draw()
{
    sprite->Draw(
        glm::vec2(0.0f, yOffset),
        glm::vec2(Config::Gameplay::CELL_SIZE * Config::Gameplay::BOARD_WIDTH, 0.025f),
        0,
        glm::vec4(1.0f),
        progress
    );
}
