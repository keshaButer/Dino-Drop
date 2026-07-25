#pragma once
#include <GLES3/gl3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "../Shaders/Shader.h"
#include "../SpriteRenderer/sprite_renderer.h"

class DrawTetromino
{
public:
    DrawTetromino();
    void OnContextRestored();

    void Draw(int row, int col, int color, bool isGhost = false);
    void Draw(float row, float col, int color, float scale = 1.0f);
    void Draw(float row, float col, int color, glm::vec2 scale, float angle);
    void DrawOutLine(int row, int col, glm::vec4 color, float thickness, SpriteRenderer& sprite);

    SpriteRenderer& GetSpriteRenderer() { return spriteRenderer; }

    SpriteRenderer spriteRenderer;
    SpriteRenderer spriteRendererGhost;
    Shader* shader;
    Shader* shaderGhost;
    GLuint timePos;
    GLuint sinTimePos;
};
