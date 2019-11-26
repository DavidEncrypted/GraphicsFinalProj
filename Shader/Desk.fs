#version 130

// -- Header part --
// Do not change this header! The names are used internally by ShaderLab.

// The camera position in world coordinates
uniform vec3 camera_position;

// Light properties
uniform vec3 light_position; // Position in world coordinates
uniform vec3 light_targetposition; // only important for spot lights
uniform vec3 light_intensity;
uniform vec4 light_params;

// Material properties for Blinn shading
uniform vec3 material_diffuse;
uniform vec3 material_specular;
uniform float material_shininess;
uniform float material_glossiness;

uniform int material_diffuse_usemap; // 0 means no texture is used, non-0 means diffuse_map is valid
uniform sampler2D material_diffuse_map; // The diffuse texture

// -- End of header part --

// Output color
out vec4 out_fragcolor;

// Variables from vertex to fragment shader
in vec3 _position; //position in world coordinates
in vec3 _normal; //normal vector in world coordinates
in vec2 _texcoord; //coordinates of the texture, UV


void main()
{
	vec3 temp_normal = normalize(_normal);
	//vec3 diffuselight = vec3(0.5, 0.5, 0.5);
	//vec3 specularlight = vec3(0.0, 0.0, 0.0);
	vec3 ambientlight = vec3(0.0,0.0,0.0);
	
	vec3 to_light = normalize(light_position - _position);
	vec3 to_viewer = normalize(camera_position - _position);
	

	vec3 light_to_target = normalize(light_targetposition - light_position);
	vec3 light_to_pos = normalize(_position - light_position);

	float S = clamp(dot(light_to_target, light_to_pos),0.0,1.0);
	S = clamp(S - cos(radians(light_params.x)),0.0,1.0);


	// Specular
	vec3 h_vec = normalize(to_light + to_viewer);

	float h_dot = clamp(dot(temp_normal, h_vec), 0.0, 1.0);

	float blinnterm = pow(h_dot, material_shininess);

	vec4 specular_intensity = clamp(vec4(material_specular * light_intensity * blinnterm,1.0),0.0,1.0);


	// Diffuse
	float anglecos = dot(normalize(temp_normal), to_light);

	anglecos = clamp(anglecos, 0.0, 1.0);

	vec4 texturecolor = texture2D(material_diffuse_map, _texcoord);
	
	vec4 diffuse_intensity = texturecolor * vec4(light_intensity,1.0) * anglecos;


	
	out_fragcolor = clamp(S*(specular_intensity + diffuse_intensity) + vec4(ambientlight, 1.0), 0.0,  1.0);

}
