#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <string>
#include <GL/glew.h>

class Shader {
public:
    Shader(const int shader_type, const std::string& path);
    ~Shader();
    GLuint get();
private:
    GLuint shader;
    std::string check();
    std::vector<char> read_shader(const std::string& path);
};

class Program {
public:
    Program(const std::string& vertex_shader_path, const std::string& fragment_shader_path);
    Program(const GLuint vertex_shader, const GLuint fragment_shader);
    void apply();
    GLuint get();
private:
    GLuint program;
    std::string check();
};

#endif