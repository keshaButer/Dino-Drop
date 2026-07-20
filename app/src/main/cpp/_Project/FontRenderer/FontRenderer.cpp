#include "FontRenderer.h"
#include <android/asset_manager.h>
#include "../Engine/engine.h"
#include "../Shaders/ShaderManager.h"
#include <glm/gtc/type_ptr.hpp>
#include "../Core/Config.h"

FontRenderer::FontRenderer(const char* fontPath, int fontSize) : spriteRenderer(ShaderManager::Get().GetShader("Font"))
{
    Initialize(fontPath, fontSize);
}

void FontRenderer::Initialize(const char* fontPath, int fontSize)
{ 
    AAssetManager* assetManager = Engine::Get().GetAssetManager();
    if (!assetManager)
    {
        Engine::Get().PrintError("FontRenderer: Asset manager is null");
        return;
    }

    AAsset* asset = AAssetManager_open(assetManager, fontPath, AASSET_MODE_BUFFER);
    if (!asset) 
    {
        Engine::Get().PrintError("FontRenderer: Failed to open font from APK: %s", fontPath);
        return;
    }

    size_t size = AAsset_getLength(asset);
    fontBuffer.resize(size);

    int readBytes = AAsset_read(asset, fontBuffer.data(), size);
    AAsset_close(asset);

    if (readBytes <= 0)
    {
        Engine::Get().PrintError("FontRenderer: Failed to read bytes from asset: %s", fontPath);
        return;
    }

    if (FT_Init_FreeType(&ft))
        Engine::Get().PrintError("FontRenderer: Could not init FreeType Library");

    if (FT_New_Memory_Face(ft, fontBuffer.data(), fontBuffer.size(), 0, &face))
        Engine::Get().PrintError("FontRenderer: Failed to load font");

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glActiveTexture(GL_TEXTURE0);

    for (GLubyte c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            Engine::Get().PrintError("FontRenderer: Failed to load Glyph");
            continue;
        }

        Unique<Texture> texture = std::make_unique<Texture>();
        glGenTextures(1, &texture->id);
        ShaderManager::Get().BindTexture(texture->id);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        float pixelToWorld = Config::UI::FONT_MULTIPLIER / static_cast<float>(face->size->metrics.height >> 6);
        float pixelBearingX = static_cast<float>(face->glyph->bitmap_left);
        float pixelBearingY = static_cast<float>(face->glyph->bitmap_top);

        float pixelSizeX = static_cast<float>(face->glyph->bitmap.width);
        float pixelSizeY = static_cast<float>(face->glyph->bitmap.rows);

        float pixelAdvance = static_cast<float>(face->glyph->advance.x >> 6);

        Character character = {
            std::move(texture),
            glm::vec2(pixelSizeX, pixelSizeY) * pixelToWorld,
            glm::vec2(pixelBearingX, pixelBearingY) * pixelToWorld,
            pixelAdvance * pixelToWorld
        };

        characters[c] = std::move(character);
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    ShaderManager::Get().BindTexture(0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

void FontRenderer::RenderText(const std::string& text, glm::vec2 position, float scale, glm::vec4 color)
{
    float horizontalOffset = 0.0f;
    glm::vec2 centeredPos = position - glm::vec2(CalculateTextWidth(text, scale), (characters[97].Bearing.y + 0.05f) * scale) * 0.5f;

    for (char c : text)
    {
        Character* character = &characters[c];

        glm::vec2 offset(
            character->Bearing.x * scale + horizontalOffset + (character->Size.x * scale) * 0.5f, 
            (character->Bearing.y - character->Size.y * 0.5f) * scale
        );

        spriteRenderer.Draw(
            centeredPos + offset,
            glm::vec2(character->Size.x, -character->Size.y) * scale,
            0,
            color,
            characters[c].texture.get()
        );

        horizontalOffset += character->Advance * scale;
    }
}

float FontRenderer::CalculateTextWidth(const std::string& text, float scale)
{
    float width = 0.0f;
    for (const char c : text)
    {
        width += characters[c].Advance * scale;
    }

    return width;
}
