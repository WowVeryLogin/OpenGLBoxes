#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <map>
#include <shaderProgram.h>
#include <texture.h>
#include <shaderparam.h>

struct WithVBO
{
    WithVBO(const std::vector<GLfloat>& vertices);
    WithVBO(const WithVBO& w)
        : vertices_size(w.vertices_size),
          VBO(w.VBO)
    {};
    virtual ~WithVBO()
    {};
    virtual void draw_buffer(const GLuint vertices_data_size);
protected:
    using Free = void (*)(GLuint* p);
    static void FreeVBO(GLuint* pVBO);
    size_t vertices_size;
    std::shared_ptr<GLuint> VBO;
};

struct WithEBO : public WithVBO
{
    WithEBO(
        const std::vector<GLfloat>& vertices,
        const std::vector<GLuint>& indices
    );
    WithEBO(WithEBO&& w)
        : WithVBO(std::move(w)),
          indicies_size(w.indicies_size),
          EBO(std::move(w.EBO))
    {};
    void draw_buffer(const GLuint vertices_data_size);
protected:
    static void FreeEBO(GLuint* pEBO);
    size_t indicies_size;
    std::unique_ptr<GLuint, Free> EBO;
};

struct WithTexture
{
    WithTexture(
        std::vector<Texture>&& textures_
    );
    WithTexture(WithTexture&& w)
        : textures(std::move(w.textures))
    {};
    void bind_texture(GLuint program);
    void init_textures(size_t& attrib, size_t& offset, size_t vertices_data_size);
    size_t texture_size();
protected:
    using Free = void (*)(std::vector<Texture>* p);
    static void FreeTexture(std::vector<Texture>* pTexture);
    std::unique_ptr<std::vector<Texture>, Free> textures;
};

struct WithoutTexture
{
    void bind_texture(GLuint program);
    void init_textures(size_t& attrib, size_t& offset, size_t vertices_data_size);
    size_t texture_size();
};

struct WithColor
{
    void init_color(size_t& attrib, size_t& offset, size_t vertices_data_size);
    size_t color_size();
};

struct WithoutColor
{
    void init_color(size_t& attrib, size_t& offset, size_t vertices_data_size);
    size_t color_size();
};

struct WithNormal
{
    void init_normal(size_t& attrib, size_t& offset, size_t vertices_data_size);
    size_t normal_size();
};

struct WithoutNormal
{
    void init_normal(size_t& attrib, size_t& offset, size_t vertices_data_size);
    size_t normal_size();
};

class ContainerObject
{
public:
    ContainerObject(std::map<std::string, std::unique_ptr<ShaderParam>>&& params)
        : params(std::move(params))
    {};
    virtual void draw() = 0;
    virtual ~ContainerObject()
    {};
    std::map<std::string, std::unique_ptr<ShaderParam>> params;
};

template<class EBO = WithVBO, class Textures = WithoutTexture, class Color = WithoutColor, class Normal = WithoutNormal>
class Object : private EBO, private Textures, private Color, private Normal, public ContainerObject
{
public:
    Object(
        const Program& prog,
        const EBO& ebo,
        Textures&& textures,
        std::map<std::string, std::unique_ptr<ShaderParam>>&& params)
        : 
        EBO(ebo),
        Textures(std::move(textures)),
        ContainerObject(std::move(params)),
        program(prog)
    {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        size_t vertices_data_size = 3 + this->color_size() + this->texture_size() + this->normal_size();
        //Init coord
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertices_data_size * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        
        size_t attrib = 1, offset = 3;
        this->init_color(attrib, offset, vertices_data_size);
        this->init_normal(attrib, offset, vertices_data_size);
        this->init_textures(attrib, offset, vertices_data_size);

        glBindVertexArray(0); 
    }

    void draw()
    {
        program.apply();
        glBindVertexArray(VAO);
        this->bind_texture(program.get());
        for (auto& param : params) {
            param.second->pass(program.get());
        }
        this->draw_buffer(3 + this->color_size() + this->texture_size() + this->normal_size());
        glBindVertexArray(0);
    }
    ~Object()
    {
        glDeleteVertexArrays(1, &VAO);
    }
private:
    Program program;
    GLuint VAO;
};

#endif