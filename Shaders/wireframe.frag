#version 330 core

uniform float linewidth;
uniform vec4 color;

noperspective in vec3 GEdgeDistance;

void main() {
	// Base frag colour on closest edge
	float distance = min(GEdgeDistance[0], min(GEdgeDistance[1], GEdgeDistance[2]));

//	if (distance==GEdgeDistance[0]) gl_FragColor = vec4(0.52, 0, 0, 1);
//	else if (distance==GEdgeDistance[1]) gl_FragColor = vec4(0, 0.52, 0, 1);
//	else if (distance==GEdgeDistance[2]) gl_FragColor = vec4(0, 0, 0.52, 1);
//	else discard;

	if (distance < linewidth)
		gl_FragColor = color;
	else
		//gl_FragColor = vec4(0,0,0,1);
		discard;
}