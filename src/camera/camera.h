#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <window.h>

class Camera {
public:
    Camera(Window& window);
    std::pair<glm::mat4, glm::mat4> apply(GLfloat deltaTime);
    glm::vec3 camPos();
private:
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
    void do_movement(GLfloat deltaTime);

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::mat4 view_matrix;
    glm::mat4 rotation_matrix;
    glm::vec2 last_pos;
    Window& window;
    GLfloat fov = 45.0f;
    bool keys[1024] = {false,};
};

#endif
