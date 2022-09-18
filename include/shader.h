#ifndef __SHADER_H__
#define __SHADER_H__

#include <GL/glew.h>
#include <GL/gl.h>

GLuint shader_create_and_use ( const char* vertex_shader_source,
		const char* fragment_shader_source );

#endif // __SHADER_H__
