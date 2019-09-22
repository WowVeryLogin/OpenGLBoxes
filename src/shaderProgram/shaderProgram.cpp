#include <shaderProgram.h>
#include <stdio.h>
#include <vector>
#include <exception>

std::string Shader::check()
{
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        return "ERROR::COMPILATION_FAILED::SHADER " + std::string(infoLog);
    }
    return "";
}

GLuint Shader::get() {
    return shader;
}

std::vector<char> Shader::read_shader(const std::string& path)
{
    FILE* f = fopen(path.c_str(), "r");
    if (!f) {
        throw std::runtime_error("Cannot open shader file " + path);
    }

    fseek(f, 0, SEEK_END);
    size_t size = ftell(f) + 1;
    fseek(f, 0, SEEK_SET);
    if (!size) {
        throw std::runtime_error("Empty shader file " + path);
    }

    std::vector<char> content(size);
    fread(content.data(), 1, size, f);
    fclose(f);
    return content;
}

Shader::Shader(const int shader_type, const std::string& path)
{
    shader = glCreateShader(shader_type);
    auto shader_source = read_shader(path);
    auto s = shader_source.data();
    glShaderSource(shader, 1, &s, nullptr);
    glCompileShader(shader);
    auto err = check();
    if (!err.empty()) {
        throw std::runtime_error(err);
    }
}

Shader::~Shader()
{
    glDeleteShader(shader);
}

std::string Program::check()
{
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        return "ERROR::LINKING_FAILED::PROGRAM " + std::string(infoLog);
    }
    return "";
}

Program::Program(const GLuint vertex_shader, const GLuint fragment_shader)
{
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    auto err = check();
    if (!err.empty()) {
        throw std::runtime_error(err);
    }
}

Program::Program(const std::string& vertex_shader_path, const std::string& fragment_shader_path)
    : Program(
        Shader(GL_VERTEX_SHADER, vertex_shader_path).get(), 
        Shader(GL_FRAGMENT_SHADER, fragment_shader_path).get()
    )
{
}

void Program::apply()
{
    glUseProgram(program);
}

GLuint Program::get()
{
    return program;
}