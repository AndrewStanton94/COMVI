#include "stdafx.h"
//#include <glut.h>
//#include <stdlib.h>
//
//static GLfloat spin = 0.0;
//
//void init(void)
//{
//	glClearColor(0.0, 0.0, 0.0, 0.0);
//	glShadeModel(GL_FLAT);
//}
//
//void display(void)
//{
//	glClear(GL_COLOR_BUFFER_BIT);
//	glPushMatrix();
//	glRotatef(spin, 0.0, 0.0, 1.0);
//	glColor3f(0.0, 0.0, 1.0);
//	glRectf(-25.0, -25.0, 25.0, 25.0);
//	glPopMatrix();
//	glutSwapBuffers();
//}
//
//void spinDisplay(void)
//{
//	spin = spin + 2.0;
//	if (spin > 360.0)
//		spin = spin - 360.0;
//	glutPostRedisplay();
//}
//
//void reshape(int w, int h)
//{
//	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//}
//
//void mouse(int button, int state, int x, int y)
//{
//	switch (button) {
//	case GLUT_LEFT_BUTTON:
//		if (state == GLUT_DOWN)
//			glutIdleFunc(spinDisplay);
//		break;
//	case GLUT_RIGHT_BUTTON:
//		if (state == GLUT_DOWN)
//			glutIdleFunc(NULL);
//		break;
//	default:
//		break;
//	}
//}
//
///*
//*  Request double buffer display mode.
//*  Register mouse input callback functions
//*/
//int main(int argc, char** argv)
//{
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
//	glutInitWindowSize(250, 250);
//	glutInitWindowPosition(100, 100);
//	glutCreateWindow(argv[0]);
//	init();
//	glutDisplayFunc(display);
//	glutReshapeFunc(reshape);
//	glutMouseFunc(mouse);
//	glutMainLoop();
//	return 0;
//}


#include <iostream>
#include <string>
#include <time.h>
#include <fstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <glut.h>
#include <stdio.h>

using namespace std;

int mousex = -1, mousey = -1;
int mouserot[4] = { 0 };
int mousetrans[3] = { -100,0,0 };

void mouseMoved(int x, int y)
{
	if (mousex >= 0 && mousey >= 0) {
		mouserot[0] -= y - mousey;
		mouserot[1] += x - mousex;
	}
	mousex = x;
	mousey = y;
}

void mousePress(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mousex = x;
		mousey = y;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		mousex = -1;
		mousey = -1;
	}
}

void DrawCameraPosition(double t[3], double R[3][3], int size)
{
	double xaxis[3] = { 1.0,0.0,0.0 };
	double yaxis[3] = { 0.0,1.0,0.0 };
	double zaxis[3] = { 0.0,0.0,1.0 };
	GLUquadricObj *cyl;

	glPushMatrix();
	glTranslated(t[0], t[1], t[2]);
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(30, 50, 50);
	glPushMatrix();

	double dx, dy, dz;
	dx = xaxis[0];
	dy = xaxis[1];
	dz = xaxis[2];

	xaxis[0] = R[0][0] * dx + R[0][1] * dy + R[0][1] * dz;
	xaxis[1] = R[1][0] * dx + R[1][1] * dy + R[1][2] * dz;
	xaxis[2] = R[2][0] * dx + R[2][1] * dy + R[2][2] * dz;

	cyl = gluNewQuadric();
	gluQuadricDrawStyle(cyl, GLU_FILL);

	double ang = acos(xaxis[2] * size / (sqrt(xaxis[0] * xaxis[0] + xaxis[1] * xaxis[1] + xaxis[2] * xaxis[2])*size)) / M_PI*180.0;
	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);
	glRotated(ang, -xaxis[1] * size, xaxis[0] * size, 0.0);
	gluCylinder(cyl, 5, 15, size * 2, 8, 16);
	glPopMatrix();


	dx = yaxis[0];
	dy = yaxis[1];
	dz = yaxis[2];

	yaxis[0] = R[0][0] * dx + R[0][1] * dy + R[0][1] * dz;
	yaxis[1] = R[1][0] * dx + R[1][1] * dy + R[1][2] * dz;
	yaxis[2] = R[2][0] * dx + R[2][1] * dy + R[2][2] * dz;

	ang = acos(yaxis[2] * size / (sqrt(yaxis[0] * yaxis[0] + yaxis[1] * yaxis[1] + yaxis[2] * yaxis[2])*size)) / M_PI*180.0;
	glPushMatrix();
	glColor3f(0.0, 1.0, 0.0);
	glRotated(ang, -yaxis[1] * size, yaxis[0] * size, 0.0);
	gluCylinder(cyl, 5, 5, size, 8, 8);
	glPopMatrix();

	dx = zaxis[0];
	dy = zaxis[1];
	dz = zaxis[2];

	glBegin(GL_LINES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(size*yaxis[0], size*yaxis[1], size*yaxis[2]);
	glEnd();

	zaxis[0] = R[0][0] * dx + R[0][1] * dy + R[0][1] * dz;
	zaxis[1] = R[1][0] * dx + R[1][1] * dy + R[1][2] * dz;
	zaxis[2] = R[2][0] * dx + R[2][1] * dy + R[2][2] * dz;

	ang = acos(zaxis[2] * size / (sqrt(zaxis[0] * zaxis[0] + zaxis[1] * zaxis[1] + zaxis[2] * zaxis[2])*size)) / M_PI*180.0;
	glPushMatrix();
	glColor3f(0.0, 0.0, 1.0);
	glRotated(ang, -zaxis[1] * size, zaxis[0] * size, 0.0);
	gluCylinder(cyl, 5, 5, size, 8, 8);
	glPopMatrix();


	gluDeleteQuadric(cyl);
}

void DrawGLScene()
{
	int key = 0;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glPushMatrix();
	gluLookAt(0.0, 0.0, mousetrans[0], 0.0, 0.0, 10000.0, 0.0, 1.0, 0.0);


	glRotatef((float)mouserot[0], 1.0f, 0.0f, 0.0f);
	glRotatef((float)mouserot[1], 0.0f, 1.0f, 0.0f);
	//glTranslated(0.0,0.0,);
	glPushMatrix();

	//Kinect 0 Position
	double KR[3][3] = { { 1.0,0.0,0.0 },{ 0.0,1.0,0.0 },{ 0.0,0.0,1.0 } };
	double Kt[3] = { 0.0,0.0,0.0 };
	DrawCameraPosition(Kt, KR, 100);

	glPopMatrix();
	glutSwapBuffers();
}

void keyPressed(unsigned char key, int x, int y)
{
	if (key == 'a') {
		mousetrans[0] -= 30;
	}
	if (key == 'z') {
		mousetrans[0] += 30;
	}
}

void ReSizeGLScene(int Width, int Height)
{
	glViewport(0, 0, Width, Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (double)Width / (double)Height, 10, 10000);
	glMatrixMode(GL_MODELVIEW);
}

void InitGL(int Width, int Height)
{
	glClearColor(0.3f, 0.3f, 0.7f, 0.5f);
	glEnable(GL_DEPTH_TEST);
	ReSizeGLScene(Width, Height);
}

void moveframe()
{
	glutPostRedisplay();
}
int main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(0, 0); //set window position
	glutCreateWindow("Window");
	glutDisplayFunc(&DrawGLScene);
	glutReshapeFunc(&ReSizeGLScene);
	glutKeyboardFunc(&keyPressed);
	glutMotionFunc(&mouseMoved);
	glutMouseFunc(&mousePress);
	glutIdleFunc(moveframe);

	InitGL(640, 480);

	glutMainLoop();
	return 0;
}
