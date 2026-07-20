#include "particle_explosion.h"
#include "../Engine/engine.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>

typedef glm::vec3 Vector3;

ParticleExplosion::ParticleExplosion(int row, glm::vec3 col, Shader* vfxShader) 
 : FXEffect(glm::vec2(0, row), col, Config::VFX::PARTICLES_DURATION), spriteRenderer(vfxShader),
   shader(vfxShader), targetRow(row)
{
    type = FXType::LineExplosionParticles;
    shader->UseProgram();
    currentTimeLocation = glGetUniformLocation(shader->IDprogram, "currentTime");
    Emmite();
    glUseProgram(0);
}

void ParticleExplosion::Render()
{
    shader->UseProgram();
    float progress = lifeTime / maxLifeTime;

    glUniform1f(currentTimeLocation, Engine::Get().GetTime());

    for (int i = 0; i < particleCount; i++)
    {
        particles[i].lifeTime = lifeTime;
        particles[i].position += particles[i].velocity * Engine::Get().GetDeltaTime() * 100.0f;
        particles[i].velocity -= Vector3(0, 0.15f, 0) * Engine::Get().GetDeltaTime();

        glm::vec2 position = offset + particles[i].position;
        glm::vec2 scale = particles[i].scale;
        glm::vec4 finalColor = glm::vec4(color.r, color.g, color.b, 1.0f);

        spriteRenderer.Draw(position, scale, 0.0f, finalColor, progress);
    }
}

void ParticleExplosion::Emmite()
{
    particles.resize(particleCount);

    float maxLifeTime = 1.0f;

    for (int i = 0; i < particleCount; i++)
    {
        auto random = rand();
        int minus = random % 2 ? 1 : -1;
        float randomSizeAdd = glm::clamp(minus*(random % 10) * 0.005f, 0.001f, 0.003f);
        particles[i].maxLifeTime = maxLifeTime;
        particles[i].position = glm::vec3(i * deltaX, targetRow * Config::Gameplay::CELL_SIZE, 0);
        particles[i].scale = glm::vec3(Config::Gameplay::CELL_SIZE * 0.1f + randomSizeAdd);
        int randX = random % 10;
        int randY = (random / 10) % 10;
        particles[i].velocity = glm::vec3(minus*(randX % 10) * 0.002, (randY % 10) * 0.003, 0);
    }
}
