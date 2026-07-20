#ifndef SHADER_H
#define SHADER_H

#pragma once
#include <string>

class Shader 
{
public:
    unsigned int IDprogram = 0;
    unsigned int textureID = 0;
    bool viewUpdate = true;

    void UseProgram();
    void Initialize(const char* vertexPath, const char* fragmentPath);
    void InitializeFromSource(const char* vertexPath, const char* fragmentPath);
    unsigned int createShaderProgramFromSource(const char* vertexSource, const char* fragmentSource);
    void DeleteProgram();
    void LoadTexture(const char* texturePath);
    
private:
    std::string getShaderSource(const char* filePath);
    unsigned int createCompileShader(unsigned int type, const char* shaderSource);
    unsigned int createShaderProgram(const char* vertexPath, const char* fragmentPath);
};

#endif
