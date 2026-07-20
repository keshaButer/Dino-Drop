#pragma
#include <vector>
#include <SmartPointers.h>
#include "FXEffect.h"

class FXSystem
{
private:
    std::vector<Unique<FXEffect>> activeEffects;
    std::vector<Unique<FXEffect>> lateActiveEffects;
    FXSystem() = default;

public:
    static FXSystem& Get() { static FXSystem instance; return instance; }

    void Update(float deltaTime)
    {
        for (auto it = activeEffects.begin(); it != activeEffects.end(); ) 
        {
            (*it)->Update(deltaTime);

            if ((*it)->IsDead())
            {
                it = activeEffects.erase(it);
            }
            else 
            {
                it++;
            }
        }

        for (auto it = lateActiveEffects.begin(); it != lateActiveEffects.end(); ) 
        {
            (*it)->Update(deltaTime);

            if ((*it)->IsDead()) 
            {
                it = lateActiveEffects.erase(it);
            }
            else 
            {
                it++;
            }
        }
    }

    void Render() 
    {
        for (const auto& effect : activeEffects) 
        {
            effect->Render();
        }
    }

    void LateRender() 
    {
        for (const auto& effect : lateActiveEffects) 
        {
            effect->Render();
        }
    }

    void AddEffect(std::unique_ptr<FXEffect> effect) 
    {
        activeEffects.push_back(std::move(effect));
    }

    void AddLateEffect(std::unique_ptr<FXEffect> effect) 
    {
        lateActiveEffects.push_back(std::move(effect));
    }
};
