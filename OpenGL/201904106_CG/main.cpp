#include <stdio.h>
#include <Windows.h>
#include "GL\glew.h"
#include "GL\glut.h"
#include "Mesh.h"
#include "Skybox.h"
#include "GL\glaux.h"

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glut32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glaux.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")

float _zoom = 15.0f;
float _rotate_x = 0.0f;
float _rotate_y = 0.001f;
float _translate_x = 0.0f;
float _translate_y = 0.0f;
int last_x = 0;
int last_y = 0;
unsigned char _btnStates[3] = { 0 };
Mesh *_mesh;
bool _smoothing = false;
bool _blend = false;
bool _wire = false;
bool _point = false;
Skybox* skybox;	


void init(void)
{
	_mesh = new Mesh("obj\\Ironman.obj", "obj\\Ironman_d.jpg");
	glEnable(GL_DEPTH_TEST);
}

void light(void)
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat position0[4] = { 2.0, 2.0, 2.0, 1.0 };
	GLfloat ambient0[4] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat diffuse0[4] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat specular0[4] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, position0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.2);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.05);				
}

void draw(void)
{
	skybox->draw(_blend);
	light();
	_mesh->drawSurface(_smoothing);
	if (_wire)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (_point)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	glutPostRedisplay();
	glEnable(GL_LIGHTING);

}

void GL_Display(void)
{
	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -_zoom);
	glTranslatef(_translate_x, _translate_y, 0.0);
	glRotatef(_rotate_x, 1, 0, 0);
	glRotatef(_rotate_y, 0, 1, 0);

	draw();
	glutSwapBuffers();
}

void GL_Reshape(int w, int h)
{
	if (w == 0) {
		h = 1;
	}
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)w / (float)h, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}

void GL_Motion(int x, int y)
{
	int diff_x = x - last_x;
	int diff_y = y - last_y;

	last_x = x;
	last_y = y;

	if (_btnStates[2]) {
		_zoom -= (float)0.05f * diff_x;
	}
	else if (_btnStates[0]) {
		_rotate_x += (float)0.5f * diff_y;
		_rotate_y += (float)0.5f * diff_x;
	}
	else if (_btnStates[1]) {
		_translate_x += (float)0.05f * diff_x;
		_translate_y -= (float)0.05f * diff_y;
	}
	glutPostRedisplay();
}

void GL_Mouse(int button, int state, int x, int y)
{
	last_x = x;
	last_y = y;
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		_btnStates[0] = ((GLUT_DOWN == state) ? 1 : 0);
		break;
	case GLUT_MIDDLE_BUTTON:
		_btnStates[1] = ((GLUT_DOWN == state) ? 1 : 0);
		break;
	case GLUT_RIGHT_BUTTON:
		_btnStates[2] = ((GLUT_DOWN == state) ? 1 : 0);
		break;
	default:
			break;
	}
	glutPostRedisplay();
}

void GL_Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'Q':
	case 'q':
		exit(0);
	case 'w':
	case 'W':
		_wire = !_wire;
		break;
	case 's':
	case 'S':
		_smoothing = !_smoothing;
		_blend = !_blend;
		break;
	case 'p':
	case 'P':
		_point = !_point;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}


void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("201904106 Final Project");
	skybox = new Skybox();
	glutDisplayFunc(GL_Display);
	glutReshapeFunc(GL_Reshape);
	glutMouseFunc(GL_Mouse);
	glutMotionFunc(GL_Motion);
	glutKeyboardFunc(GL_Keyboard);
	init();
	glutMainLoop();
}