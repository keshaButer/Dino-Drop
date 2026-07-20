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

private:
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
