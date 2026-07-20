#include "MainMenuState.h"
#include "../Engine/engine.h"
#include "../Core/Config.h"
#include "../GameStateManager.h"
#include "../Gameplay/GameplayState.h"
#include <cstdlib>

MainMenuState::MainMenuState(Camera* camera) : mainCamera(camera)
{}

void MainMenuState::Enter()
{
    Engine::Get().PrintInfo("MainMenuState: Enter");
    background = std::make_unique<Background>();
    textRenderer = std::make_unique<FontRenderer>(Config::GetFontPathOTF("Base").c_str(), 82);
    spriteRenderer = std::make_unique<SpriteRenderer>();

    startButton = std::make_unique<Button>(
        glm::vec2(0.0f),
        1.0f,
        0.5f,
        "Start",
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


    startButton->onPress.Subscribe([this]()
        {
            GameStateManager::Get().SetState(new GameplayState(mainCamera)); 
        }
    );

    exitButton->onPress.Subscribe([this]()
        {
            std::exit(0);
        }
    );

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
    startButton->Update(deltaTime);
    settingsButton->Update(deltaTime);
    exitButton->Update(deltaTime);
}

void MainMenuState::DrawButtons()
{
    startButton->Draw();
    settingsButton->Draw();
    exitButton->Draw();
}
