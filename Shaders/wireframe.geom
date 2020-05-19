#version 330 core

uniform mat4 pvm;

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

noperspective out vec3 GEdgeDistance;

void main() {
	// Get the lengths of each side of the incoming tri
	float a = length(gl_in[1].gl_Position.xyz - gl_in[2].gl_Position.xyz);
	float b = length(gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz);
	float c = length(gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz);

	// angle opposite a 
	float alpha = acos( (b*b + c*c - a*a) / (2.0*b*c) );
	// angle opposite b
	float beta  = acos( (a*a + c*c - b*b) / (2.0*a*c) );

	// Get the minimum distance to each vertex from the opposite edge
	float ha = abs( c * sin( beta ) );
	float hb = abs( c * sin( alpha ) );
	float hc = abs( b * sin( alpha ) );

	GEdgeDistance = vec3(ha, 0, 0);
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	GEdgeDistance = vec3(0, hb, 0);
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	GEdgeDistance = vec3(0, 0, hc);
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();
}