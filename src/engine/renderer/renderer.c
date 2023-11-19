#include <glad/glad.h>
#include <cglm/cglm.h>

#include "renderer-internal.h"
#include "renderer.h"
#include "../util/error.h"
#include "../util/file.h"

#include "../engine.h"


static u32 quadVAO;
static u32 quadVBO;
static u32 quadEBO;

static u32 shaderProgram;

static int viewportWidth;
static int viewportHeight;
static f32 viewportAspect;

extern Engine *engine;


void _rendererInit(size_t width, size_t height) {
    glEnable(GL_SCISSOR_TEST);
    glViewport(0, 0, width, height);

    glfwGetWindowSize(engine->window, &viewportWidth, &viewportHeight);
    viewportAspect = (float)viewportWidth / viewportHeight;

    _rendererUpdateViewport(width, height);

    _rendererInitShader();
    _rendererInitMatrices(width, height);

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

    mat4 model;
    glm_mat4_identity(model);

    glm_translate_make(model, (vec3){x, y, 0});
    glm_scale(model, (vec3){w, h, 1});

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);

    glBindVertexArray(quadVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void _rendererInitShader() {
    File vertexShaderSource = engineFileRead("assets/shaders/vertex.glsl");
    if (!vertexShaderSource.isValid) ERROR_EXIT("Failed to read vertex shader file");

    File fragmentShaderSource = engineFileRead("assets/shaders/fragment.glsl");
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

void _rendererInitMatrices(u32 width, u32 height) {
    glUseProgram(shaderProgram);

    mat4 proj;
    glm_mat4_identity(proj);
    glm_ortho(0, width, 0, height, -2, 2, proj);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &proj[0][0]);

    mat4 view;
    glm_mat4_identity(view);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
}


void _rendererUpdateViewport(u32 width, u32 height) {
    f32 newWidth = width, newHeight = height;
    f32 left=0, top=0;

    f32 aspect = (float)width/height;

    if (aspect >= viewportAspect) {
        newWidth = height * viewportAspect;
        left = (width - newWidth) / 2;
    } else {
        aspect = (float)viewportHeight/viewportWidth;
        newHeight = width * aspect;
        top = (height - newHeight) / 2;
    }

    glScissor(left, top, newWidth, newHeight);
    glViewport(left, top, newWidth, newHeight);
}