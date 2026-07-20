#include "GameOverState.h"
#include "../../Engine/engine.h"
#include "../Core/Config.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include "../GameStateManager.h"
#include "../Score/HighScoreManager.h"
#include "../Gameplay/GameplayState.h"
#include "../Audio/AudioManager.h"
#include "../MainMenu/MainMenuState.h"
#include "../Core/Config.h"

GameOverState::GameOverState(Camera* camera, StateData _data) : mainCamera(camera), data(_data)
{}

void GameOverState::Enter()
{
    Engine::Get().PrintInfo("GameOverState: Enter");

    if (data.Score > HighScoreManager::Get().GetBestScore())
    {
        HighScoreManager::Get().AddNewScore(data.Score);
    }

    background = std::make_unique<Background>(ResourceManager::Get().GetTexture(Config::TextureNames::GAMEPLAY_BACKGROUND));

    fontRenderer = std::make_unique<FontRenderer>(Config::GetFontPathOTF("Base").c_str(), 128);
    spriteRenderer = std::make_unique<SpriteRenderer>();

    enterMenuButton = std::make_unique<Button>(
        glm::vec2(0.0f, -1.2f),
        1.0f,
        0.5,
        "Menu",
        spriteRenderer.get(),
        fontRenderer.get(),
        0.6f
    );

    restartButton = std::make_unique<Button>(
        glm::vec2(0.0f, -0.7f),
        1.0f,
        0.5f,
        "Restart",
        spriteRenderer.get(),
        fontRenderer.get(),
        0.6f
    );

    enterMenuButton->onPress.Subscribe([this]() { GameStateManager::Get().SetState(new MainMenuState(mainCamera)); } );
    restartButton->onPress.Subscribe([this]() { GameStateManager::Get().SetState(new GameplayState(mainCamera)); } );

    AudioManager::Get().PlayAudioClip(Config::Sound::GAME_OVER_SOUND);
}

void GameOverState::Update(float deltaTime)
{
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    background->Draw();
    fontRenderer->RenderText("Game Over", glm::vec2(0.0f, 1.0f), Config::UI::GAME_OVER_TEXT_SIZE, glm::vec4(1.0f));
    fontRenderer->RenderText("Score: " + std::to_string(data.Score), glm::vec2(0.0f, 0.0f), Config::UI::SCORE_GAME_OVER_TEXT_SIZE, glm::vec4(1.0f));

    enterMenuButton->Update(deltaTime);
    enterMenuButton->Draw();

    restartButton->Update(deltaTime);
    restartButton->Draw();
}

void GameOverState::Exit()
{

}
