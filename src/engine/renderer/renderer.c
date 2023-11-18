#include <glad/glad.h>

#include "renderer-internal.h"
#include "renderer.h"
#include "../util/error.h"
#include "../util/file.h"


u32 quadVAO;
u32 quadVBO;
u32 quadEBO;

u32 shaderProgram;

void _rendererInit(size_t width, size_t height) {
    glViewport(0, 0, width, height);

    _rendererInitShader();
    _rendererInitQuad();
}

void _rendererInitQuad() {
    f32 vertices[] = {
         0.5f,  0.5f,  
         0.5f, -0.5f,  
        -0.5f, -0.5f, 
        -0.5f,  0.5f
    };

    unsigned int indices[] = { 
        0, 1, 3,   
        1, 2, 3   
    };  

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glGenBuffers(1, &quadEBO);

    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  
}

void engineRendererDrawQuad(f32 x, f32 y, f32 w, f32 h) {
    glUseProgram(shaderProgram);
    glBindVertexArray(quadVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void _rendererInitShader() {
    File vertexShaderSource = engineFileRead("res/shaders/vertex.glsl");
    if (!vertexShaderSource.isValid) ERROR_EXIT("Failed to read vertex shader file");

    File fragmentShaderSource = engineFileRead("res/shaders/fragment.glsl");
    if (!fragmentShaderSource.isValid) ERROR_EXIT("Failed to read fragment shader file");

    int success;
    char infoLog[512];

    u32 vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const char* const*)&vertexShaderSource.data, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        ERROR_EXIT("The compilation of the vertex shader failed: %s\n", infoLog);
    }

    u32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const char* const*)&fragmentShaderSource.data, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        ERROR_EXIT("The compilation of the fragment shader failed: %s\n", infoLog);
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        ERROR_EXIT("The linking of the program failed: %s\n", infoLog);
    }

    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}