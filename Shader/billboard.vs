#version 130

// Input vertex data, different for all executions of this shader.
in vec3 in_position;
in vec4 xyzs; // Position of the center of the particule and size of the square
in vec4 color; // Position of the center of the particule and size of the square

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec4 particlecolor;
// Values that stay constant for the whole mesh.
uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;

uniform mat4 matmodelview;
uniform mat4 matprojection;

void main()
{
	float particleSize = xyzs.w; // because we encoded it this way.
	vec3 particleCenter_worldspace = xyzs.xyz;


	vec3 vertexPosition_worldspace = 
	particleCenter_worldspace
	+ CameraRight_worldspace * in_position.x * particleSize
	+ CameraUp_worldspace * in_position.y * particleSize;

	gl_Position = matprojection * (matmodelview * vec4(vertexPosition_worldspace, 1.0f));

	UV = in_position.xy + vec2(0.5, 0.5);
	particlecolor = color;
}