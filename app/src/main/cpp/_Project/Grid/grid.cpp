#include "../Engine/engine.h"
#include "grid.h"
#include "../Core/Config.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include "../Shaders/ShaderManager.h"

Grid::Grid(Board* board) : 
    board(board), 
    shader(ShaderManager::Get().GetShader(Config::ShaderNames::GRID)),
    boardBG(ShaderManager::Get().GetShader(Config::ShaderNames::DEFAULT_NO_TEXTURE))
{
    shader->UseProgram();

    std::vector<float> vertices = GenerateVertices();
    vertexCount = vertices.size() / 2;

    modelPos = glGetUniformLocation(shader->IDprogram, "model");
    colorPos = glGetUniformLocation(shader->IDprogram, "uColor");
    isGridPos = glGetUniformLocation(shader->IDprogram, "isGrid");
    timePos = glGetUniformLocation(shader->IDprogram, "time");

    waveCenterYPos = glGetUniformLocation(shader->IDprogram, "uWaveCenterY");
    waveRadiusPos = glGetUniformLocation(shader->IDprogram, "uWaveRadius");
    waveIntensityPos = glGetUniformLocation(shader->IDprogram, "uWaveIntensity");

    // VBO, VAO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
    glUseProgram(0);
}

void Grid::OnContextRestored()
{
    shader = ShaderManager::Get().GetShader(Config::ShaderNames::GRID);
    boardBG.shader = ShaderManager::Get().GetShader(Config::ShaderNames::DEFAULT_NO_TEXTURE);

    shader->UseProgram();

    std::vector<float> vertices = GenerateVertices();
    vertexCount = vertices.size() / 2;

    modelPos = glGetUniformLocation(shader->IDprogram, "model");
    colorPos = glGetUniformLocation(shader->IDprogram, "uColor");
    isGridPos = glGetUniformLocation(shader->IDprogram, "isGrid");
    timePos = glGetUniformLocation(shader->IDprogram, "time");

    waveCenterYPos = glGetUniformLocation(shader->IDprogram, "uWaveCenterY");
    waveRadiusPos = glGetUniformLocation(shader->IDprogram, "uWaveRadius");
    waveIntensityPos = glGetUniformLocation(shader->IDprogram, "uWaveIntensity");

    // VBO, VAO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
    glUseProgram(0);
}

void Grid::Draw()
{
    boardBG.Draw(
        glm::vec2(0.0f),
        glm::vec2(Config::Gameplay::BOARD_WIDTH * Config::Gameplay::CELL_SIZE, Config::Gameplay::BOARD_HEIGHT * Config::Gameplay::CELL_SIZE),
        0,
        glm::vec4(0.0f, 0.0f, 0.0f, 0.85f)
    );

    shader->UseProgram();

    float dt = Engine::Get().GetUnscaledDeltaTime();
    if (waveIntensity > 0.0f)
    {
        waveRadius += 1.8f * dt;
        waveIntensity -= 2.2f * dt;
        if (waveIntensity < 0.0f) waveIntensity = 0.0f;
    }

    glUniform1f(waveCenterYPos, waveCenterY);
    glUniform1f(waveRadiusPos, waveRadius);
    glUniform1f(waveIntensityPos, waveIntensity);

    float textureOffset = fmodf(Engine::Get().GetTime() * 0.012f, 1.0f);
    glUniform1f(timePos, textureOffset);

    glUniform1i(isGridPos, 1);
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(gridWidthScale, gridHeightScale, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5, -0.5, 0));

    glUniformMatrix4fv(modelPos, 1, GL_FALSE, glm::value_ptr(model));
    glUniform4f(colorPos, Config::Color::COLOR_GRID[0], Config::Color::COLOR_GRID[1], Config::Color::COLOR_GRID[2], Config::Color::COLOR_GRID[3]);

    glBindVertexArray(VAO);

    glLineWidth(Config::Gameplay::GRID_LINE_WIDTH);
    glDrawArrays(GL_LINES, 0, vertexCount);

    glUniform1i(isGridPos, 0);
}

std::vector<float> Grid::GenerateVertices() 
{
    std::vector<float> vertices;

    for (int row = 0; row <= Config::Gameplay::BOARD_HEIGHT; row++)
    {
        float y = (float)row / Config::Gameplay::BOARD_HEIGHT;

        vertices.push_back(0.0f);
        vertices.push_back(y);
        vertices.push_back(1.0f);
        vertices.push_back(y);
    }
    
    for (int col = 0; col <= Config::Gameplay::BOARD_WIDTH; col++)
    {
        float x = (float)col / Config::Gameplay::BOARD_WIDTH;

        vertices.push_back(x);
        vertices.push_back(0.0f);
        vertices.push_back(x);
        vertices.push_back(1.0f);
    }

    return vertices;
}
