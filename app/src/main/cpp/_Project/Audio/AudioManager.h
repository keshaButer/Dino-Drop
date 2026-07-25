#pragma once
#include <oboe/Oboe.h>
#include "AudioClip.h"
#include <unordered_map>
#include <string>

using namespace oboe;

class AudioManager : public AudioStreamCallback
{
public:
    static AudioManager& Get() { static AudioManager instance; return instance; }

    void Initialize();
    void Exit();

    void Reset();

    void PlayAudioClip(std::string_view viewName, bool loop = false, float volume = 1.0f, float pitch = 1.0f, bool isUnstoppable = false);

    void AddAudioClip(std::string name, AudioClip clip);
    void LoadWavAudioClip(std::string_view viewName);
    void SetClipPlaying(std::string_view name, bool enabled);

    DataCallbackResult onAudioReady(AudioStream* oboeStream, void* audioData, int32_t numFrames) override;

private:
    AudioStream* stream = nullptr;

    std::unordered_map<std::string, AudioClip> audioClips;
};
