#version 460 core

#define MAX_LIGHTS 5
#define POINT 0
#define DIRECTIONAL 1
#define SPOT 2


in vec3 v_position;

in VS_OUT
{
	vec2 texcoord;
	vec3 color;
	vec3 normal;
} fs_in;

out vec4 f_color;


uniform struct Light
{
	int type;
	vec3 position;
	vec3 direction;
	vec3 color;
	float intensity;
	float range;
	float innerSpotAngle;
	float outerSpotAngle;
};

uniform struct Material
{
	sampler2D baseMap;
	vec3 baseColor;

	float shininess;
	vec2 tiling;
	vec2 offset;
}; 

uniform int u_numLights;
uniform Light u_lights[5];
uniform Material u_material;

uniform vec3 u_light_pos;

uniform vec3 u_ambient_light;
uniform sampler2D u_texture;

float calculateAttenuation(in float light_distance, in float range)
{
	float attenuation = clamp(0, (1-light_distance/range), 1);  
	return 1;//attenuation;
}


vec3 calculateLight(in Light light, vec3 position, in vec3 normal)
{
	float attenuation = 1.0;
	vec3 light_dir;
	switch (light.type)
	{
		case POINT:
			{
			light_dir = normalize(light.position - position);
			float light_distance = length(light.position - position);
			attenuation = calculateAttenuation(light_distance, light.range);
			}
		break;
		case DIRECTIONAL:
			light_dir = light.direction;
		break;
		case SPOT:
			{
				vec3 L = normalize(light.position - position);
				float light_distance = length(light.position - position);
				attenuation = calculateAttenuation(light_distance, light.range);

				float angle = acos(dot(light_dir, normalize(light.direction)));
				float spotAttenuation = 0;
				if (angle > light.outerSpotAngle)
					attenuation = 0;
				else {
					spotAttenuation = smoothstep(light.outerSpotAngle + 0.001, light.innerSpotAngle, angle);
					attenuation *= spotAttenuation;
				}	
				
			}
		break;
	}
	//Diffuse lighting
	float NdotL = max(dot(light_dir, normal), 0);
	vec3 diffuse = light.color * u_material.baseColor * NdotL;

	//specular
	vec3 reflection = reflect(-light_dir, normal);
	vec3 view_dir = normalize(-position);

	vec3 halfway_dir = normalize(light_dir + view_dir);
	float NdotH = max(dot(normal, halfway_dir), 0);
	NdotH = pow(NdotH, u_material.shininess);
	vec3 specular = vec3(NdotH);

	/*
	vec3 reflection;
	float specintensity = pow(max(dot(reflection, view_dir), 0), u_material.shininess);
	vec3 specular = vec3(specintensity);
	*/
	

	return (diffuse + specular) * light.intensity * attenuation;
}



void main()
{

	
	vec3 color = u_ambient_light;
	for (int i = 0; i < u_numLights; i++)
	{
		color += calculateLight(u_lights[i], v_position, fs_in.normal);
	}

	f_color = texture(u_material.baseMap, fs_in.texcoord) * vec4(color, 1);
}