#version 330 core

in vec3 vertexColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform float mixParam;

void main()
{
    color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.5) * vec4(vertexColor, 1.0f);
}