#define GL_GLEXT_PROTOTYPES 1

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "effect.h"
#include "util.h"

#include <GL/gl.h>
#include <GL/glext.h>

void set_uniform_float(GLuint glsl_program_num, const std::string &prefix, const std::string &key, float value)
{
	std::string name = prefix + "_" + key;
	GLint l = glGetUniformLocation(glsl_program_num, name.c_str());
	if (l == -1) {
		return;
	}
	check_error();
	glUniform1f(l, value);
	check_error();
}

void set_uniform_vec2(GLuint glsl_program_num, const std::string &prefix, const std::string &key, const float *values)
{
	std::string name = prefix + "_" + key;
	GLint l = glGetUniformLocation(glsl_program_num, name.c_str());
	if (l == -1) {
		return;
	}
	check_error();
	glUniform2fv(l, 1, values);
	check_error();
}

void set_uniform_vec3(GLuint glsl_program_num, const std::string &prefix, const std::string &key, const float *values)
{
	std::string name = prefix + "_" + key;
	GLint l = glGetUniformLocation(glsl_program_num, name.c_str());
	if (l == -1) {
		return;
	}
	check_error();
	glUniform3fv(l, 1, values);
	check_error();
}

bool Effect::set_int(const std::string &key, int value)
{
	if (params_int.count(key) == 0) {
		return false;
	}
	*params_int[key] = value;
	return true;
}

bool Effect::set_float(const std::string &key, float value)
{
	if (params_float.count(key) == 0) {
		return false;
	}
	*params_float[key] = value;
	return true;
}

bool Effect::set_vec2(const std::string &key, const float *values)
{
	if (params_vec2.count(key) == 0) {
		return false;
	}
	memcpy(params_vec2[key], values, sizeof(float) * 2);
	return true;
}

bool Effect::set_vec3(const std::string &key, const float *values)
{
	if (params_vec3.count(key) == 0) {
		return false;
	}
	memcpy(params_vec3[key], values, sizeof(float) * 3);
	return true;
}

void Effect::register_int(const std::string &key, int *value)
{
	assert(params_int.count(key) == 0);
	params_int[key] = value;
}

void Effect::register_float(const std::string &key, float *value)
{
	assert(params_float.count(key) == 0);
	params_float[key] = value;
}

void Effect::register_vec2(const std::string &key, float *values)
{
	assert(params_vec2.count(key) == 0);
	params_vec2[key] = values;
}

void Effect::register_vec3(const std::string &key, float *values)
{
	assert(params_vec3.count(key) == 0);
	params_vec3[key] = values;
}

// Output convenience uniforms for each parameter.
// These will be filled in per-frame.
std::string Effect::output_convenience_uniforms()
{
	std::string output = "";
	for (std::map<std::string, float*>::const_iterator it = params_float.begin();
	     it != params_float.end();
	     ++it) {
		char buf[256];
		sprintf(buf, "uniform float PREFIX(%s);\n", it->first.c_str());
		output.append(buf);
	}
	for (std::map<std::string, float*>::const_iterator it = params_vec2.begin();
	     it != params_vec2.end();
	     ++it) {
		char buf[256];
		sprintf(buf, "uniform vec2 PREFIX(%s);\n", it->first.c_str());
		output.append(buf);
	}
	for (std::map<std::string, float*>::const_iterator it = params_vec3.begin();
	     it != params_vec3.end();
	     ++it) {
		char buf[256];
		sprintf(buf, "uniform vec3 PREFIX(%s);\n", it->first.c_str());
		output.append(buf);
	}
	return output;
}

void Effect::set_uniforms(GLuint glsl_program_num, const std::string& prefix)
{
	for (std::map<std::string, float*>::const_iterator it = params_float.begin();
	     it != params_float.end();
	     ++it) {
		set_uniform_float(glsl_program_num, prefix, it->first, *it->second);
	}
	for (std::map<std::string, float*>::const_iterator it = params_vec2.begin();
	     it != params_vec2.end();
	     ++it) {
		set_uniform_vec2(glsl_program_num, prefix, it->first, it->second);
	}
	for (std::map<std::string, float*>::const_iterator it = params_vec3.begin();
	     it != params_vec3.end();
	     ++it) {
		set_uniform_vec3(glsl_program_num, prefix, it->first, it->second);
	}
}
