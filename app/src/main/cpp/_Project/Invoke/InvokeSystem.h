#pragma once
#include <vector>
#include <functional>

class InvokeSystem 
{
private:

    struct DelayedTask 
    {
        float delay;
        std::function<void()> callback;
    };

    std::vector<DelayedTask> tasks;
    InvokeSystem() = default;

public:
    static InvokeSystem& Get() { static InvokeSystem instance; return instance; }

    void Add(float delayInSeconds, std::function<void()> callback) 
    {
        tasks.push_back({ delayInSeconds, callback });
    }

    void Update(float deltaTime) 
    {
        for (auto it = tasks.begin(); it != tasks.end(); ) 
        {
            it->delay -= deltaTime;

            if (it->delay <= 0.0f) 
            {
                it->callback();
                it = tasks.erase(it);
            } 
            else 
            {
                ++it;
            }
        }
    }
};
