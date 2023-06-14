#version 330 core
in vec3 vp;
uniform mat4 modelViewProjectionMatrix;
out vec3 position;
layout (location = 1) in vec3 color;
out vec3 outColor;

void main() {
	gl_Position = modelViewProjectionMatrix * vec4(vp, 1.0);
	outColor = color;
}