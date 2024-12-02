#pragma once
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "Shader.h"
#include "Texture.h"
#include "Vertices.h"
#include <iostream>

#define DIAG(program) \
    char infoLog[512]; \
    int success; \
    glGetProgramiv(program, GL_LINK_STATUS, &success); \
    if (!success) { \
        glGetProgramInfoLog(program, 512, NULL, infoLog); \
        std::cout << "ERROR::SHADER::PROGRAM::FAILED\n" << infoLog << std::endl; \
    } \

using std::cout;
using std::endl;

enum Type {
    TRIANGLE_RGB,
    TEXTURE,
    CUBE
};

unsigned compile_vertex_shader();
unsigned compile_fragment_shader();
unsigned link_shaders(unsigned vertex, unsigned fragment);
void setupAttributes(Type type);
void getShaderFiles();

Shader shader;

std::pair<unsigned, unsigned> draw_init(Type type) {
    // grab shaders
    getShaderFiles();

    // draw vertex into viewport
    unsigned vertexShader = compile_vertex_shader();

    // draw colors, lighting, and other frag info
    unsigned fragmentShader = compile_fragment_shader();

    // link all shaders as a pipeline
    unsigned shaderProgram = link_shaders(vertexShader, fragmentShader);


    // create vertex shader via comms with GPU's VBO and VAO
    unsigned VBO_ID, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_ID);
    glGenBuffers(1, &EBO);
    

    // all calls to VBO, attrib, etc inside this VAO
    glBindVertexArray(VAO);

    // bind buffer to array_buf which controls internal VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);

    // copy vertices to the currently bound buffer
    // GL_STREAM_DRAW : the data is set only once and used by the GPU at most a few times.
    // GL_STATIC_DRAW : the data is set only once and used many times.
    // GL_DYNAMIC_DRAW : the data is changed a lot and used many times.

    if (type == TEXTURE) {
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_texture), vertices_texture, GL_STATIC_DRAW);

        // Now make the EBO the active buffer and send data to it
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        // For example, the indices array{ 0, 1, 3, 1, 2, 3 } tells OpenGL to :
        // Draw the first triangle using vertices at indices 0, 1, and 3.
        // Draw the second triangle using vertices at indices 1, 2, and 3.
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_texture), indices_texture, GL_STATIC_DRAW);
    } else if (type == TRIANGLE_RGB) {
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    }
    else if (type == CUBE) {
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_cube), vertices_cube, GL_STATIC_DRAW);
    }

    // this is for changing the color over time (specifically the green val in our case)
    //glUniform4f(glGetUniformLocation(shaderProgram, "changingColor"),
        //0.0f, (sin(glfwGetTime()) / 2), 0.0f, 1.0f);

    // setup vertex attributes (in's passed between shaders)
    setupAttributes(type);

    return {shaderProgram, VAO};
}

void getShaderFiles() {
    shader.add("C:/Users/Aman/source/repos/LearnGL/1.Basics/lib/vert.glsl", "vertex");
    shader.add("C:/Users/Aman/source/repos/LearnGL/1.Basics/lib/frag.glsl", "fragment");
}

unsigned compile_vertex_shader() {
    unsigned vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // shader object meshes with source
    const char* file = shader.vals.at("vertex").c_str();
    glShaderSource(vertexShader, 1, &file, NULL);
    glCompileShader(vertexShader);

    DIAG(vertexShader);

    return vertexShader;
}

unsigned compile_fragment_shader() {
    unsigned fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    const char* file = shader.vals.at("fragment").c_str();
    glShaderSource(fragmentShader, 1, &file, NULL);
    glCompileShader(fragmentShader);

    return fragmentShader;
}

unsigned link_shaders(unsigned vertex, unsigned fragment) {
    unsigned shaderProgram = glCreateProgram();

    // pipeline of shaders
    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragment);
    glLinkProgram(shaderProgram);

    DIAG(shaderProgram);

    // sets current active program for subsequent drawing cmds
    return shaderProgram;
}

void setupAttributes(Type type) {
    // how to interpret VBO object (how our shader language knows to interpret our float[])
    // index : location of vertex attrib (location=0 in test.glsl)
    
    switch (type) {
    case TRIANGLE_RGB:
        // position attrib
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attrib
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        break;  // Add break!

    case TEXTURE:
        // position attrib
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attrib
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // texture coord attrib
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        break;  // Add break!

    case CUBE:
        // position attrib
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // texture coord attrib
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        break;  // Add break!
    }
}