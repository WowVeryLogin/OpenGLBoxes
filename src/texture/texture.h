#ifndef TEXTURE_H
#define TEXTURE_H
#include <GL/glew.h>
#include <string>

class Texture 
{
public:
    Texture(const std::string& name, const std::string& path);
    GLuint load_texture(const std::string& path);
    std::string name;
    GLuint id;
};

#endif