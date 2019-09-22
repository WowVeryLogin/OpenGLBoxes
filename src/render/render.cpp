#include <render.h>
#include <functional>
#include <glm/gtc/matrix_transform.hpp>

Render::Render(Window& window, std::vector<std::unique_ptr<ContainerObject>>& objects)
    : window(window),
      objects(objects)
{
    auto size = window.Size();
    glViewport(0, 0, size.first, size.second);
    glEnable(GL_DEPTH_TEST);

    window.register_keyboard_cb(
        [&](GLFWwindow* window, int key, int scancode, int action, int mode) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    });
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
}

void Render::Run()
{
    //GLfloat radius = 10.0f;
    GLfloat lastFrame = 0.0f;
    Camera camera(window);

    std::vector<glm::vec3> cubePositions{
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    while (!glfwWindowShouldClose(window.get())) {
        glfwPollEvents();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //float mix_value = 0;
        //GLfloat lightX = sin(glfwGetTime()) * radius;
        //GLfloat lightZ = cos(glfwGetTime()) * radius;
        glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);//glm::vec3(lightX, 1.0f, lightZ);////
        auto cam_matrixes = camera.apply(deltaTime);

        objects[1]->params["model"] = std::make_unique<Param<glm::mat4>>("model", glm::scale(glm::translate(glm::mat4(1.0f), lightPos), glm::vec3(0.2f)));

        for (size_t i = 0; i < objects.size(); ++i) {
            if (i == 0) {
                for (auto& pos : cubePositions) {
                    objects[i]->params["model"] = std::make_unique<Param<glm::mat4>>("model", glm::translate(glm::mat4(1.0f), pos));
                    objects[i]->params["light.position"] = std::make_unique<Param<glm::vec3>>("light.position", std::move(lightPos));
                    objects[i]->params["viewPos"] = std::make_unique<Param<glm::vec3>>("viewPos", camera.camPos());
                    objects[i]->params["projection"] = std::make_unique<Param<glm::mat4>>("projection", std::move(cam_matrixes.second));
                    objects[i]->params["view"] = std::make_unique<Param<glm::mat4>>("view", std::move(cam_matrixes.first));
                    objects[i]->draw();
                }
            } else {
                objects[i]->params["light.position"] = std::make_unique<Param<glm::vec3>>("light.position", std::move(lightPos));
                objects[i]->params["viewPos"] = std::make_unique<Param<glm::vec3>>("viewPos", camera.camPos());
                objects[i]->params["projection"] = std::make_unique<Param<glm::mat4>>("projection", std::move(cam_matrixes.second));
                objects[i]->params["view"] = std::make_unique<Param<glm::mat4>>("view", std::move(cam_matrixes.first));
                objects[i]->draw();
            }
        }

        glfwSwapBuffers(window.get());
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // GLfloat angle = 20.0f * (i+1); 
        // model = glm::rotate(model, ((i+1)%2 * (GLfloat)glfwGetTime()) * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        // GLfloat timeValue = glfwGetTime();
        // GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
        // GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        //GLuint vertexMixLocation = glGetUniformLocation(shaderProgram, "mixParam");
        //GLuint transform = glGetUniformLocation(shaderProgram, "transform");
        //glUniform1f(vertexMixLocation, mix_value);
        //glUniformMatrix4fv(transform, 1, GL_FALSE, glm::value_ptr(trans));
        //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
    }
}
