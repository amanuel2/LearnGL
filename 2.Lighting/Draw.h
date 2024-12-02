#pragma once
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "Shader.h"
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
    LIGHT,
    CUBE
};

typedef struct {
    unsigned shaderColor;
    unsigned shaderLight;
    unsigned VAO_Cube;
    unsigned VAO_Light;
} Draw_t;

unsigned compile_color_shader();
unsigned compile_light_shader();
unsigned link_shaders(unsigned color, unsigned light);
void setupAttributes(Type type);
void getShaderFiles();

void checkShaderCompilation(unsigned int shader) {
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "Shader compilation failed: " << infoLog << std::endl;
    }  else {
        std::cout << " shader compiled successfully" << std::endl;
    }
}


Shader shader;

Draw_t draw_init() {
    // grab shaders
    getShaderFiles();

    // draw vertex into viewport
    unsigned colorShader = compile_color_shader();

    // draw colors, lighting, and other frag info
    unsigned lightShader = compile_light_shader();

   
    unsigned VBO_ID;
    unsigned VAO_Cube, VAO_light;
    glGenBuffers(1, &VBO_ID);

    ////// Cube first
    glGenVertexArrays(1, &VAO_Cube);
    glBindVertexArray(VAO_Cube);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_cube), vertices_cube, GL_STATIC_DRAW);

    glBindVertexArray(VAO_Cube);
    setupAttributes(CUBE);

    ////// Light Cube second
    // we need a new VAO since we will add new vert attrs
    // but same vertices positions so same VBO
    glGenVertexArrays(1, &VAO_light);
    glBindVertexArray(VAO_light);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_cube), vertices_cube, GL_STATIC_DRAW);
    setupAttributes(LIGHT);


    return { colorShader, lightShader, VAO_Cube, VAO_light };
}

void getShaderFiles() {
    shader.add("C:/Users/Aman/source/repos/LearnGL/2.Lighting/light.vs.glsl", "light_vert");
    shader.add("C:/Users/Aman/source/repos/LearnGL/2.Lighting/light.fs.glsl", "light_frag");
    shader.add("C:/Users/Aman/source/repos/LearnGL/2.Lighting/color.vs.glsl", "color_vert");
    shader.add("C:/Users/Aman/source/repos/LearnGL/2.Lighting/color.fs.glsl", "color_frag");
}

unsigned compile_color_shader() {
    // Vertex shader
    unsigned colorVertShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertFile = shader.vals.at("color_vert").c_str();
    glShaderSource(colorVertShader, 1, &vertFile, NULL);
    glCompileShader(colorVertShader);
    checkShaderCompilation(colorVertShader);

    // Fragment shader
    unsigned colorFragShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragFile = shader.vals.at("color_frag").c_str();
    glShaderSource(colorFragShader, 1, &fragFile, NULL);
    glCompileShader(colorFragShader);
    checkShaderCompilation(colorFragShader);

    return link_shaders(colorVertShader, colorFragShader);
}

unsigned compile_light_shader() {
    // Vertex shader
    unsigned lightVertShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertFile = shader.vals.at("light_vert").c_str();
    glShaderSource(lightVertShader, 1, &vertFile, NULL);
    glCompileShader(lightVertShader);
    checkShaderCompilation(lightVertShader);

    // Fragment shader
    unsigned lightFragShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragFile = shader.vals.at("light_frag").c_str();
    glShaderSource(lightFragShader, 1, &fragFile, NULL);
    glCompileShader(lightFragShader);
    checkShaderCompilation(lightFragShader);

    return link_shaders(lightVertShader, lightFragShader);
}

unsigned link_shaders(unsigned vertexShader, unsigned fragmentShader) {
    unsigned shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkShaderCompilation(shaderProgram);

    // Clean up shaders after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void setupAttributes(Type type) {
    /* normal vector (up-vector) to calculate diffused light */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    if (type == CUBE) {
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
}