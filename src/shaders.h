#ifndef SHADERS_H
#define SHADERS_H

#include "portablegl.h"

typedef struct My_Uniforms
{
	mat4 matrix;

	vec3 camera;
	int ortho;
	int is_sign;  // bool
	float fog_distance;
	float timer;
	float daylight;

	GLuint sampler;
	GLuint sky_sampler;

	vec3 light_dir;
} My_Uniforms;


// Could/should just use the default shaders in PortableGL for lines
void line_vs(float* vs_output, void* vertex_attribs, Shader_Builtins* builtins, void* uniforms);
void line_fs(float* fs_input, Shader_Builtins* builtins, void* uniforms);

void block_vs(float* vs_output, void* vertex_attribs, Shader_Builtins* builtins, void* uniforms);
void block_fs(float* fs_input, Shader_Builtins* builtins, void* uniforms);

void sky_vs(float* vs_output, void* vertex_attribs, Shader_Builtins* builtins, void* uniforms);
void sky_fs(float* fs_input, Shader_Builtins* builtins, void* uniforms);

// text_vs is sky_vs
void text_fs(float* fs_input, Shader_Builtins* builtins, void* uniforms);

#endif
