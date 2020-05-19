#version 330

in vec3 aPos;

uniform mat4 pvm;
uniform vec4 color;

out float CenterDistance_gs;

void main() {
	gl_Position = pvm * vec4(aPos, 1);
	CenterDistance_gs = distance(aPos, vec3(0, 0, -0.5));
}