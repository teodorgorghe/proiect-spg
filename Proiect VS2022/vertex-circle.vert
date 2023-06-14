#version 330 core
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 color;
out vec2 position;
out vec3 out_color;

uniform mat4 modelViewProjectionMatrix;

void main()
{
    position = vertex.xy;
    out_color = color;
    gl_Position = vec4(vertex, 1.0f);
}  