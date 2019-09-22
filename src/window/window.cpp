#include <window.h>

uint32_t Window::last_id = 0;
std::map<uint32_t, GLFWwindow*> Window::windows;

void Window::init_glfw()
{
    //Init window
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x Сглаживание
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Мы хотим использовать OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Мы не хотим старый OpenGL
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

Window::Window(const unsigned int width, const unsigned int height)
    : id(last_id)
{
    if (windows.empty() && !glfwInit()) {
        throw "Ошибка при инициализации GLFW";
    }

    init_glfw();

    auto w = glfwCreateWindow(width, height, "Mess up with OpenGL", nullptr, nullptr);
    if (!w) {
        throw "Невозможно открыть окно GLFW.";
    }
    
    glfwMakeContextCurrent(w);

    glfwSetInputMode(w, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    glfwSetWindowUserPointer(w, this);
    glfwSetKeyCallback(w, Window::key_cb);
    glfwSetCursorPosCallback(w, Window::mouse_cb);
    glfwSetScrollCallback(w, Window::scroll_cb);
    windows.insert(std::make_pair(last_id++, w));
}

void Window::register_scroll_cb(ScrollFunc&& scroll_func)
{
    scroll_cbs.emplace_back(scroll_func);
}

void Window::register_mouse_cb(CursorFunc&& mouse_func)
{
    mouse_cbs.emplace_back(mouse_func);
}

void Window::register_keyboard_cb(KeyboardFunc&& control_func)
{
    keyboard_cbs.emplace_back(control_func);
}

std::pair<int, int> Window::Size()
{
    int width, height;
    glfwGetFramebufferSize(windows[id], &width, &height);
    return std::make_pair(width, height);
}

GLFWwindow* Window::get()
{
    return windows[id];
}

Window::~Window()
{
    windows.erase(id);
    if (windows.empty()) {
        glfwTerminate();
    }
}