#pragma once
#include "../FXEffect.h"
#include "../../SpriteRenderer/sprite_renderer.h"

class GhostTrail : public FXEffect
{
private:
    SpriteRenderer spriteRenderer;
    Shader* const shader;
    int targetCol;
    int startRow;
    int endRow;
    int widthInCells;
    GLuint widthInCellsLocation;
    GLuint heightInCellsLocation;
    glm::vec3 uColor;

public:
    GhostTrail(int col, int fromRow, int toRow, glm::vec3 colVec,
        Shader* vfxShader, int _widthInCells, glm::vec3 _color);

    void Render() override;
};
