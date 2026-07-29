#include "GameplayState.h"
#include "../Render/render.h"
#include "../Grid/grid.h"
#include "../Tetrominos/draw_tetromino.h"
#include "../GameStateManager.h"
#include "../MainMenu/MainMenuState.h"
#include "../Tetrominos/activeTetromino.h"
#include "../Camera/camera.h"
#include "../Background/moving_background.h"
#include "../Score/DrawScore.h"
#include "../Score/ScoreManager.h"
#include "../Audio/AudioManager.h"
#include "../GameSettings/SettingsManager.h"
#include "../Button/Button.h"
#include "../SpriteRenderer/sprite_renderer.h"
#include "../Pause/PauseManager.h"
#include "../LockDelayStatusBar/LockBar.h"
#include "../Shaders/ShaderManager.h"

GameplayState::GameplayState(Camera* camera) : mainCamera(camera) {}

void GameplayState::OnContextRestored()
{
    mainCamera->ClearViewMatrix();
    PauseManager::Get().SetPaused(true);

    background->texture = ResourceManager::Get().GetTexture(Config::TextureNames::GAMEPLAY_BACKGROUND);

    Shader* shader = ShaderManager::Get().GetShader(Config::ShaderNames::DEFAULT_TEXTURE_UI);
    spriteRenderer->shader = shader;
    spriteRenderer->SetTextureID(shader->textureID);

    pauseBG = ResourceManager::Get().GetTexture(Config::TextureNames::PAUSE_BG);
    pauseButton->texture = ResourceManager::Get().GetTexture(Config::TextureNames::PAUSE);
    restartButton->OnContextRestore();
    enterMenuButton->OnContextRestore();

    drawTetromino->OnContextRestored();

    grid->OnContextRestored();
    board->OnContextRestored();
    activeTetromino->OnContextRestored();
    lockBar->OnContextRestored();
    drawScore->OnContextRestored();

    fontRenderer->Initialize(Config::GetFontPathOTF("Base").c_str(), 126);
}

void GameplayState::Enter()
{
    Engine::Get().PrintInfo("GameplayState: Enter");

    pauseBG = ResourceManager::Get().GetTexture(Config::TextureNames::PAUSE_BG);

    spriteRenderer = std::make_unique<SpriteRenderer>(ShaderManager::Get().GetShader(Config::ShaderNames::DEFAULT_TEXTURE_UI));
    fontRenderer = std::make_unique<FontRenderer>(Config::GetFontPathOTF("Base").c_str(), 128);

    glm::vec2 size = glm::vec2(0.9f, 0.4f);
    enterMenuButton = std::make_unique<Button>(
        glm::vec2(0.0f, -1.2f),
        size,
        "Menu",
        spriteRenderer.get(),
        fontRenderer.get(),
        0.6f
    );

    restartButton = std::make_unique<Button>(
        glm::vec2(0.0f, -0.7f),
        size,
        "Restart",
        spriteRenderer.get(),
        fontRenderer.get(),
        0.6f
    );

    enterMenuButton->onPress.Subscribe([this]() { GameStateManager::Get().SetState(new MainMenuState(mainCamera)); } );
    restartButton->onPress.Subscribe([this]() { GameStateManager::Get().SetState(new GameplayState(mainCamera)); } );

    enterMenuButton->isActive = false;
    restartButton->isActive = false;

    if (SettingsManager::Get().GetSettings().isMusicEnabled)
    {
        AudioManager::Get().PlayAudioClip(Config::Sound::GAMEPLAY_BG_MUSIC, true, 0.45f, 1.0f);
    }

    pauseButton = std::make_unique<Button>(glm::vec2(0.0f, 1.85f), glm::vec2(0.3f), "", spriteRenderer.get(), fontRenderer.get(), 0.5f, ResourceManager::Get().GetTexture(Config::TextureNames::PAUSE));
    pauseButton->onPress.Subscribe([this]()
        {
            PauseManager::Get().TogglePause();
        }
    );

    background = std::make_unique<Background>();
    drawTetromino = std::make_unique<DrawTetromino>();

    grid = std::make_unique<Grid>(board.get());
    board = std::make_unique<Board>(drawTetromino.get(), mainCamera, grid.get());
    ScoreManager::Get().Initialize(board.get());

    activeTetromino = std::make_unique<ActiveTetromino>(board.get(), drawTetromino.get(), mainCamera);

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

    if (!PauseManager::Get().IsPausedUI())
    {
        mainCamera->Move(deltaTime);
        activeTetromino->Update(deltaTime);
        lockBar->Update(deltaTime);
        enterMenuButton->isActive = false;
        restartButton->isActive = false;
    }
    else
    {
        enterMenuButton->isActive = true;
        restartButton->isActive = true;

        enterMenuButton->Update(deltaTime);
        restartButton->Update(deltaTime);
    }

    render->RenderFrame();

    if (PauseManager::Get().IsPausedUI())
    {
        spriteRenderer->Draw(glm::vec2(0.0f), glm::vec2(2.0f, 1.5f), 0, glm::vec4(1.0f), pauseBG);
        fontRenderer->RenderText("Paused" , glm::vec2(0.0f, 0.1f), Config::UI::GAME_OVER_TEXT_SIZE, glm::vec4(1.0f));

        enterMenuButton->Draw();
        restartButton->Draw();
    }
}

void GameplayState::Exit()
{

}
