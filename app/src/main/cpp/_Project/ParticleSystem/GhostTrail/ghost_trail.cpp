#include "ghost_trail.h"
#include "../../Core/Config.h"

#include "ghost_trail.h"

GhostTrail::GhostTrail(int col, int fromRow, int toRow, glm::vec3 colVec, Shader* vfxShader, int _widthInCells, glm::vec3 _color)
    : FXEffect(glm::vec2(col, toRow), colVec, Config::VFX::GHOST_EFFECT_DURATION),
      spriteRenderer(vfxShader),
      shader(vfxShader),
      targetCol(col),
      startRow(fromRow),
      endRow(toRow),
      widthInCells(_widthInCells),
      uColor(_color)
{
    type = FXType::GhostTrail; 

    shader->UseProgram();
    widthInCellsLocation = glGetUniformLocation(shader->IDprogram, "uWidthInCells");
    heightInCellsLocation = glGetUniformLocation(shader->IDprogram, "uHeightInCells");
}

void GhostTrail::Render()
{
    int heightInCells = startRow - endRow;
    if (heightInCells <= 0) return;

    shader->UseProgram();

    float progress = lifeTime / maxLifeTime;

    glm::vec2 scale = glm::vec2(
        Config::Gameplay::CELL_SIZE * widthInCells, 
        Config::Gameplay::CELL_SIZE * heightInCells
    );

    glm::vec2 position = glm::vec2(
        Config::Gameplay::BOARD_OFFSET_X + scale.x * 0.5f + targetCol * Config::Gameplay::CELL_SIZE,
        Config::Gameplay::BOARD_OFFSET_Y + scale.y * 0.5f + endRow * Config::Gameplay::CELL_SIZE
    );

    glUniform1i(widthInCellsLocation, widthInCells);
    glUniform1i(heightInCellsLocation, heightInCells);

    spriteRenderer.Draw(position, scale, 0.0f, glm::vec4(uColor, 1.0), progress);
}
