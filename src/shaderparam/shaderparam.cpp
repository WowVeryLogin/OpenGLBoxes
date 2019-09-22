#include <shaderparam.h>
#include <glm/gtc/type_ptr.hpp>

template<>
void Param<GLfloat>::pass(GLuint prog)
{
    glUniform1f(location(prog), data);
}

template<>
void Param<glm::vec3>::pass(GLuint prog)
{
    glUniform3f(location(prog), data.x, data.y, data.z);
}

template<>
void Param<glm::mat4>::pass(GLuint prog)
{
    glUniformMatrix4fv(location(prog), 1, GL_FALSE, glm::value_ptr(data));
}