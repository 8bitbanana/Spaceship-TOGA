#version 330

in vec3 aPos;

uniform mat4 pvm;

void main() {
	gl_Position = pvm * vec4(aPos, 1);
}