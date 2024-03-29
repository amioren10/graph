#include <stdio.h>
#include "GL\glut.h"
#include "Vector3f.h"
GLfloat rot;

void drawSquare(Vector3f color)
{
	glMaterialfv(GL_FRONT, GL_EMISSION, color);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 1);
	glVertex3f(0.0, -1.0f, -1.0);
	glNormal3f(1, 0, 1);
	glVertex3f(1.0, 0.0f, -1.0);
	glNormal3f(0, 0, 1);
	glVertex3f(0.0, 1.0f, -1.0);
	glNormal3f(0, 0, 1);
	glVertex3f(-1.0, 0.0f, -1.0);
	glEnd();
}

void drawTriangle(Vector3f color, Vector3f normal)
{
	glMaterialfv(GL_FRONT, GL_EMISSION, color);
	glNormal3f(normal.x, normal.y, normal.z);
	glBegin(GL_TRIANGLES);
	glVertex3f(-1.0, 1.0f, -1.0);
	glVertex3f(1.0, 1.0f, -1.0);
	glVertex3f(0, -1.0f, -1.0);
	glEnd();
}

void mydisplay()
{
	GLfloat light_position[] = { 0, 0, 3, 1 };
	GLfloat light_direction[] = { 0, 0, -1 };
	GLfloat emission[] = { 0, 0, 1 };
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glRotatef(rot, 0, 1, 0); //rotate scene
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	//draws a purple square on the back wall
	drawSquare(Vector3f(0.3, 0, 0.3));

	//draws a green triangle on the ceiling
	glPushMatrix();
	glRotatef(90, 1, 0, 0);

	drawTriangle(Vector3f(0.0, .5, 0), Vector3f(0, -1, 0));
	glPopMatrix();

	//draws a white sphere in the midle
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	glutSolidSphere(0.5, 32, 32);

	//draws cube
	glMaterialfv(GL_FRONT, GL_EMISSION, Vector3f(1, 1, 1));
	glutWireCube(2.0f);

	glFlush(); //print to screen

}

void disp(int value)
{
	glutPostRedisplay();
	glutTimerFunc(1, disp, 0);
}

void initLight()
{
	//lightning

	GLfloat light_direction[] = { 0, 0.75, -1, 1.0 };
	GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 }; //color
	GLfloat light_diffuse[] = { 0.1, 0.7, 0.7, 1.0 }; //color
	GLfloat light_specular[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat light_position[] = { -0.0, -1.5, 1, 0, 0.0 };
	GLfloat angle[] = { 20.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);
	//glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF,angle);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat mat_a[] = { 0.1, 0.5, 0.5, 1.0 };
	GLfloat mat_d[] = { 0.1, 0.7, 0.7, 1.0 };
	GLfloat mat_s[] = { 1.0, 0.0, 0.9, 1.0 };
	GLfloat low_sh[] = { 5.0 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, low_sh);

	//	glEnable(GL_COLOR_MATERIAL);
	//	glDisable(GL_LIGHTING);

}


void init()
{
	float modelMatrix[16], projectionMatrix[16];
	glClearColor(0, 0, 0, 1);

	glMatrixMode(GL_PROJECTION); /* switch matrix mode */
	glLoadIdentity();		//load Identity matrix

	//defines view mode
	gluPerspective(45, 1, 2, 10);
	//glRotatef(180,0,1,0);
	glTranslatef(0, 0, -6);

	//glTranslatef(0,1,0);
	//gluLookAt(0,-1,-1,0,-2,-2,1,1,0);  //define view direction
	//gluLookAt(0,-1,-1,0,0,0,1,1,0);  //define view direction


	glEnable(GL_DEPTH_TEST);  //define in which order the scene will built
	/* return to modelview mode */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	rot = 0.01;
	initLight();
}



void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		rot = 0;
		break;
	case GLUT_RIGHT_BUTTON:
		if (rot == 0)
			if (x>y)
				rot = 0.01;
			else rot = -0.01;
		else (rot += rot);
		break;
	}
}


int main(int  argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Lighting");


	init();
	glutDisplayFunc(mydisplay);

	glutMouseFunc(mouse);

	glutTimerFunc(2, disp, 0);
	glutMainLoop();
}