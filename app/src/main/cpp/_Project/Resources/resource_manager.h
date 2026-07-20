#pragma once
#include <GLES3/gl3.h>
#include <string>
#include "../Texture/Texture.h"
#include "../Core/Config.h"
#include <unordered_map>
#include "SmartPointers.h"

class ResourceManager
{
public:
    static ResourceManager& Get() { static ResourceManager instance; return instance; }

    void Initialize();
    void Shutdown();
    GLuint GetQuadVAO() { return quadVAO; }

    void LoadTexture(const std::string_view& name)
    {
        std::string path = Config::GetTexturePath(name);
        std::string key = static_cast<std::string>(name);
        if (textures.find(key) != textures.end())
        {
            Engine::Get().PrintError("ResourceManager: failed to load texture, texture with that name already loaded: %s", key.c_str());
            return;
        }

        textures[key] = std::move(std::make_unique<Texture>(path.c_str()));
    }

    void LoadTexture(const char* name, Unique<Texture> texture)
    {
        std::string key = static_cast<std::string>(name);
        if (textures.find(key) != textures.end())
        {
            Engine::Get().PrintError("ResourceManager: failed to load texture, texture with that name already loaded: %s", key.c_str());
            return;
        }

        textures[key] = std::move(texture);
    }

    Texture* GetTexture(const std::string_view& name)
    {
        std::string key = static_cast<std::string>(name);
        if (textures.find(key) == textures.end())
        {
            Engine::Get().PrintError("ResourceManager: failed to give texture, texture with that name not loaded: %s", key.c_str());
            return nullptr;
        }

        return textures[key].get();
    }

private:
    std::unordered_map<std::string, Unique<Texture>> textures;
    GLuint quadVAO = 0, quadVBO = 0, quadEBO = 0;
};
