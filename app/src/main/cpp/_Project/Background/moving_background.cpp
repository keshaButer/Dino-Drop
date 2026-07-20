#include "moving_background.h"
#include <glm/glm.hpp>
#include "../Shaders/ShaderManager.h"
#include "../Core/Config.h"
#include "../Engine/engine.h"
#include <cmath>

Background::Background() : mainBG(ShaderManager::Get().GetShader("Background")),
shader(ShaderManager::Get().GetShader("Background"))
{ 
    timeLocation = glGetUniformLocation(shader->IDprogram, "time");
}

Background::Background(Texture* _texture) : mainBG(ShaderManager::Get().GetShader("Background")),
shader(ShaderManager::Get().GetShader("Background")), texture(_texture)
{ 
    timeLocation = glGetUniformLocation(shader->IDprogram, "time");
}

void Background::Draw()
{
    cycledTime = std::fmod(Engine::Get().GetTime() * Config::Gameplay::BACKGROUND_SPEED, 1.0);

    shader->UseProgram();
    glUniform1f(timeLocation, cycledTime);
    
    if (texture == nullptr)
    {
        mainBG.Draw(
            glm::vec2(0.0f),
            glm::vec2(2.6f, 5.0f),
            0,
            glm::vec4(0.7f, 0.7f, 1.0f, 1.0f)
        );
    }
    else
    {
        mainBG.Draw(
            glm::vec2(0.0f),
            glm::vec2(2.6f, 5.0f),
            0,
            glm::vec4(0.7f, 0.7f, 1.0f, 1.0f),
            texture
        );
    }
}
