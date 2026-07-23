#include "game.h"
#include "../Engine/engine.h"
#include "../ParticleSystem/FXSystem.h"
#include "../GameState/GameStateManager.h"
#include "../GameState/MainMenu/MainMenuState.h"
#include "../Invoke/InvokeSystem.h"
#include "../Shaders/ShaderManager.h"
#include "../Audio/AudioManager.h"
#include "../Score/HighScoreManager.h"
#include "../Core/Config.h"
#include "../Resources/resource_manager.h"

void Game::Start()
{
    Engine::Get().PrintInfo("Game Start");

    HighScoreManager::Get().Initialize();
    SettingsManager::Get().LoadSettings();
    LoadTextures();
    LoadShaders();
    LoadSounds();

    glUseProgram(0);

    GameStateManager::Get().mainCamera = mainCamera;
    GameStateManager::Get().SetState(new MainMenuState(mainCamera));
}

void Game::Update()
{
    float deltaTime = Engine::Get().GetDeltaTime();

    InvokeSystem::Get().Update(Engine::Get().GetUnscaledDeltaTime());
    FXSystem::Get().Update(deltaTime);

    GameStateManager::Get().UpdateCurrentState(deltaTime);
}

void Game::LoadShaders()
{
    ShaderManager& shaderManager = ShaderManager::Get();
    shaderManager.LoadShader("Background", false)->LoadTexture(Config::GetTexturePath("GameplayBackground").c_str());
    shaderManager.LoadShader("DefaultNoTex");
    shaderManager.LoadShader("DefaultTex");
    shaderManager.LoadShader("GhostFX");
    shaderManager.LoadShader("Tetromino")->LoadTexture(Config::GetTexturePath("tetromino").c_str());
    shaderManager.LoadShader("Flash");
    shaderManager.LoadShader("Particles");
    shaderManager.LoadShader("Ghost")->LoadTexture(Config::GetTexturePath("ghost").c_str());
    shaderManager.LoadShader("Font");
    shaderManager.LoadShader(Config::ShaderNames::LOCK_DELAY);
    shaderManager.LoadShader(Config::ShaderNames::GRID);

    shaderManager.AttachAllToCamera(mainCamera);
    shaderManager.SetTextureSlotZeroToAll();
}

void Game::LoadSounds()
{
    AudioManager& audioManager = AudioManager::Get();
    audioManager.LoadWavAudioClip(Config::Sound::TETROMINO_FREEZE_SOUND);
    audioManager.LoadWavAudioClip(Config::Sound::GAME_OVER_SOUND);
    audioManager.LoadWavAudioClip(Config::Sound::CLEAR_LINE_SOUND);
    audioManager.LoadWavAudioClip(Config::Sound::GAMEPLAY_BG_MUSIC);
    audioManager.LoadWavAudioClip(Config::Sound::MOVE_TETROMINO);
    audioManager.LoadWavAudioClip(Config::Sound::ROTATE_TETROMINO);
}

void Game::LoadTextures()
{
    ResourceManager& resourceManager = ResourceManager::Get();
    resourceManager.LoadTexture(Config::TextureNames::GAMEPLAY_BACKGROUND);
    resourceManager.LoadTexture(Config::TextureNames::MENU_BUTTON_BG);
    resourceManager.LoadTexture(Config::TextureNames::SWITCH_TUMBLER);
    resourceManager.LoadTexture(Config::TextureNames::SWITCH_BG);
}
