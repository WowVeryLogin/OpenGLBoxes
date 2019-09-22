#ifndef RENDER_H
#define RENDER_H

#include <object.h>
#include <camera.h>

class Render {
public:
    Render(Window& window, std::vector<std::unique_ptr<ContainerObject>>& objects);
    void Run();
private:
    Window& window;
    std::vector<std::unique_ptr<ContainerObject>>& objects;
    GLfloat deltaTime = 0.0f;
};

#endif