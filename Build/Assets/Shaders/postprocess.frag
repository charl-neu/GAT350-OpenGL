#version 430

#define GRAYSCALE	(1 << 0)
#define COLORTINT	(1 << 1)
#define INVERSE		(1 << 2)
#define GRAIN 		(1 << 3)
#define SCANLINE	(1 << 4)
#define VIGNETTE	(1 << 5)

// add rest of bit masks

in vec2 v_texcoord;

out vec4 f_color;

// post process controls
uniform uint u_parameters;
uniform vec3 u_colorTint;
uniform float u_time;	// used in random function
uniform float u_blend;  // blend between original and postprocess color

uniform sampler2D u_baseMap;

// There is no random function in GLSL, uses math function to generate psuedo random number
// https://thebookofshaders.com/10/
float random(vec2 st) 
{
	return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
	vec4 color = texture(u_baseMap, v_texcoord);
	vec4 postprocess = color;

	if ((u_parameters & GRAYSCALE) != 0u)	postprocess = vec4(vec3(dot(postprocess.rgb, vec3(0.299, 0.587, 0.114))), 1.0);
	if ((u_parameters & COLORTINT) != 0u)	postprocess = postprocess * vec4(u_colorTint, 1.0);
	if ((u_parameters & INVERSE) != 0u)		postprocess = vec4(1.0) - postprocess;
	if ((u_parameters & GRAIN) != 0u)		postprocess = postprocess + vec4(vec3(random(gl_FragCoord.xy + u_time) * 0.1), 0.0);
	if ((u_parameters & SCANLINE) != 0u)	postprocess = (int(gl_FragCoord.y) % 3 != 0) ? vec4(0, 0, 0, 1) : postprocess;
	if ((u_parameters & VIGNETTE) != 0u)	postprocess = postprocess * vec4(vec3(1.0 - smoothstep(0.5, 0.8, distance(v_texcoord, vec2(0.5)))), 1.0);

	
	f_color = mix(color, postprocess, u_blend); // mix colors using u_blend (0-1)
}