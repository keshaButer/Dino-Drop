#include "GameplayState.h"
#include "../Render/render.h"
#include "../Grid/grid.h"
#include "../Tetrominos/draw_tetromino.h"
#include "../Tetrominos/activeTetromino.h"
#include "../Camera/camera.h"
#include "../Background/moving_background.h"
#include "../Score/DrawScore.h"
#include "../Score/ScoreManager.h"
#include "../Audio/AudioManager.h"
#include "../Button/Button.h"
#include "../SpriteRenderer/sprite_renderer.h"
#include "../Pause/PauseManager.h"
#include "../LockDelayStatusBar/LockBar.h"

GameplayState::GameplayState(Camera* camera) : mainCamera(camera) {}

void GameplayState::Enter()
{
    Engine::Get().PrintInfo("GameplayState: Enter");

    AudioManager::Get().PlayAudioClip(Config::Sound::GAMEPLAY_BG_MUSIC, true, 0.45f, 1.0f);

    spriteRenderer = std::make_unique<SpriteRenderer>();
    fontRenderer = std::make_unique<FontRenderer>(Config::GetFontPathOTF("Base").c_str(), 82);
    pauseButton = std::make_unique<Button>(glm::vec2(0.0f, 1.85f), 0.3f, 0.3f, "| |", spriteRenderer.get(), fontRenderer.get(), 0.5f);

    pauseButton->onPress.Subscribe([this]()
        {
            PauseManager::Get().TogglePause();
        }
    );

    background = std::make_unique<Background>();
    drawTetromino = std::make_unique<DrawTetromino>();

    board = std::make_unique<Board>(drawTetromino.get(), mainCamera);
    ScoreManager::Get().Initialize(board.get());

    activeTetromino = std::make_unique<ActiveTetromino>(board.get(), drawTetromino.get(), mainCamera);
    grid = std::make_unique<Grid>(board.get());

    lockBar = std::make_unique<LockBar>(activeTetromino.get());

    drawScore = std::make_unique<DrawScore>(board.get(), mainCamera);

    render = std::make_unique<Render>(
        grid.get(),
        board.get(),
        activeTetromino.get(),
        background.get(),
        drawScore.get(),
        pauseButton.get(),
        lockBar.get()
    );
}

void GameplayState::Update(float deltaTime)
{
    pauseButton->Update(deltaTime);

    if (!PauseManager::Get().IsPaused())
    {
        mainCamera->Move(deltaTime);
        activeTetromino->Update(deltaTime);
        lockBar->Update(deltaTime);
    }

    render->RenderFrame();
}

void GameplayState::Exit()
{

}
