#ifndef SHADERPARAM_H
#define SHADERPARAM_H

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class ShaderParam
{
public:
    ShaderParam()
    {};
    virtual ~ShaderParam()
    {};
    virtual void pass(GLuint prog) = 0;
};

template<class Data>
class Param : public ShaderParam
{
public:
    Param()
    {};
    Param(std::string&& name, Data&& data) 
        : data(data),
          name(name)
    {};

private:
    GLuint location(GLuint prog)
    {
        return glGetUniformLocation(prog, name.c_str());
    }

    Data data;
    std::string name;
    void pass(GLuint);
};

#endif