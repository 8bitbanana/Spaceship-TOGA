#version 330 core

uniform float linewidth;
uniform vec4 color;
uniform float time;

noperspective in vec3 GEdgeDistance;

float pulse(float t) {
	float a = (abs(sin(t*1.5)) * 0.7) + 0.3;
	return clamp(a, 0.0, 1.0);
}

void main() {
	// Base frag colour on closest edge
	float distance = min(GEdgeDistance[0], min(GEdgeDistance[1], GEdgeDistance[2]));

//	if (distance==GEdgeDistance[0]) gl_FragColor = vec4(0.52, 0, 0, 1);
//	else if (distance==GEdgeDistance[1]) gl_FragColor = vec4(0, 0.52, 0, 1);
//	else if (distance==GEdgeDistance[2]) gl_FragColor = vec4(0, 0, 0.52, 1);
//	else discard;

	vec4 colorPulse = vec4(color.xyz, pulse(time));

	if (distance < linewidth)
		gl_FragColor = colorPulse;
	else
		//gl_FragColor = vec4(0,0,0,1);
		discard;
}