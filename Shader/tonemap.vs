#version 130

// Do not modify this file!

in vec4 in_position;

out vec2 _texcoord;

void main(){
	gl_Position = in_position;
	_texcoord = in_position.xy * 0.5 + 0.5;
}
