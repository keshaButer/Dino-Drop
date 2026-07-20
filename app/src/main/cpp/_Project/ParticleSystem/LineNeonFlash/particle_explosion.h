#pragma once
#include "../ParticleSystem/FXEffect.h"
#include "../Shaders/Shader.h"
#include <GLES3/gl3.h>
#include "../../SpriteRenderer/sprite_renderer.h"
#include "../Core/Config.h"

struct Particle
{
    glm::vec3 position = glm::vec3(0);
    glm::vec3 scale = glm::vec3(0);
    glm::vec3 velocity = glm::vec3(0);
    float lifeTime = 0.0f;
    float maxLifeTime = 0.0f;
};

class ParticleExplosion : public FXEffect 
{
private:
    SpriteRenderer spriteRenderer;
    Shader* const shader;
    GLuint currentTimeLocation;
    int targetRow;
    int particleCount = 50;
    float deltaX = Config::Gameplay::BOARD_WIDTH * Config::Gameplay::CELL_SIZE / particleCount;
    std::vector<Particle> particles;

    glm::vec3 offset = glm::vec3(Config::Gameplay::BOARD_OFFSET_X, Config::Gameplay::BOARD_OFFSET_Y, 0.0f);

    void Emmite();

public:
    ParticleExplosion(int row, glm::vec3 col, Shader* vfxShader);

    void Render() override;
};
