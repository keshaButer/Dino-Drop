#pragma once
#include <vector>
#include <GLES3/gl3.h>
#include "../Shaders/Shader.h"
#include "../Board/board.h"

struct Vector2
{
    float x;
    float y;
};

class Grid
{
public:
    Grid(Board* board);
    void Draw();

    void TriggerWave(int targetRow)
    {
        waveCenterY = static_cast<float>(targetRow) / static_cast<float>(Config::Gameplay::BOARD_HEIGHT);
        waveRadius = 0.0f;
        waveIntensity = 1.0f;
    }

private:
    float waveCenterY = 0.0f;
    float waveRadius = 0.0f;
    float waveIntensity = 0.0f;

    GLint waveCenterYPos = -1;
    GLint waveRadiusPos = -1;
    GLint waveIntensityPos = -1;

    Vector2 cells[20][10];
    Shader* const shader;
    Board* const board;

    SpriteRenderer boardBG;

    float gridWidthScale = Config::Gameplay::BOARD_WIDTH * Config::Gameplay::CELL_SIZE;
    float gridHeightScale = Config::Gameplay::BOARD_HEIGHT * Config::Gameplay::CELL_SIZE;

    std::vector<float> GenerateVertices();

    GLuint VBO, VAO;
    GLsizei vertexCount;
    GLuint modelPos;
    GLuint colorPos;
    GLuint isGridPos;
    GLuint timePos;
};
