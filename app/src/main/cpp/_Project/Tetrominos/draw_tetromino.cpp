#include "draw_tetromino.h"
#include "../Engine/engine.h"
#include "../Core/Config.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Shaders/ShaderManager.h"

DrawTetromino::DrawTetromino() : spriteRenderer(ShaderManager::Get().GetShader("Tetromino")),
 spriteRendererGhost(ShaderManager::Get().GetShader("Ghost")),
 shader(ShaderManager::Get().GetShader("Tetromino")), shaderGhost(ShaderManager::Get().GetShader("Ghost"))
{
    shader->UseProgram();

    timePos = glGetUniformLocation(shader->IDprogram, "time");
    sinTimePos = glGetUniformLocation(shader->IDprogram, "sinTime");

    shaderGhost->UseProgram();

    timePos = glGetUniformLocation(shaderGhost->IDprogram, "time");
    sinTimePos = glGetUniformLocation(shaderGhost->IDprogram, "sinTime");

    glUseProgram(0);
}

void DrawTetromino::Draw(int row, int col, int color, bool isGhost)
{
    shader->UseProgram();

    float currentTime = Engine::Get().GetTime();

    glUniform1f(timePos, currentTime);

    float width = 1.0f;
    float height = 0.0f;
    glUniform1f(sinTimePos, sin(currentTime * width) + height);

    row++;
    col++;

    if (row > Config::Gameplay::BOARD_HEIGHT || row <= 0 || col > Config::Gameplay::BOARD_WIDTH || col <= 0) return;

    int baseColor = color * 3;
    glm::vec4 finalColor = glm::vec4( 
        Config::Color::TETROMINO_COLORS[baseColor], 
        Config::Color::TETROMINO_COLORS[baseColor + 1], 
        Config::Color::TETROMINO_COLORS[baseColor + 2],
        1.0f
    );

    if (isGhost)
    {
        spriteRendererGhost.Draw(
            glm::vec2(Config::Gameplay::BOARD_OFFSET_X - Config::Gameplay::CELL_SIZE * 0.5f + col * Config::Gameplay::CELL_SIZE, Config::Gameplay::BOARD_OFFSET_Y - Config::Gameplay::CELL_SIZE * 0.5f + row * Config::Gameplay::CELL_SIZE),
            glm::vec2(Config::Gameplay::CELL_SIZE - 0.025f, Config::Gameplay::CELL_SIZE - 0.025f),
            0.0f,
            finalColor
        );
    }
    else
    {
        spriteRenderer.Draw(
            glm::vec2(Config::Gameplay::BOARD_OFFSET_X - Config::Gameplay::CELL_SIZE * 0.5f + col * Config::Gameplay::CELL_SIZE, Config::Gameplay::BOARD_OFFSET_Y - Config::Gameplay::CELL_SIZE * 0.5f + row * Config::Gameplay::CELL_SIZE),
            glm::vec2(Config::Gameplay::CELL_SIZE, Config::Gameplay::CELL_SIZE),
            0.0f,
            finalColor
        );
    }
}

void DrawTetromino::Draw(float row, float col, int color, float scale)
{
    shader->UseProgram();

    float currentTime = Engine::Get().GetTime();

    glUniform1f(timePos, currentTime);

    float width = 1.0f;
    float height = 0.0f;
    glUniform1f(sinTimePos, sin(currentTime * width) + height);

    row++;
    col++;

    int baseColor = color * 3;
    glm::vec4 finalColor = glm::vec4( 
        Config::Color::TETROMINO_COLORS[baseColor], 
        Config::Color::TETROMINO_COLORS[baseColor + 1], 
        Config::Color::TETROMINO_COLORS[baseColor + 2],
        1.0f
    );

    spriteRenderer.Draw(
        glm::vec2(Config::Gameplay::BOARD_OFFSET_X - Config::Gameplay::CELL_SIZE * 0.5f + col * Config::Gameplay::CELL_SIZE, Config::Gameplay::BOARD_OFFSET_Y - Config::Gameplay::CELL_SIZE * 0.5f + row * Config::Gameplay::CELL_SIZE),
        glm::vec2(Config::Gameplay::CELL_SIZE, Config::Gameplay::CELL_SIZE) * scale,
        0.0f,
        finalColor
    );
}
