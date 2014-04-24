#include <cstdio>

#include "MyWindow.h"
#include "GLFuncs.h"


void MyWindow::displayTimer(int _val)
{
   glutPostRedisplay();
   glutTimerFunc(mDisplayTimeout, refreshTimer, _val);
}

void MyWindow::draw()
{
	glLineWidth(4.0);
	glColor3f( 1.0f, 0.0f, 0.0f );
	glBegin( GL_LINE_LOOP);
	for(int i = 0; i < 360; i+=4){
		double theta = i / 180.0 * M_PI;
		double x = 1 * cos(theta);
		double y = 1 * sin(theta);
		glVertex2d( (GLdouble)((1 >> 1) + x), (GLdouble)((1 >> 1) + y));
	}
	glEnd();
}

void MyWindow::keyboard(unsigned char key, int x, int y)
{
    switch(key){
    case ' ': // use space key to play or stop the motion
       
        break;
    case '1':
       
        break;
    case '2':
      
        break;
    case 'p': // playBack
        
        break;
    default:
        Win3D::keyboard(key,x,y);
    }
    glutPostRedisplay();
}

