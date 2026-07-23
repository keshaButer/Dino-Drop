#include "MainMenuState.h"
#include "../Engine/engine.h"
#include "../Core/Config.h"
#include "../GameStateManager.h"
#include "../Gameplay/GameplayState.h"
#include "../Score/HighScoreManager.h"
#include "../Audio/AudioManager.h"
#include <cstdlib>

MainMenuState::MainMenuState(Camera* camera) : mainCamera(camera)
{}

void MainMenuState::Enter()
{
    Engine::Get().PrintInfo("MainMenuState: Enter");
    background = std::make_unique<Background>();
    textRenderer = std::make_unique<FontRenderer>(Config::GetFontPathOTF("Base").c_str(), 82);
    spriteRenderer = std::make_unique<SpriteRenderer>();

    GameSettings& settings = SettingsManager::Get().GetSettingsMutable();
    InitializeSwitches(settings);

    InitializeButtons();
    SubscribeButtons();

    if (SettingsManager::Get().GetSettings().isMusicEnabled)
    {
        AudioManager::Get().PlayAudioClip(Config::Sound::GAMEPLAY_BG_MUSIC, true, 0.5f, 1.0f);
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
            startButton->Draw();
            scoresButton->Draw();
            settingsButton->Draw();
            exitButton->Draw();
        } break;

        case MenuScreen::Scores:
        {
            textRenderer->RenderText("PERSONAL BESTS", glm::vec2(0.0f, 1.2f), 0.7f, glm::vec4(1.0f));

            const auto& scoresList = HighScoreManager::Get().LoadHighScores();
            float yOffset = 0.85f;
            int count = 1;

            for (int i = static_cast<int>(scoresList.size()) - 1; i >= 0; i--)
            {
                if (count > 10) break; 
                std::string scoreLine = std::to_string(count) + ". " + std::to_string(scoresList[i]);
                textRenderer->RenderText(scoreLine, glm::vec2(-0.75f, yOffset), 0.4f, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), false);
                yOffset -= 0.15f;
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
    outlineSwitch = std::make_unique<Switch>(
        glm::vec2(0.75f, 0.9f),
        0.3f,
        0.2f,
        spriteRenderer.get(),
        textRenderer.get(),
        settings.isOutlineEnabled,
        "Outline",
        1.65f
    );

    musicSwitch = std::make_unique<Switch>(
        glm::vec2(0.75f, 1.5f),
        0.3f,
        0.2f,
        spriteRenderer.get(),
        textRenderer.get(),
        settings.isMusicEnabled,
        "Music",
        1.65f
    );

    ghostSwitch = std::make_unique<Switch>(
        glm::vec2(0.75f, 1.2f),
        0.3f,
        0.2f,
        spriteRenderer.get(),
        textRenderer.get(),
        settings.isGhostEnabled,
        "Ghost",
        1.65f
    );
}

void MainMenuState::InitializeButtons()
{
    startButton = std::make_unique<Button>(
        glm::vec2(0.0f),
        1.0f,
        0.5f,
        "Start",
        spriteRenderer.get(),
        textRenderer.get(),
        0.6f
    );

    scoresButton = std::make_unique<Button>(
        glm::vec2(0.0f, 0.5f),
        1.0f,
        0.5f,
        "Scores",
        spriteRenderer.get(),
        textRenderer.get(),
        0.6f
    );

    settingsButton = std::make_unique<Button>(
        glm::vec2(0.0f, -0.5f),
        1.0f,
        0.5f,
        "Settings",
        spriteRenderer.get(),
        textRenderer.get(),
        0.6f
    );

    exitButton = std::make_unique<Button>(
        glm::vec2(0.0f, -1.0f),
        1.0f,
        0.5f,
        "Exit",
        spriteRenderer.get(),
        textRenderer.get(),
        0.6f
    );

    backButton = std::make_unique<Button>(
        glm::vec2(0.0f, -1.5f),
        1.0f,
        0.5f,
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
