#include <stdio.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "../include/shader.h"
#include "../include/linear_algebra.h"

GLuint EBO, VBO, VAO;
GLuint ID;

typedef enum {
	pos = 0,
	uv,
	colour,
	attribs_count
} Attribs ;

typedef struct {
	vec2f pos;
	vec2f uv;
	vec4f colour;
} vertex ;

#define VERTEX_ARRAY_SIZE 2048
vertex vertexes[VERTEX_ARRAY_SIZE];
uint32_t vertex_array_sz = 0;
#define INDEX_ARRAY_SIZE 2048
GLuint indexes[INDEX_ARRAY_SIZE];
uint32_t index_array_sz = 0;

void push_quad ( vertex tl, vertex br ) // vec2f tl, vec2f br )
{
	vertex a = tl;
	vertex b = (vertex){ (vec2f){.x=br.pos.x,.y=tl.pos.y}, (vec2f){.x=br.uv.x,.y=tl.uv.y}, (vec4f){0} };
	vertex c = br;
	vertex d = (vertex){ (vec2f){.x=tl.pos.x,.y=br.pos.y}, (vec2f){.x=tl.uv.x,.y=br.uv.y}, (vec4f){0} };
	vertexes[vertex_array_sz++] = a;
	vertexes[vertex_array_sz++] = b;
	vertexes[vertex_array_sz++] = c;
	vertexes[vertex_array_sz++] = d;
	indexes[index_array_sz++] = 0;
	indexes[index_array_sz++] = 1;
	indexes[index_array_sz++] = 2;
	indexes[index_array_sz++] = 3;
	indexes[index_array_sz++] = 2;
	indexes[index_array_sz++] = 0;
}

void pop_quad ( void )
{
	vertex_array_sz = 0;
	index_array_sz = 0;
}

void sync_vertex_buffer ( void )
{
	glBufferSubData ( GL_ARRAY_BUFFER, 0, sizeof(vertex) * VERTEX_ARRAY_SIZE, vertexes );
	glBufferSubData ( GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint)*INDEX_ARRAY_SIZE, indexes );
}

GLint time_uni, resolution_uni, fix_point_uni, scale_uni, offset_uni;
void init_one_uniform ( GLint* uni, const char* name )
{
	*uni = glGetUniformLocation(ID,name);
	if ( *uni == -1 ) {
		fprintf ( stderr, "%s uniform location retrive failed.\n", name );
	}
}
void init_gl_unifomrs ( void )
{
	init_one_uniform ( &time_uni, "vtime" );
	init_one_uniform ( &resolution_uni, "vresolution" );
	init_one_uniform ( &fix_point_uni, "fix_point" );
	init_one_uniform ( &scale_uni, "scale" );
	init_one_uniform ( &offset_uni, "offset" );
}

bool mouse_held = false;
float current_max_x, current_max_y;
float xlow = -2.00, xhigh = 0.47;
float ylow = -1.12, yhigh = 1.12;
int old_mx, old_my;

void process_Normal_Keys(int key, int x, int y)
{
	if ( key == 101 ) { // up arrow
	}
	if ( key == 103 ) { // down arrow
	}
}

void normalkeyfun ( unsigned char key, int x, int y )
{
	if ( key == 'r' ) {  };
}

void MouseButton ( int key, int state, int x, int y )
{
	if ( state == GLUT_DOWN ) {
		old_mx = x; old_my = y;
		mouse_held = true;
	} else if ( state == GLUT_UP ) {
		mouse_held = false;
	}
}

void MouseMotion ( int x, int y )
{
	if ( mouse_held ) {
		float cx = old_mx - x;
		float sx = (cx/current_max_x)*(xhigh-xlow);
		xlow += sx; xhigh += sx;
		float cy = old_my - y;
		float sy = (cy/current_max_x)*(yhigh-ylow);
		ylow += sy; yhigh += sy;
	}
	old_mx = x; old_my = y;
}

vertex tl, br;
static void display ( void )
{
	// setting panning zooming variable
	current_max_x = (float)glutGet(GLUT_WINDOW_WIDTH);
	current_max_y = (float)glutGet(GLUT_WINDOW_HEIGHT);
	pop_quad ();
	tl.pos = (vec2f) { -1.0, 1.0 }; br.pos = (vec2f) { 1.0, -1.0 };
	tl.uv = (vec2f) { xlow, ylow }; br.uv = (vec2f) { xhigh, yhigh };
	push_quad ( tl, br );
	sync_vertex_buffer ();

	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram ( ID );
	glUniform1f( time_uni, (float)glutGet(GLUT_ELAPSED_TIME) / 1000.0f );
	glUniform2f( resolution_uni, (float)glutGet(GLUT_WINDOW_WIDTH), (float)glutGet(GLUT_WINDOW_HEIGHT) );
	//glUniform2f( fix_point_uni, fix_point_x, fix_point_y );
	glUniform2f( scale_uni, 1, 1 );
	glUniform2f( offset_uni, 1, 1 );
	//glUniform2f( resolution_uni, 800, 800 );
	glDrawElements ( GL_TRIANGLES, index_array_sz, GL_UNSIGNED_INT, (void*)0 );
	//glDrawArrays( GL_TRIANGLES, 0, vertex_array_sz );
	glutSwapBuffers();
}

static void idle ( void ) { glutPostRedisplay (); }

int main ( int argc, char* argv[] )
{
	glutInit ( &argc, argv );
	glutInitWindowSize ( 741, 672 );
	glutInitWindowPosition ( 100, 100 );
	glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE);

	int win = glutCreateWindow("Drawing Library Demo");

	if ( glewInit () != GLEW_OK ) {
		fprintf ( stderr, "GLEW INIT failed.\n" );
	}

	ID = shader_create_and_use ( "./shaders/vertext.glsl", "./shaders/fragment.glsl" );

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, VERTEX_ARRAY_SIZE*sizeof(vertex), vertexes, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray ( pos );
	glVertexAttribPointer( pos, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex,pos) );
	glEnableVertexAttribArray ( uv );
	glVertexAttribPointer( uv, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex,uv) );
	glEnableVertexAttribArray ( colour );
	glVertexAttribPointer( colour, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex,colour) );

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDEX_ARRAY_SIZE*sizeof(GLuint), indexes, GL_DYNAMIC_DRAW);

	init_gl_unifomrs ();

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutSpecialFunc( process_Normal_Keys );
	glutKeyboardFunc( normalkeyfun );
	glutMouseFunc (MouseButton);
	glutMotionFunc ( MouseMotion);

	tl.pos = (vec2f) { -1.0, 1.0 }; br.pos = (vec2f) { 1.0, -1.0 };
	tl.uv = (vec2f) { 0, 0 }; br.uv = (vec2f) { 0.8f, 0.45f };
	push_quad ( tl, br );

	sync_vertex_buffer ();

	printf ( "%s \n", glGetString(GL_VERSION) );

	glutMainLoop();

	glDeleteProgram ( ID );
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glutDestroyWindow ( win );

	return 0;
}
