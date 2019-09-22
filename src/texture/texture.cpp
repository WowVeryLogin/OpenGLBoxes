#include <texture.h>
#include <SOIL/SOIL.h>

Texture::Texture(const std::string& name, const std::string& path)
    : name(name), id(load_texture(path))
{}

GLuint Texture::load_texture(const std::string& path)
{
    int width, height;
    unsigned char* image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    if (!image) {
        throw std::runtime_error("Cannot load texture");
    }
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}