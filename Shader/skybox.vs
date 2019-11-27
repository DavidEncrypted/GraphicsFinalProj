#version 130

// -- Header part --

uniform mat4 matmodelview;
uniform mat4 matprojection;

//Vertex shader attributes
in vec4 in_position;

// -- End of header part --

// Variables from vertex to fragment shader

out vec3 _texcoord;


void main()
{
	_texcoord = in_position.xyz;
	
	// Transform the vertex position to screencoordinates
	//gl_Position = matprojection * (matmodelview * in_position);
	vec4 pos = matprojection * (matmodelview * in_position);
	gl_Position = pos.xyww;
}