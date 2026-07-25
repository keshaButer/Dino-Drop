#pragma once
#include <vector>

struct AudioClip
{
    std::vector<float> values;

    bool isPlaying = false;
    bool isLooped = false;
    bool isUnstoppable = false;

    float playbackIndex = 0.0f;

    float currentVolume = 1.0f;
    float currentPitch = 1.0f;
};
