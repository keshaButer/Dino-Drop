#include "AudioManager.h"
#include "../Engine/engine.h"
#include <android/asset_manager.h>
#include <algorithm>
#include "../Core/Config.h"
#include "../Pause/PauseManager.h"

void AudioManager::Initialize()
{
    AudioStreamBuilder builder;

    builder.setPerformanceMode(PerformanceMode::LowLatency)
          ->setSharingMode(SharingMode::Exclusive)
          ->setFormat(AudioFormat::Float)
          ->setChannelCount(ChannelCount::Mono)
          ->setSampleRate(44100)
          ->setDataCallback(this);

    Result result = builder.openStream(&stream);

    if (result != Result::OK)
    {
        Engine::Get().PrintError("AudioManager: could not open stream: %s", convertToText(result));
    }

    auto setBufferSizeResult = stream->setBufferSizeInFrames(stream->getFramesPerBurst() * 2);
    if (!setBufferSizeResult)
    {
        // Engine::Get().PrintInfo("AudioManager: new buffer size is %d frames", setBufferSizeResult.value());
        Engine::Get().PrintInfo("AudioManager: failed to set buffer size");
    }

    result = stream->requestStart();

    if (result != Result::OK)
    {
        Engine::Get().PrintError("AudioManager: error starting stream: %s", convertToText(result));
    }
}

DataCallbackResult AudioManager::onAudioReady(AudioStream* oboeStream, void* audioData, int32_t numFrames)
{
    float* outputData = static_cast<float*>(audioData);

    float data[numFrames];
    std::fill(data, data + numFrames, 0.0f);

    bool isPaused = PauseManager::Get().IsPaused();
    for (auto& [name, clip] : audioClips)
    {
        if (clip.isPlaying && ((!clip.isUnstoppable && !isPaused) || clip.isUnstoppable))
        {
            for (int i = 0; i < numFrames; i++)
            {
                int index = static_cast<int>(clip.playbackIndex + i * clip.currentPitch);
                if (index < clip.values.size())
                {
                    data[i] += clip.values[index] * clip.currentVolume;
                }
            }

            clip.playbackIndex += static_cast<float>(numFrames) * clip.currentPitch;
            if (static_cast<int>(clip.playbackIndex) >= clip.values.size())
            {
                clip.isPlaying = false;
                clip.playbackIndex = 0.0f;

                if (clip.isLooped)
                {
                    clip.isPlaying = true;
                }
                else 
                {
                    clip.currentVolume = 1.0f;
                    clip.currentPitch = 1.0f;
                }
            }
        }
    }

    for (int i = 0; i < numFrames; i++)
    {
        outputData[i] = std::clamp(data[i], -1.0f, 1.0f);
    }

    return DataCallbackResult::Continue;
}

void AudioManager::PlayAudioClip(std::string_view viewName, bool loop, float volume, float pitch, bool isUnstoppable)
{
    std::string name = (std::string)viewName;
    auto node = audioClips.find(name);

    if (node == audioClips.end())
    {
        Engine::Get().PrintError("AudioManager: audio clip with this key not loaded: %s", name.c_str());
        return;
    }

    auto& clip = node->second;
    clip.isPlaying = true;
    clip.isLooped = loop;
    clip.playbackIndex = 0.0f;
    clip.currentVolume = volume * 0.4f;
    clip.currentPitch = pitch;
    clip.isUnstoppable = isUnstoppable;
}

void AudioManager::AddAudioClip(std::string name, AudioClip clip)
{
    auto node = audioClips.find(name);

    if (node != audioClips.end())
    {
        Engine::Get().PrintError("AudioManager: can not add clip, that clip name already exists: %s", name.c_str());
        return;
    }

    audioClips[name] = clip;
}

void AudioManager::LoadWavAudioClip(std::string_view viewName)
{
    std::string pathString = Config::GetSoundPathWav(viewName);
    const char* path = pathString.c_str();

    std::string name = (std::string)viewName;
    auto node = audioClips.find(name);

    if (node != audioClips.end())
    {
        Engine::Get().PrintError("AudioManager: can not load clip, that name clip already exists: %s", name.c_str());
        return;
    }

    auto* manager = Engine::Get().GetAssetManager();
    AAsset* asset = AAssetManager_open(manager, path, AASSET_MODE_BUFFER);

    if (!asset)
    {
        Engine::Get().PrintError("AudioManager: could not open asset: %s", path);
    }

    size_t size = AAsset_getLength(asset);
    AAsset_seek(asset, 44, SEEK_SET);

    size_t elementsCount = (size - 44) / sizeof(short);

    short* rawBuffer = new short[elementsCount];
    AAsset_read(asset, rawBuffer, size - 44);

    AudioClip clip
    {
        .isPlaying = false,
        .playbackIndex = 0
    };

    clip.values.resize(elementsCount);
    for (size_t i = 0; i < elementsCount; i++)
    {
        clip.values[i] = static_cast<float>(rawBuffer[i]) / 32768.0f;
    }
    
    audioClips[name] = clip;

    delete[] rawBuffer;
    AAsset_close(asset);
}

void AudioManager::Reset()
{
    for (auto& [name, clip] : audioClips)
    {
        clip.isPlaying = false;
        clip.isLooped = false;
        clip.playbackIndex = 0.0f;
        clip.currentVolume = 1.0f;
        clip.currentPitch = 1.0f;
    }
}

void AudioManager::SetClipPlaying(std::string_view name, bool enabled)
{
    auto it = audioClips.find(static_cast<std::string>(name));

    if (it != audioClips.end())
    {
        it->second.isPlaying = enabled;
    }
    else 
    {
        Engine::Get().PrintError("AudioManager: can not set clip playing, there is no clip with that name");
    }
}

void AudioManager::Exit()
{
    stream->close();    
}
