#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

class Shader
{
public:
    unsigned int ID;
    std::string _path;

    Shader(const std::string path);
    ~Shader();

    unsigned int init_shader(const std::string path);
    void use() const;
    void reload();

    // utility uniform functions
    void set(const std::string &name, bool value) const;
    void set(const std::string &name, int value) const;
    void set(const std::string &name, unsigned int value) const;
    void set(const std::string &name, float value) const;
    void set(const std::string &name, const glm::vec2 &value) const;
    void set(const std::string &name, float x, float y) const;
    void set(const std::string &name, const glm::vec3 &value) const;
    void set(const std::string &name, float x, float y, float z) const;
    void set(const std::string &name, const glm::vec4 &value) const;
    void set(const std::string &name, float x, float y, float z, float w) const;
    void set(const std::string &name, const glm::mat2 &mat) const;
    void set(const std::string &name, const glm::mat3 &mat) const;
    void set(const std::string &name, const glm::mat4 &mat) const;

private:
    // utility function for checking shader compilation/linking errors.
    void check_compile_errors(GLuint shader, std::string type);
};