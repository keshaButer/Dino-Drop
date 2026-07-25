#pragma once

struct StateData
{
public:
    int Score = 0;
};

class GameState
{
public:
    virtual void Enter() = 0; 
    virtual void Update(float deltaTime) = 0;
    virtual void Exit() = 0;

    virtual void OnContextRestored() {}

    virtual ~GameState() = default;
};
