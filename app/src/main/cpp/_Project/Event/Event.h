#pragma once
#include <vector>
#include <functional>

template <typename... Args>
class Event
{
public:
    
    void Subscribe(std::function<void(Args...)> callback)
    {
        listeners.push_back(callback);
    }

    // void Unsubscribe(std::function<void(Args...)> callback)
    // {
    //     auto it = std::find(listeners.begin(), listeners.end(), callback);
    //     if (it != listeners.end()) 
    //     {
    //         listeners.erase(it);
    //     }
    // }

    void Invoke(Args... args)
    {
        for (const auto& listener : listeners)
        {
            if (listener)
            {
                listener(args...);
            }
        }
    }

    void Clear()
    {
        listeners.clear();
    }

private:

    std::vector<std::function<void(Args...)>> listeners;
};
