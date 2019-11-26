#version 130

// -- Header part --
// Do not change this header! The names are used internally by ShaderLab.

uniform mat4 matmodelview;
uniform mat4 matprojection;

//Vertex shader attributes
in vec4 in_position;
in vec3 in_normal;
in vec2 in_texcoord;

// -- End of header part --

// Variables from vertex to fragment shader
out vec3 _position;
out vec3 _normal;
out vec2 _texcoord;


void main()
{
	// Get a copy of the original position and normal vector
	_position = in_position.xyz;
	_normal = in_normal;
	_texcoord = in_texcoord;
	
	// Transform the vertex position to screencoordinates
	gl_Position = matprojection * (matmodelview * in_position);
}
