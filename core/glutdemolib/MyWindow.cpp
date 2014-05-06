#include <cstdio>

#include "MyWindow.h"
#include "GLFuncs.h"
#include <Eigen/Eigen>
void drawBox(float center[3], float size)
{
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	//glEnable();
	float am[4] = {(float)0.5, (float)0.5, (float)0.5, (float)1};
	glMaterialfv(GL_FRONT, GL_AMBIENT, am);
	float dif[4] = {(float)0.8, (float)0.5, (float)0.5, (float)1};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	float spe[4] = {(float)0.2, (float)0.2, (float)0.2, (float)1};
	glMaterialfv(GL_FRONT, GL_SPECULAR, spe);
	glMateriali(GL_FRONT, GL_SHININESS, 16);

	glBegin(GL_POLYGON);
	size = size*2;
	glNormal3f(0,0,-1);
	glVertex3f(  0.5 * size + center[0],  0.5 * size + center[1], -0.5 * size + center[2] );      // P2 is green
	glVertex3f(  0.5 * size + center[0], -0.5 * size + center[1], -0.5 * size + center[2] );      // P1 is red
	glVertex3f( -0.5 * size + center[0], -0.5 * size + center[1], -0.5 * size + center[2] );      // P4 is purple
	glVertex3f( -0.5 * size + center[0],  0.5 * size + center[1], -0.5 * size + center[2]);      // P3 is blue

	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(0,0,1);
	glVertex3f(  0.5 * size + center[0], -0.5 * size + center[1], 0.5 * size + center[2] );
	glVertex3f(  0.5 * size + center[0],  0.5 * size + center[1], 0.5 * size + center[2] );
	glVertex3f( -0.5 * size + center[0],  0.5 * size + center[1], 0.5 * size + center[2] );
	glVertex3f( -0.5 * size + center[0], -0.5 * size + center[1], 0.5 * size + center[2] );
	glEnd();

	// Purple side - RIGHT
	glBegin(GL_POLYGON);
	glNormal3f(1,0,0);
	glVertex3f( 0.5 * size + center[0], -0.5 * size + center[1], -0.5 * size + center[2] );
	glVertex3f( 0.5 * size + center[0],  0.5 * size + center[1], -0.5 * size + center[2] );
	glVertex3f( 0.5 * size + center[0],  0.5 * size + center[1],  0.5 * size + center[2] );
	glVertex3f( 0.5 * size + center[0], -0.5 * size + center[1],  0.5 * size + center[2] );
	glEnd();

	// Green side - LEFT
	glBegin(GL_POLYGON);
	glNormal3f(-1,0,0);
	glVertex3f( -0.5 * size + center[0], -0.5 * size + center[1],  0.5 * size + center[2] );
	glVertex3f( -0.5 * size + center[0],  0.5 * size + center[1],  0.5 * size + center[2] );
	glVertex3f( -0.5 * size + center[0],  0.5 * size + center[1], -0.5 * size + center[2] );
	glVertex3f( -0.5 * size + center[0], -0.5 * size + center[1], -0.5 * size + center[2] );
	glEnd();

	// Blue side - TOP
	glBegin(GL_POLYGON);
	glNormal3f(0,1,0);
	glVertex3f(  0.5 * size + center[0],  0.5 * size + center[1],  0.5 * size + center[2] );
	glVertex3f(  0.5 * size + center[0],  0.5 * size + center[1], -0.5 * size + center[2] );
	glVertex3f( -0.5 * size + center[0],  0.5 * size + center[1], -0.5 * size + center[2] );
	glVertex3f( -0.5 * size + center[0],  0.5 * size + center[1],  0.5 * size + center[2] );
	glEnd();

	// Red side - BOTTOM
	glBegin(GL_POLYGON);
	glNormal3f(0,-1,0);
	glVertex3f(  0.5 * size + center[0], -0.5 * size + center[1], -0.5 * size + center[2] );
	glVertex3f(  0.5 * size + center[0], -0.5 * size + center[1],  0.5 * size + center[2] );
	glVertex3f( -0.5 * size + center[0], -0.5 * size + center[1],  0.5 * size + center[2] );
	glVertex3f( -0.5 * size + center[0], -0.5 * size + center[1], -0.5 * size + center[2] );
	glEnd();
}

void MyWindow::displayTimer(int _val)
{
	glutPostRedisplay();
	glutTimerFunc(mDisplayTimeout, refreshTimer, _val);
}

void MyWindow::draw()
{
	performPhysics();
	/*if(_shadowOn)
	{
		glCullFace(GL_FRONT);
		renderer->startShadowPass();
		drawSceneSimple();
		renderer->endShadowPass();
		glCullFace(GL_BACK);
		renderer->startScenePass();
		drawScene();
		renderer->endScenePass();
	}else
	{*/
		drawScene();
	//}

}

void MyWindow::performPhysics()
{

}


void MyWindow::drawSceneSimple()
{
	{
		float pos[3] = {0,0.2,0};

		drawBox(pos, 0.1);
	}
	{
		float pos[3] = {-0.3,0.4,0};

		drawBox(pos, 0.1);
	}
	{
		float pos[3] = {0,-5,0};

		drawBox(pos, 5);
	}
	/*glLineWidth(4.0);
	glColor3f( 1.0f, 0.0f, 0.0f );
	glBegin( GL_LINE_LOOP);
	for(int i = 0; i < 360; i+=4){
	double theta = i / 180.0 * M_PI;
	double x = 1 * cos(theta);
	double y = 1 * sin(theta);
	glVertex2d( (GLdouble)((1 >> 1) + x), (GLdouble)((1 >> 1) + y));
	}
	glEnd();*/
}

void MyWindow::drawScene()
{
	{
		float pos[3] = {0,0.2,0};

		drawBox(pos, 0.1);
	}
	{
		float pos[3] = {-0.3,0.4,0};

		drawBox(pos, 0.1);
	}
	{
		float pos[3] = {0,-5,0};

		drawBox(pos, 5);
	}
	/*glLineWidth(4.0);
	glColor3f( 1.0f, 0.0f, 0.0f );
	glBegin( GL_LINE_LOOP);
	for(int i = 0; i < 360; i+=4){
	double theta = i / 180.0 * M_PI;
	double x = 1 * cos(theta);
	double y = 1 * sin(theta);
	glVertex2d( (GLdouble)((1 >> 1) + x), (GLdouble)((1 >> 1) + y));
	}
	glEnd();*/
}

void MyWindow::keyboard(unsigned char key, int x, int y)
{
	switch(key){
	case ' ': // use space key to play or stop the motion

		break;
	case '1':
		//_shadowOn = true;
		break;
	case '2':
		//_shadowOn = false;
		break;
	case 'p': // playBack

		break;
	default:
		Win3D::keyboard(key,x,y);
	}
	glutPostRedisplay();
}

