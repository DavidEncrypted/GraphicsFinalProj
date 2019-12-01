#version 130

out vec4 out_fragcolor;

in vec3 _texcoord;
in vec3 colorfixout;

uniform samplerCube skybox;

void main(){
	//gl_FragColor = texture(skybox, _texcoord);
	vec4 texturecolor = texture(skybox, _texcoord);
	out_fragcolor = vec4(texturecolor.x * colorfixout.x, texturecolor.y * colorfixout.y, texturecolor.z * colorfixout.z, texturecolor.w);
}