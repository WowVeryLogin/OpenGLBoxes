#include <camera.h>
#include <functional>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

Camera::Camera(Window& window)
    : view_matrix(1.0),
      rotation_matrix(1.0),
      last_pos(0.0f, 0.0f),
      window(window)
{
    window.register_mouse_cb([&](GLFWwindow* window, double xpos, double ypos) {
        this->mouse_callback(window, xpos, ypos);
    });
    window.register_keyboard_cb([&](GLFWwindow* window, int key, int scancode, int action, int mode) {
        this->key_callback(window, key, scancode, action, mode);
    });
    window.register_scroll_cb([&](GLFWwindow* window,  double xoffset, double yoffset) {
        this->scroll_callback(window, xoffset, yoffset);
    });
    double x, y;
    glfwGetCursorPos(window.get(), &x, &y);
    last_pos.x = x;
    last_pos.y = y;
}

glm::vec3 Camera::camPos()
{
    return cameraPos;
}

void Camera::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (fov >= 1.0f && fov <= 65.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 65.0f)
        fov = 65.0f;
}

void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    glm::vec2 cur_pos(xpos, ypos);
    glm::vec2 mouse_delta = last_pos - cur_pos;
    last_pos = cur_pos;

    const GLfloat sensitivity = 0.05;

    static GLfloat yaw = 0, pitch = 0;
    yaw   += sensitivity * mouse_delta.x;
    pitch += sensitivity * mouse_delta.y;

    glm::quat qpitch = glm::angleAxis(glm::radians(-pitch), glm::vec3(1.0f, 0.0f, 0.0f)),
              qyaw = glm::angleAxis(glm::radians(-yaw), glm::vec3(0.0f, 1.0f, 0.0f));

    rotation_matrix = glm::mat4_cast(glm::normalize(glm::cross(qpitch, qyaw)));
}

std::pair<glm::mat4, glm::mat4> Camera::apply(GLfloat deltaTime)
{
    do_movement(deltaTime);
    glm::mat4 translate = glm::mat4(1.0f);
    translate = glm::translate(translate, -cameraPos);
    view_matrix = rotation_matrix * translate;
    auto screen = window.Size();
    return std::make_pair(view_matrix, glm::perspective(glm::radians(fov), float(screen.first / screen.second), 0.1f, 100.0f));
}

void Camera::do_movement(GLfloat deltaTime)
{
    // Camera controls
    GLfloat cameraSpeed = 5.0f * deltaTime;
    GLfloat dx, dz;
    if(keys[GLFW_KEY_W]) dz = 2;
    if(keys[GLFW_KEY_S]) dz = -2;
    if(keys[GLFW_KEY_A]) dx = -2;
    if(keys[GLFW_KEY_D]) dx = 2;
    glm::vec3 forward(view_matrix[0][2], view_matrix[1][2], view_matrix[2][2]);
    glm::vec3 strafe(view_matrix[0][0], view_matrix[1][0], view_matrix[2][0]);
    cameraPos += (-dz * forward + dx * strafe) * cameraSpeed;
}

void Camera::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (action == GLFW_PRESS) {
        this->keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        this->keys[key] = false;
    }
}