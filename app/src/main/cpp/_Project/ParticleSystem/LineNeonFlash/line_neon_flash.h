#pragma once
#include "../Core/Config.h"
#include "../ParticleSystem/FXEffect.h"
#include "../Shaders/Shader.h"
#include <GLES3/gl3.h>
#include "../../SpriteRenderer/sprite_renderer.h"

class LineNeonFlash : public FXEffect
{
private:
    SpriteRenderer spriteRenderer;
    GLuint uTimeLocation = 0;
    Shader* const shader;
    int targetRow;

    float width = Config::Gameplay::CELL_SIZE * Config::Gameplay::BOARD_WIDTH;
    glm::vec2 offset = glm::vec2(Config::Gameplay::BOARD_OFFSET_X + width * 0.5f, Config::Gameplay::BOARD_OFFSET_Y + Config::Gameplay::CELL_SIZE * 0.5f);
    glm::vec2 position;
    glm::vec2 scale = glm::vec2(width, Config::Gameplay::CELL_SIZE);
    glm::vec4 finalColor = glm::vec4(color, 1.0f);

    void InitGeometry();

public:
    LineNeonFlash(int row, glm::vec3 col, Shader* vfxShader);

    void Render() override;
};
