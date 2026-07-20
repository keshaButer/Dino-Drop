#pragma once
#include <glm/glm.hpp>
#include "../Pause/PauseManager.h"

enum class FXType 
{
    LineExplosionParticles,
    LineNeonFlash,
    GhostTrail, 
    GridShokewave
};

class FXEffect
{
protected:
    float lifeTime = 0.0f;
    float maxLifeTime = 0.0f;
    bool isDead = false;
    FXType type;
    glm::vec2 position{0.0f};
    glm::vec3 color{ 1.0f, 1.0f, 1.0f };

public:
    FXEffect(glm::vec2 pos, glm::vec3 col, float duration) 
        : position(pos), color(col), maxLifeTime(duration) {}

    virtual ~FXEffect() = default;

    virtual void Update(float deltaTime)
    {
        if (!PauseManager::Get().IsPaused())
        {
            lifeTime += deltaTime;
            if (lifeTime >= maxLifeTime) isDead = true;
        }
    }

    virtual void Render() {};

    bool IsDead() const { return isDead; }
    FXType GetType() const { return type; };
};
