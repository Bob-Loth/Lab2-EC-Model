#version 330 core
out vec4 color;
in vec3 vertex_normal;
in vec3 vertex_pos;
in vec2 vertex_tex;

uniform float flashAmt;
uniform sampler2D tex;
uniform vec3 flashCol;
void main()
{
	vec3 n = normalize(vertex_normal);
	vec3 lp=vec3(50, 30, 50);
	vec3 ld = normalize(lp - vertex_pos);
	float diffuse = dot(n,ld);
	vec3 tcol= texture(tex, vec2(vertex_tex.x, -vertex_tex.y)).rgb;
	color.rgb = (1- flashAmt) * tcol + flashAmt * flashCol;
	diffuse = clamp(diffuse, 0.1f, 1.0f);
	color *= diffuse;
	// color.rgb = vec3(vertex_tex, 0);
	// color *= 1000;
	color.a = 1;
}