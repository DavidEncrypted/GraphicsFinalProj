#version 130

out vec4 out_fragcolor;

in vec2 UV;
in vec4 particlecolor;
uniform sampler2D billboardTextureSampler;

void main(){
	//out_fragcolor = vec4(0.2, 0.8, 0.2, 1.0);
	vec4 color = texture(billboardTextureSampler, UV);
	if (color.a < 0.4) discard;
	out_fragcolor = vec4(particlecolor.xyz,color.a); 
}
