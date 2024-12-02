#pragma once

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <string_view>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>


class Shader
{
public:
    // the program ID
    unsigned int ID;
    std::unordered_map<std::string, std::string> vals;

    // constructor reads and builds the shader
    Shader() = default;
    void add(const char* path, std::string name) {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string code;
        std::ifstream file;
        // ensure ifstream objects can throw exceptions:
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            // open files
            file.open(path);

            std::stringstream stream;
            // read file's buffer contents into streams
            stream << file.rdbuf();

            // close file handlers
            file.close();

            // convert stream into string
            vals[name] = stream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            std::cout << e.what() << std::endl;
        }
    }

    // uniforms
    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
};