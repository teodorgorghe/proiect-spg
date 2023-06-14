#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 in_color;

flat out vec3 startPos;
out vec3 vertPos;
out vec3 color;

uniform mat4 modelViewProjectionMatrix;

void main()
{
    vec4 pos    = modelViewProjectionMatrix * vec4(inPos, 1.0);
    gl_Position = pos;
    vertPos     = pos.xyz / pos.w;
    startPos    = vertPos;
    color = in_color;
}