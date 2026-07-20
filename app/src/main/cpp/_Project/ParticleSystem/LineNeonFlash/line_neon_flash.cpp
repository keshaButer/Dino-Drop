#include "line_neon_flash.h"
#include "../Engine/engine.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../FXSystem.h"

LineNeonFlash::LineNeonFlash(int row, glm::vec3 col, Shader* vfxShader)
 : FXEffect(glm::vec2(0, row), col, Config::Gameplay::SHAKE_DURATION), spriteRenderer(vfxShader),
   shader(vfxShader), targetRow(row)
{
    type = FXType::LineNeonFlash;

    position = offset + glm::vec2(0, row) * Config::Gameplay::CELL_SIZE;

    shader->UseProgram();
    uTimeLocation = glGetUniformLocation(shader->IDprogram, "currentTime");

    glUseProgram(0);
}

void LineNeonFlash::Render()
{
    shader->UseProgram();

    float progress = lifeTime / maxLifeTime;
    glUniform1f(uTimeLocation, Engine::Get().GetTime());

    spriteRenderer.Draw(position, scale, 0.0f, finalColor, progress);
}
