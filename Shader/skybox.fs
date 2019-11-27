#version 130

out vec4 out_fragcolor;

in vec3 _texcoord;

uniform samplerCube skybox;

void main(){
	//gl_FragColor = texture(skybox, _texcoord);
	out_fragcolor = texture(skybox, _texcoord);
}