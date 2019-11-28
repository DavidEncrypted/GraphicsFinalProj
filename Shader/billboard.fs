#version 140

out vec4 out_fragcolor;

in vec2 UV;

uniform sampler2D billboardTextureSampler;

void main(){
	//out_fragcolor = vec4(0.2, 0.8, 0.2, 1.0);
	out_fragcolor = texture(billboardTextureSampler, UV);
}
