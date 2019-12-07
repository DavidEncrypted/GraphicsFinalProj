#version 420

out vec4 out_fragcolor;

in vec2 UV;
in vec4 particlecolor;
in vec3 _position;

layout(binding = 0) uniform sampler2D billboardTextureSampler;
layout(binding = 1) uniform sampler2D smokeTextureSampler;

// Light properties
//uniform vec3 light_position; // Position in world coordinates
//uniform vec3 light_intensity;

uniform vec4 light_position[10];
uniform vec4 light_intensity[10];

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
		
		vec3 pixcolor = color.xyz;
		
		for (int i = 0; i < 10; i++){
			if (length(light_intensity[i]) > 0.0f){
				float dist = distance(light_position[i].xyz, _position);
				if (dist < 40.0f) pixcolor = pixcolor + (light_intensity[i].xyz);
				else if (dist < 100.0f) pixcolor = pixcolor + 
					(light_intensity[i].xyz * (1.0f-((dist-40.0f) / 40.0f)));
			}
		}

		clamp(pixcolor, 0.0f,1.0f);
		out_fragcolor = vec4(pixcolor,0.05);
	}
	
}
