#pragma once
#include <GLES3/gl3.h>
#include "../Engine/engine.h"
#include <android/asset_manager.h>
#include "../Plugins/stb_image.h"
#include <vector>

class Texture
{
public:
    GLuint id = 0;
    int width = 0;
    int height = 0;

    Texture() = default;

    Texture(const char* texturePath)
    {
        AAssetManager* assetManager = Engine::Get().GetAssetManager();
        if (!assetManager)
        {
            Engine::Get().PrintError("Asset manager is null");
            return;
        }

        AAsset* asset = AAssetManager_open(assetManager, texturePath, AASSET_MODE_BUFFER);
        if (!asset) 
        {
            Engine::Get().PrintError("Failed to open asset from APK: %s", texturePath);
            return;
        }

        size_t size = AAsset_getLength(asset);
        std::vector<unsigned char> fileBuffer(size);

        int readBytes = AAsset_read(asset, fileBuffer.data(), size);
        AAsset_close(asset);

        if (readBytes <= 0)
        {
            Engine::Get().PrintError("Failed to read bytes from asset: %s", texturePath);
            return;
        }

        int nrChannels;
        unsigned char *data = stbi_load_from_memory(fileBuffer.data(), size, &width, &height, &nrChannels, 0);

        if (data == nullptr) 
        {
            Engine::Get().PrintError("stbi_load_from_memory FAILED for: %s! Data pointer is NULL", texturePath);
            return;
        }

        GLint internalFormat;
        GLenum format;

        if (nrChannels == 4) 
        {
            internalFormat = 0x8058;
            format = GL_RGBA;
        } 
        else 
        {
            internalFormat = 0x8051;
            format = GL_RGB;
        }

        glGenTextures(1, &id);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);

        glBindTexture(GL_TEXTURE_2D, 0); 
    }

    ~Texture()
    {
        if (id != 0)
        {
            glDeleteTextures(1, &id);
        }
    }

    void Bind(GLuint slot = 0) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, id);
    }

private:
    
};
