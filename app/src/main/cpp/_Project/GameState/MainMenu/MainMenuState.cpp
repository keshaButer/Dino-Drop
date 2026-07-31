#include "MainMenuState.h"
#include "../Engine/engine.h"
#include "../Core/Config.h"
#include "../GameStateManager.h"
#include "../Gameplay/GameplayState.h"
#include "../Shaders/ShaderManager.h"
#include "../Score/HighScoreManager.h"
#include "../Audio/AudioManager.h"
#include "../Pause/PauseManager.h"
#include <cstdlib>

MainMenuState::MainMenuState(Camera* camera) : mainCamera(camera)
{}

void MainMenuState::OnContextRestored()
{
    mainCamera->ClearViewMatrix();
    PauseManager::Get().SetPaused(false);

    background->texture = ResourceManager::Get().GetTexture(Config::TextureNames::GAMEPLAY_BACKGROUND);
    textRenderer->Initialize(Config::GetFontPathOTF("Base").c_str(), 82);
    titleRenderer->Initialize(Config::GetFontPathOTF("Base").c_str(), 512);

    Shader* shader = ShaderManager::Get().GetShader(Config::ShaderNames::DEFAULT_TEXTURE);
    spriteRenderer->shader = shader;
    spriteRenderer->SetTextureID(shader->textureID);

    startButton->OnContextRestore();
    exitButton->OnContextRestore();
    settingsButton->OnContextRestore();
    scoresButton->OnContextRestore();
    backButton->OnContextRestore();
    musicSwitch->OnContextRestore();
    ghostSwitch->OnContextRestore();
    outlineSwitch->OnContextRestore();
}

void MainMenuState::Enter()
{
    Engine::Get().PrintInfo("MainMenuState: Enter");
    background = std::make_unique<Background>();
    textRenderer = std::make_unique<FontRenderer>(Config::GetFontPathOTF("Base").c_str(), 82);
    titleRenderer = std::make_unique<FontRenderer>(Config::GetFontPathOTF("Base").c_str(), 512);
    spriteRenderer = std::make_unique<SpriteRenderer>();

    HighScoreManager::Get().Initialize();

    GameSettings& settings = SettingsManager::Get().GetSettingsMutable();
    InitializeSwitches(settings);

    InitializeButtons();
    SubscribeButtons();

    if (SettingsManager::Get().GetSettings().isMusicEnabled)
    {
        AudioManager::Get().PlayAudioClip(Config::Sound::GAMEPLAY_BG_MUSIC, true, 0.45f, 1.0f);
    }
}

void MainMenuState::Update(float deltaTime)
{
    UpdateButtons(deltaTime);

    background->Draw();
    DrawButtons();
}

void MainMenuState::Exit()
{
    Engine::Get().PrintInfo("MainMenuState: Exit");
}

void MainMenuState::UpdateButtons(float deltaTime)
{
    DisableAllButtons();

    switch (currentScreen)
    {
        case MenuScreen::Main:
        {
            startButton->isActive = true;
            scoresButton->isActive = true;
            settingsButton->isActive = true;
            exitButton->isActive = true;
        } break;

        case MenuScreen::Scores:
        {
            backButton->isActive = true;
        } break;

        case MenuScreen::Settings:
        {
            musicSwitch->isActive = true;
            ghostSwitch->isActive = true;
            outlineSwitch->isActive = true;
            backButton->isActive = true;
        } break;
    }

    startButton->Update(deltaTime);
    scoresButton->Update(deltaTime);
    settingsButton->Update(deltaTime);
    exitButton->Update(deltaTime);
    backButton->Update(deltaTime);
    musicSwitch->Update(deltaTime);
    ghostSwitch->Update(deltaTime);
    outlineSwitch->Update(deltaTime);
}

void MainMenuState::DisableAllButtons()
{
    startButton->isActive = false;
    scoresButton->isActive = false;
    settingsButton->isActive = false;
    exitButton->isActive = false;
    backButton->isActive = false;
    musicSwitch->isActive = false;
    ghostSwitch->isActive = false;
    outlineSwitch->isActive = false;
}

void MainMenuState::DrawButtons()
{
    switch (currentScreen)
    {
        case MenuScreen::Main:
        {
            titleRenderer->RenderText("Dino", glm::vec2(-0.2f, 1.6f), 2.0f, glm::vec4(1.0f, 1.0, 0.0f, 1.0f), true, 0.055f);
            titleRenderer->RenderText("Drop", glm::vec2(0.2f, 0.9f), 2.0f, glm::vec4(1.0f, 1.0, 0.0f, 1.0f), true, 0.055f);

            startButton->Draw();
            scoresButton->Draw();
            settingsButton->Draw();
            exitButton->Draw();
        } break;

        case MenuScreen::Scores:
        {
            textRenderer->RenderText("PERSONAL BESTS", glm::vec2(0.0f, 1.5f), 0.7f, glm::vec4(1.0f));

            const auto& scoresList = HighScoreManager::Get().GetHighScores();
            float yOffset = 1.2f;
            int count = 1;

            for (auto score : scoresList)
            {
                if (count > 10) break; 

                std::string scoreLine = std::to_string(count) + ". " + std::to_string(score);
                textRenderer->RenderText(scoreLine, glm::vec2(0.0f, yOffset), 0.55f, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

                yOffset -= 0.20f;
                count++;
            }

            if (scoresList.empty())
            {
                textRenderer->RenderText("No records yet!", glm::vec2(0.0f, 0.2f), 0.8f, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
            }

            backButton->Draw();

        } break;

        case MenuScreen::Settings:
        {
            musicSwitch->Draw();
            ghostSwitch->Draw();
            outlineSwitch->Draw();
            backButton->Draw();
        } break;
    }
}

void MainMenuState::InitializeSwitches(GameSettings& settings)
{
    glm::vec2 size = glm::vec2(0.3f, 0.2f);
    outlineSwitch = std::make_unique<Switch>(
        glm::vec2(0.75f, 0.9f),
        size,
        spriteRenderer.get(),
        textRenderer.get(),
        settings.isOutlineEnabled,
        "Outline",
        1.65f
    );

    musicSwitch = std::make_unique<Switch>(
        glm::vec2(0.75f, 1.5f),
        size,
        spriteRenderer.get(),
        textRenderer.get(),
        settings.isMusicEnabled,
        "Music",
        1.65f
    );

    ghostSwitch = std::make_unique<Switch>(
        glm::vec2(0.75f, 1.2f),
        size,
        spriteRenderer.get(),
        textRenderer.get(),
        settings.isGhostEnabled,
        "Ghost",
        1.65f
    );
}

void MainMenuState::InitializeButtons()
{
    glm::vec2 size = glm::vec2(0.9f, 0.4f);
    startButton = std::make_unique<Button>(
        glm::vec2(0.0f),
        size,
        "Start",
        spriteRenderer.get(),
        textRenderer.get(),
        0.6f
    );

    scoresButton = std::make_unique<Button>(
        glm::vec2(0.0f, -0.5f),
        size,
        "Scores",
        spriteRenderer.get(),
        textRenderer.get(),
        0.6f
    );

    settingsButton = std::make_unique<Button>(
        glm::vec2(0.0f, -1.0f),
        size,
        "Settings",
        spriteRenderer.get(),
        textRenderer.get(),
        0.6f
    );

    exitButton = std::make_unique<Button>(
        glm::vec2(0.0f, -1.5f),
        size,
        "Exit",
        spriteRenderer.get(),
        textRenderer.get(),
        0.6f
    );

    backButton = std::make_unique<Button>(
        glm::vec2(0.0f, -1.5f),
        size,
        "Back",
        spriteRenderer.get(),
        textRenderer.get(),
        0.6f
    );
}

void MainMenuState::SubscribeButtons()
{
    startButton->onPress.Subscribe([this]()
        {
            GameStateManager::Get().SetState(new GameplayState(mainCamera)); 
        }
    );

    scoresButton->onPress.Subscribe([this]()
        {
            currentScreen = MenuScreen::Scores;
        }
    );

    settingsButton->onPress.Subscribe([this]()
        {
            currentScreen = MenuScreen::Settings;
        }
    );

    exitButton->onPress.Subscribe([this]()
        {
            std::exit(0);
        }
    );

    backButton->onPress.Subscribe([this]()
        {
            currentScreen = MenuScreen::Main;
        }
    );

    musicSwitch->onSwitch.Subscribe([this]()
        {
            if (!SettingsManager::Get().GetSettings().isMusicEnabled)
            {
                AudioManager::Get().SetClipPlaying(Config::Sound::GAMEPLAY_BG_MUSIC, false);
            }
            else 
            {
                AudioManager::Get().SetClipPlaying(Config::Sound::GAMEPLAY_BG_MUSIC, true);
            }
        }
    );
}
