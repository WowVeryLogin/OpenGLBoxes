#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <list>
#include <map>
#include <functional>
#include <type_traits>
#include <memory>
#include <utility>
#include <GLFW/glfw3.h>

class Window {
public:
    Window(const unsigned int width, const unsigned int height);
    ~Window();

    using CursorFunc = std::function<std::remove_pointer_t<GLFWcursorposfun>>;
    using KeyboardFunc = std::function<std::remove_pointer_t<GLFWkeyfun>>;
    using ScrollFunc = std::function<std::remove_pointer_t<GLFWscrollfun>>;

    void register_mouse_cb(CursorFunc&& mouse_func);
    void register_keyboard_cb(KeyboardFunc&& control_func);
    void register_scroll_cb(ScrollFunc&& scroll_func);
    GLFWwindow* get();
    std::pair<int, int> Size();
private:
    template<class ... Args>
    static void key_cb(GLFWwindow* window, Args ... args)
    {
        Window* obj = (Window*)glfwGetWindowUserPointer(window); 
        obj->call_key_cb(window, args...);
    }

    template<class ... Args>
    static void mouse_cb(GLFWwindow* window, Args ... args)
    {
        Window* obj = (Window*)glfwGetWindowUserPointer(window); 
        obj->call_mouse_cb(window, args...);
    }

    template<class ... Args>
    static void scroll_cb(GLFWwindow* window, Args ... args)
    {
        Window* obj = (Window*)glfwGetWindowUserPointer(window); 
        obj->call_scroll_cb(window, args...);
    }

    template<class ... Args>
    void call_scroll_cb(Args ... args)
    {
        for (auto& cb : scroll_cbs) {
            cb(args...);
        }
    }

    template<class ... Args>
    void call_mouse_cb(Args ... args)
    {
        for (auto& cb : mouse_cbs) {
            cb(args...);
        }
    }

    template<class ... Args>
    void call_key_cb(Args ... args)
    {
        for (auto& cb : keyboard_cbs) {
            cb(args...);
        }
    }

    const uint32_t id;
    static uint32_t last_id;
    static std::map<uint32_t, GLFWwindow*> windows;
    std::list<KeyboardFunc> keyboard_cbs;
    std::list<CursorFunc> mouse_cbs;
    std::list<ScrollFunc> scroll_cbs;
    void init_glfw();
};

#endif