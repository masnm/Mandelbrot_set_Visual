#include "../include/shader.h"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

char* create_string_from_file ( const char* file_path )
{
	FILE* fp = fopen ( file_path, "r" );
	if ( fp == NULL ) {
		fprintf ( stderr, "File open failed %s\n", strerror(errno) );
		exit ( 1 );
	}
	if ( fseek ( fp, 0, SEEK_END ) < 0 ) {
		fprintf ( stderr, "File seek_end failed %s\n", strerror(errno) );
		exit ( 1 );
	}

	long size = ftell ( fp );
	if ( size < 0 ) {
		fprintf ( stderr, "File ftell failed %s\n", strerror(errno) );
		exit ( 1 );
	}

	char* buffer = malloc ( size + 1 );
	if ( buffer == NULL ) {
		fprintf ( stderr, "File malloc failed %s\n", strerror(errno) );
		exit ( 1 );
	}

	if ( fseek ( fp, 0, SEEK_SET ) < 0 ) {
		fprintf ( stderr, "File seek_set failed %s\n", strerror(errno) );
		exit ( 1 );
	}

	fread ( buffer, 1, size, fp );
	if ( ferror ( fp ) ) {
		fprintf ( stderr, "File read failed %s\n", strerror(errno) );
		exit ( 1 );
	}

	buffer[ size ] = '\0';
	if ( fp ) {
		fclose ( fp );
		errno = 0;
	}
	return buffer;
}

GLuint shader_create_and_use ( const char* vertex_shader_source_file,
		const char* fragment_shader_source_file )
{
	GLint success;
	GLchar infoLog[512];

	char* vertex_shader_source = create_string_from_file ( vertex_shader_source_file );
	// Vertex Shader
	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource ( vertex, 1, (char const * const *)&vertex_shader_source, NULL);
	glCompileShader ( vertex );
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if ( !success ) {
		glGetShaderInfoLog ( vertex, 512, NULL, infoLog);
		fprintf ( stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED : %s\n", infoLog );
	};

	char* fragment_shader_source = create_string_from_file ( fragment_shader_source_file );
	//fragment Shader
	GLuint fragment = glCreateShader ( GL_FRAGMENT_SHADER );
	glShaderSource ( fragment, 1, (char const* const*)&fragment_shader_source, NULL );
	glCompileShader ( fragment );
	glGetShaderiv ( fragment, GL_COMPILE_STATUS, &success );
	if ( !success ) {
		glGetShaderInfoLog ( fragment, 512, NULL, infoLog);
		fprintf ( stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED : %s\n", infoLog );
	}

	// shader program
	GLuint ID = glCreateProgram ();
	glAttachShader ( ID, vertex );
	glAttachShader ( ID, fragment );
	glLinkProgram ( ID );
	glGetProgramiv ( ID, GL_LINK_STATUS, &success );
	if ( !success ) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		fprintf ( stderr, "ERROR::SHADER::PROGRAM::COMPILATION_FAILED : %s\n", infoLog );
	}

	glUseProgram ( ID );

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	free ( vertex_shader_source );
	free ( fragment_shader_source );

	return ID;
}



