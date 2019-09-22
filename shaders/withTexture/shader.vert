#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texture;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 vertexColor;
out vec2 TexCoord;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    vertexColor = color; // Устанавливаем значение цвета, полученное от вершинных данных
    TexCoord = vec2(texture.x, 1.0f - texture.y);
}