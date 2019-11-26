#version 130

// Do not modify this file!

uniform sampler2D screentex;

out vec4 out_fragcolor;

in vec2 _texcoord;

void main(){
	gl_FragColor = pow(texture(screentex, _texcoord), vec4(1.0 / 2.2));
}
