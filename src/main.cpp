#include <render.h>
#include <glm/gtc/matrix_transform.hpp>

int main() {
    std::vector<GLfloat> vertices = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    try {
        Window window(800, 600);

        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            fprintf(stderr, "Невозможно инициализировать GLEW\n");
            return -1;
        }

        Program shaderProgram("shaders/light/shader.vert", "shaders/light/shader.frag");
        Program shaderProgramLight("shaders/lamp/shader.vert", "shaders/lamp/shader.frag");
        std::vector<std::unique_ptr<ContainerObject>> objs;

        std::map<std::string, std::unique_ptr<ShaderParam>> obj_params;
        obj_params["model"] = std::make_unique<Param<glm::mat4>>("model", glm::mat4(1.0));
        obj_params["lightColor"] = std::make_unique<Param<glm::vec3>>("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        
        obj_params["light.ambient"] = std::make_unique<Param<glm::vec3>>("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        obj_params["light.diffuse"] = std::make_unique<Param<glm::vec3>>("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        obj_params["light.specular"] = std::make_unique<Param<glm::vec3>>("light.specular", glm::vec3( 1.0f, 1.0f, 1.0f));

        obj_params["material.ambient"] = std::make_unique<Param<glm::vec3>>("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
        //obj_params["material.diffuse"] = std::make_unique<Param<glm::vec3>>("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
        //obj_params["material.specular"] = std::make_unique<Param<glm::vec3>>("material.specular", glm::vec3( 0.5f, 0.5f, 0.5f));
        obj_params["material.shininess"] = std::make_unique<Param<GLfloat>>("material.shininess", 32.0f);

        WithVBO VBO(vertices);
        objs.emplace_back(
            std::make_unique<Object<WithVBO, WithTexture, WithoutColor, WithNormal>>(
                shaderProgram, 
                VBO,
                WithTexture({{"material.diffuse", "static/container2.png"}, {"material.specular", "static/container2_specular.png"}}),
                std::move(obj_params)
            )
        );

        glm::mat4 model(1.0);
        model = glm::translate(model, glm::vec3(1.2f, 1.0f, 2.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        std::map<std::string, std::unique_ptr<ShaderParam>> light_params;
        light_params["model"] = std::make_unique<Param<glm::mat4>>("model", std::move(model));
        objs.emplace_back(
            std::make_unique<Object<WithVBO, WithTexture, WithoutColor, WithNormal>>(
                shaderProgramLight, 
                VBO, 
                WithTexture({{"texture1","static/container2.png"}}),
                std::move(light_params)
            )
        );
        Render(window, objs).Run();
    } catch (const std::exception& e) {
        fprintf(stderr, "%s\n", e.what());
        return -1;
    }

    return 0;
}



//glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS Проверяем нажатие клавиши Escape или закрытие окна
// #include <cmath>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>