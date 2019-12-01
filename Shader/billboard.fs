#version 420

out vec4 out_fragcolor;

in vec2 UV;
in vec4 particlecolor;

layout(binding = 0) uniform sampler2D billboardTextureSampler;
layout(binding = 1) uniform sampler2D smokeTextureSampler;

void main(){
	//out_fragcolor = vec4(0.2, 0.8, 0.2, 1.0);
	
	if (particlecolor.a == 0){
		vec4 color = texture(billboardTextureSampler, UV);
		if (color.a < 0.4) discard;
		out_fragcolor = vec4(particlecolor.xyz,color.a); 
	}
	else{
		vec4 color = texture(smokeTextureSampler, UV);
		if (color.a < 0.2) discard;
		out_fragcolor = vec4(color.xyz,0.01);
	}
	
}
