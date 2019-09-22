#include <object.h>
#include <string>
#include <memory.h>

WithVBO::WithVBO(const std::vector<GLfloat>& vertices)
    : vertices_size(vertices.size()),
      VBO(new GLuint, FreeVBO)
{
    glGenBuffers(1, VBO.get());
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
}

void WithVBO::FreeVBO(GLuint* pVBO)
{
    glDeleteBuffers(1, pVBO);
    delete pVBO;
}

void WithVBO::draw_buffer(const GLuint vertices_data_size)
{
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glDrawArrays(GL_TRIANGLES, 0, (vertices_size/vertices_data_size));
}

WithEBO::WithEBO(
    const std::vector<GLfloat>& vertices,
    const std::vector<GLuint>& indices
) : WithVBO(vertices),
    indicies_size(indices.size()),
    EBO(new GLuint, FreeEBO)
{
    glGenBuffers(1, EBO.get());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

void WithEBO::FreeEBO(GLuint* pEBO)
{
    glDeleteBuffers(1, pEBO);
    delete(pEBO);
}

void WithEBO::draw_buffer(const GLuint vertices_data_size)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glDrawElements(GL_TRIANGLES, indicies_size, GL_UNSIGNED_INT, 0);
}

WithTexture::WithTexture(
    std::vector<Texture>&& textures_
)
    : textures(new std::vector<Texture>, FreeTexture)
{
    for (const auto& texture : textures_) {
        textures->emplace_back(texture);
    }
}

void WithTexture::FreeTexture(std::vector<Texture>* pTexture)
{
    std::vector<GLuint> textures;
    for (const auto& texture : *pTexture) {
        textures.push_back(texture.id);
    }
    glDeleteTextures(textures.size(), textures.data());
    delete pTexture;
}

void WithTexture::bind_texture(GLuint program)
{
    for (size_t i = 0; i < textures->size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, (*textures)[i].id);
        glUniform1i(glGetUniformLocation(program, (*textures)[i].name.c_str()), i);
    }
}

void WithTexture::init_textures(size_t& attrib, size_t& offset, size_t vertices_data_size) {
    glVertexAttribPointer(attrib, 3, GL_FLOAT, GL_FALSE, vertices_data_size * sizeof(GLfloat), (GLvoid*)(offset*sizeof(GLfloat)));
    glEnableVertexAttribArray(attrib++);
    offset += 2;
}

size_t WithTexture::texture_size() {
    return 2;
}

void WithoutTexture::bind_texture(GLuint program)
{};

void WithoutTexture::init_textures(size_t& attrib, size_t& offset, size_t vertices_data_size)
{};

size_t WithoutTexture::texture_size() {
    return 0;
}

void WithColor::init_color(size_t& attrib, size_t& offset, size_t vertices_data_size) {
    glVertexAttribPointer(attrib, 3, GL_FLOAT, GL_FALSE, vertices_data_size * sizeof(GLfloat), (GLvoid*)(offset*sizeof(GLfloat)));
    glEnableVertexAttribArray(attrib++);
    offset += 3;
}

size_t WithColor::color_size() {
    return 3;
}

void WithoutColor::init_color(size_t& attrib, size_t& offset, size_t vertices_data_size) {
}

size_t WithoutColor::color_size() {
    return 0;
}

void WithNormal::init_normal(size_t& attrib, size_t& offset, size_t vertices_data_size) {
    glVertexAttribPointer(attrib, 3, GL_FLOAT, GL_FALSE, vertices_data_size * sizeof(GLfloat), (GLvoid*)(offset*sizeof(GLfloat)));
    glEnableVertexAttribArray(attrib++);
    offset += 3;
}

size_t WithNormal::normal_size() {
    return 3;
}

void WithoutNormal::init_normal(size_t& attrib, size_t& offset, size_t vertices_data_size)
{};

size_t WithoutNormal::normal_size() {
    return 0;
}