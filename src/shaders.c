
#define PORTABLEGL_IMPLEMENTATION
#include "shaders.h"

// Could/should just use the default shaders in PortableGL for lines
void line_vs(float* vs_output, void* vertex_attribs, Shader_Builtins* builtins, void* uniforms)
{
	builtins->gl_Position = mult_mat4_vec4(*((mat4*)uniforms), ((vec4*)vertex_attribs)[0]);
}

void line_fs(float* fs_input, Shader_Builtins* builtins, void* uniforms)
{
	vec4* fragcolor = &builtins->gl_FragColor;
	//wish I could use a compound literal, stupid C++ compatibility
	fragcolor->x = 0.0f;
	fragcolor->y = 0.0f;
	fragcolor->z = 0.0f;
	fragcolor->w = 1.0f;
}


/*

   The order specified is the order stored
incoming attributes:
	vec4 position
	vec3 normal
	vec4 uv

outgoing parameters: (varying == smooth)
	varying vec2 fragment_uv;
	varying float fragment_ao;
	varying float fragment_light;
	varying float fog_factor;
	varying float fog_height;
	varying float diffuse;


*/
void block_vs(float* vs_output, void* vertex_attribs, Shader_Builtins* builtins, void* uniforms)
{
	//convenience
	vec4* v_attribs = vertex_attribs;
	My_Uniforms* u = uniforms;

	vec4 pos = v_attribs[0];
	builtins->gl_Position = mult_mat4_vec4(u->matrix, pos);

	// outputs, to assign to vs_output
	vec4 uv = v_attribs[2];
	vec2 fragment_uv = { uv.x, uv.y };
	*(vec2*)vs_output = fragment_uv;

	float fragment_ao = 0.3 + (1.0 - uv.z) * 0.7;
	vs_output[2] = fragment_ao;
	float fragment_light = uv.w;
	vs_output[3] = fragment_light;

	const vec3 light_direction = norm_vec3(make_vec3(-1.0, 1.0, -1.0));
	vec3 normal = *(vec3*)&v_attribs[1];
	float tmp = dot_vec3s(normal, light_direction);  // avoid macro duplication
	float diffuse = MAX(0.0, tmp);
	vs_output[6] = diffuse;

	float fog_factor, fog_height;

	if (u->ortho) {
		fog_factor = 0.0;
		fog_height = 0.0;
	} else {
		float camera_distance = distance_vec3(u->camera, make_vec3(pos.x, pos.y, pos.z));
		fog_factor = pow(clamp(camera_distance / u->fog_distance, 0.0, 1.0), 4.0);
		float dy = pos.y - u->camera.y;
		// TODO
		float dx = distance_vec2(make_vec2(pos.x, pos.z), make_vec2(u->camera.x, u->camera.z));
		fog_height = (atan2f(dy, dx) + RM_PI / 2) / RM_PI;
	}
	vs_output[4] = fog_factor;
	vs_output[5] = fog_height;
}


void block_fs(float* fs_input, Shader_Builtins* builtins, void* uniforms)
{
	My_Uniforms* u = uniforms;

	//interpolated variables
	vec2 fragment_uv = *(vec2*)fs_input;
	float fragment_ao = fs_input[2];
	float fragment_light = fs_input[3];
	float fog_factor = fs_input[4];
	float fog_height = fs_input[5];
	float diffuse = fs_input[6];

	// TODO add vector conversion functions to crsw_math
	vec4 tcolor = texture2D(u->sampler, fragment_uv.x, fragment_uv.y);
	vec3 color = { tcolor.x, tcolor.y, tcolor.z };
	if (equal_vec3s(color, make_vec3(1.0, 0.0, 1.0))) {
		builtins->discard = GL_TRUE;
		return;
	}
	int cloud = equal_vec3s(color, make_vec3(1.0,1.0,1.0));
	if (cloud && u->ortho) {
		builtins->discard = GL_TRUE;
		return;
	}

	float df = cloud ? 1.0 - diffuse * 0.2 : diffuse;
	float ao = cloud ? 1.0 - (1.0 - fragment_ao) * 0.2 : fragment_ao;

	// TODO add proper min/max functions to crsw_math
	ao = MIN(1.0, ao + fragment_light);
	df = MIN(1.0, df + fragment_light);
	float value = MIN(1.0, u->daylight + fragment_light);

	float tmp = value * 0.3 + 0.2;
	vec3 light_color = { tmp, tmp, tmp };
	vec3 ambient = light_color;

	vec3 light = add_vec3s(ambient, scale_vec3(light_color, df));

	vec3 cl_ao = { color.x*light.x*ao, color.y*light.y*ao, color.z*light.z*ao };
	// TODO match GLSL where min and max are vec3's as well so you can have different
	// min/max for each field?
	color = clamp_vec3(cl_ao, 0.0f, 1.0f);

	vec3 sky_color = vec4_to_vec3(texture2D(u->sky_sampler, u->timer, fog_height));

	// TODO mix
	color = mix_vec3s(color, sky_color, fog_factor);
	builtins->gl_FragColor = make_vec4(color.x, color.y, color.z, 1.0);
}


// TODO why have uv at 2 if we're not using normal at 1?
void sky_vs(float* vs_output, void* vertex_attribs, Shader_Builtins* builtins, void* uniforms)
{
	vec4* v_attribs = vertex_attribs;
	My_Uniforms* u = uniforms;

	builtins->gl_Position = mult_mat4_vec4(u->matrix, v_attribs[0]);

	vec2 fragment_uv = *(vec2*)&v_attribs[2];
	*(vec2*)vs_output = fragment_uv;
}


void sky_fs(float* fs_input, Shader_Builtins* builtins, void* uniforms)
{
	My_Uniforms* u = uniforms;

	//interpolated variables
	vec2 fragment_uv = *(vec2*)fs_input;

	// only have xyzw not rgba or stpq
	builtins->gl_FragColor = texture2D(u->sampler, u->timer, fragment_uv.y);
}

// can use sky_vs as text_vs
void text_fs(float* fs_input, Shader_Builtins* builtins, void* uniforms)
{
	My_Uniforms* u = uniforms;

	//interpolated variables
	vec2 fragment_uv = *(vec2*)fs_input;

	vec4 ones = { 1.0, 1.0, 1.0, 1.0 };

	vec4 color = texture2D(u->sampler, fragment_uv.x, fragment_uv.y);
	if (u->is_sign) {
		if (equal_vec4s(color, ones)) {
			builtins->discard = GL_TRUE;
			return;
		}
	} else {
		color.w = MAX(color.w, 0.4);
	}
	builtins->gl_FragColor = color;
}

























