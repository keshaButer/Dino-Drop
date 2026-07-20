#include "resource_manager.h"
#include <vector>

void ResourceManager::Initialize()
{
    std::vector<float> vertices = 
    {
        //  X      Y         U     V
        -0.5f, -0.5f,     0.0f, 0.0f,  // Левый нижний
         0.5f, -0.5f,     1.0f, 0.0f,  // Правый нижний
         0.5f,  0.5f,     1.0f, 1.0f,  // Правый верхний
        -0.5f,  0.5f,     0.0f, 1.0f   // Левый верхний
    };

    std::vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0 };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glGenBuffers(1, &quadEBO);

    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    GLsizei stride = 4 * sizeof(float);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));

    glBindVertexArray(0);
}

void ResourceManager::Shutdown() 
{
    if (quadVAO != 0) glDeleteVertexArrays(1, &quadVAO);
    if (quadVBO != 0) glDeleteBuffers(1, &quadVBO);
    if (quadEBO != 0) glDeleteBuffers(1, &quadEBO);
}
