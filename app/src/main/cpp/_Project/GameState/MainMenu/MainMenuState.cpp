#include "MainMenuState.h"
#include "../Engine/engine.h"
#include "../Core/Config.h"
#include "../GameStateManager.h"
#include "../Gameplay/GameplayState.h"
#include "../Score/HighScoreManager.h"
#include <cstdlib>

MainMenuState::MainMenuState(Camera* camera) : mainCamera(camera)
{}

void MainMenuState::Enter()
{
    Engine::Get().PrintInfo("MainMenuState: Enter");
    background = std::make_unique<Background>();
    textRenderer = std::make_unique<FontRenderer>(Config::GetFontPathOTF("Base").c_str(), 82);
    spriteRenderer = std::make_unique<SpriteRenderer>();

    InitializeButtons();
    SubscribeButtons();
    // тут потом включу музыку
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
    switch (currentScreen)
    {
        case MenuScreen::Main:
        {
            startButton->isActive = true;
            scoresButton->isActive = true;
            settingsButton->isActive = true;
            exitButton->isActive = true;
            backButton->isActive = false;
        } break;

        case MenuScreen::Scores:
        {
            startButton->isActive = false;
            scoresButton->isActive = false;
            settingsButton->isActive = false;
            exitButton->isActive = false;
            backButton->isActive = true;
        } break;

        case MenuScreen::Settings:
        {

        } break;
    }

    startButton->Update(deltaTime);
    scoresButton->Update(deltaTime);
    settingsButton->Update(deltaTime);
    exitButton->Update(deltaTime);
    backButton->Update(deltaTime);
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
            float yOffset = 0.6f;
            int count = 1;

            for (int i = static_cast<int>(scoresList.size()) - 1; i >= 0; i--)
            {
                if (count > 5) break; 
                std::string scoreLine = std::to_string(count) + ". " + std::to_string(scoresList[i]);
                textRenderer->RenderText(scoreLine, glm::vec2(0.0f, yOffset), 0.8f, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
                yOffset -= 0.35f;
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

        } break;
    }
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

}
