
#include "Win3D.h"
#include "Jitter.h"
using namespace Eigen;


void drawArrow(float length, float radius, int nbSubdivisions = 10)
{
	static GLUquadric* quadric = gluNewQuadric();

	if (radius < 0.0)
		radius = 0.05 * length;

	const float head = 2.5*(radius / length) + 0.1;
	const float coneRadiusCoef = 4.0 - 5.0 * head;

	gluCylinder(quadric, radius, radius, length * (1.0 - head/coneRadiusCoef), nbSubdivisions, 1);
	glTranslatef(0.0, 0.0, length * (1.0 - head));
	gluCylinder(quadric, coneRadiusCoef * radius, 0.0, head * length, nbSubdivisions, 1);
	glTranslatef(0.0, 0.0, -length * (1.0 - head));
}

void drawAxis(float length)
{
	const float charWidth = length / 40.0;
	const float charHeight = length / 30.0;
	const float charShift = 1.04 * length;

	GLboolean lighting, colorMaterial;
	glGetBooleanv(GL_LIGHTING, &lighting);
	glGetBooleanv(GL_COLOR_MATERIAL, &colorMaterial);

	glDisable(GL_LIGHTING);

	glBegin(GL_LINES);
	// The X
	glVertex3f(charShift,  charWidth, -charHeight);
	glVertex3f(charShift, -charWidth,  charHeight);
	glVertex3f(charShift, -charWidth, -charHeight);
	glVertex3f(charShift,  charWidth,  charHeight);
	// The Y
	glVertex3f( charWidth, charShift, charHeight);
	glVertex3f(0.0,        charShift, 0.0);
	glVertex3f(-charWidth, charShift, charHeight);
	glVertex3f(0.0,        charShift, 0.0);
	glVertex3f(0.0,        charShift, 0.0);
	glVertex3f(0.0,        charShift, -charHeight);
	// The Z
	glVertex3f(-charWidth,  charHeight, charShift);
	glVertex3f( charWidth,  charHeight, charShift);
	glVertex3f( charWidth,  charHeight, charShift);
	glVertex3f(-charWidth, -charHeight, charShift);
	glVertex3f(-charWidth, -charHeight, charShift);
	glVertex3f( charWidth, -charHeight, charShift);
	glEnd();

	glEnable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);

	float color[4];
	color[0] = 0.7f;  color[1] = 0.7f;  color[2] = 1.0f;  color[3] = 1.0f;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
	drawArrow(length, 0.01*length);

	color[0] = 1.0f;  color[1] = 0.7f;  color[2] = 0.7f;  color[3] = 1.0f;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
	glPushMatrix();
	glRotatef(90.0, 0.0, 1.0, 0.0);
	drawArrow(length, 0.01*length);
	glPopMatrix();

	color[0] = 0.7f;  color[1] = 1.0f;  color[2] = 0.7f;  color[3] = 1.0f;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	drawArrow(length, 0.01*length);
	glPopMatrix();

	if (colorMaterial)
		glEnable(GL_COLOR_MATERIAL);
	if (!lighting)
		glDisable(GL_LIGHTING);
}

Win3D::Win3D()
	:GlutWindow(), mTrans(0.0,0.0,0.0), mEye(0.0,0.0,1.0), mZoom(1.0), mPersp(45.0), mRotate(false), mTranslate(false), mZooming(false)
{}

void Win3D::initWindow(int w, int h, const char* name)
{
	GlutWindow::initWindow(w,h,name);

	int smaller = w<h?w:h;
	mTrackBall.setTrackball(Vector2d(w*0.5,h*0.5), smaller/2.5);
}

void Win3D::resize(int w, int h)
{
	mWinWidth = w;
	mWinHeight = h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, mWinWidth, mWinHeight);
	gluPerspective(mPersp, (double)mWinWidth/(double)mWinHeight, 0.1,100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	int smaller = w<h?w:h;
	mTrackBall.setCenter(Vector2d(w*0.5,h*0.5));
	mTrackBall.setRadius(smaller/2.5);

	glutPostRedisplay();
}

void Win3D::keyboard(unsigned char key, int x, int y)
{
	switch(key){
	case ',': // slow down
		mDisplayTimeout +=2;
		break;
	case '.': // speed up
		mDisplayTimeout -= 2;
		if( mDisplayTimeout <1 )
			mDisplayTimeout = 1;
		break;
	case 'c':
	case 'C': // screen capture
		mCapture = !mCapture;
		break;
	case 27: //ESC
		exit(0);
	}

	glutPostRedisplay();
	//printf("ascii key: %lu\n", key);
}

void Win3D::click(int button, int state, int x, int y)
{
	mMouseDown = !mMouseDown;
	int mask = glutGetModifiers();
	if(mMouseDown){
		if(button == GLUT_LEFT_BUTTON){
			if(mask == GLUT_ACTIVE_SHIFT)
				mZooming = true;
			else{
				mRotate = true;
				mTrackBall.startBall(x,mWinHeight-y);
			}
		}else if(button == GLUT_RIGHT_BUTTON)
			mTranslate = true;
		else if(button == GLUT_MIDDLE_BUTTON)
		{
			mZooming = true;
		}

		mMouseX = x;
		mMouseY = y;
	}else{
		mTranslate = false;
		mRotate = false;
		mZooming = false;
	}
	glutPostRedisplay();
}


//void Win3D::mouseWheel(int button, int dir, int x, int y)
//{
//
//	mZoom += 0.01 * dir;
//	glutPostRedisplay();
//}

void Win3D::drag(int x, int y)
{
	double deltaX = x - mMouseX;
	double deltaY = y - mMouseY;

	mMouseX = x;
	mMouseY = y;

	if(mRotate){
		if(deltaX!=0 || deltaY!=0)
			mTrackBall.updateBall(x,mWinHeight-y);
	}
	if(mTranslate){
		Matrix3d rot = mTrackBall.getRotationMatrix();
		mTrans += rot.transpose()*Vector3d(deltaX, -deltaY, 0.0);
	}
	if(mZooming){
		mZoom += deltaY*0.01;
		if(mZoom < 0.01) mZoom = 0.01;
		if(mZoom > 20) mZoom = 20;
	}
	glutPostRedisplay();
}

void Win3D::render()
{
	if(mCapture){
		capturing();
		glutSwapBuffers();
		screenshot();
		return;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(mPersp , (double)mWinWidth/(double)mWinHeight,0.1,100.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	initGL();
	
	gluLookAt(mEye[0],mEye[1],mEye[2] + mZoom,0.0,0.0,0, 0.0,1.0,0.0);
	mTrackBall.applyGLRotation();
	
	glEnable( GL_DEPTH_TEST );
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_LIGHTING );
	glLineWidth(2.0);
	
	//do not use scaling for eviromememt matrix
	//glScalef(mZoom,mZoom,mZoom); 
	glTranslatef(mTrans[0]*0.001, mTrans[1]*0.001, mTrans[2]*0.001);
	
	//initLights();
	initLights();
	draw();

	
	if(mRotate || mTranslate || mZooming){
		drawAxis(0.2);
	}
	/*if(mRotate)
		mTrackBall.draw(mWinWidth,mWinHeight);*/
	glutSwapBuffers();
}

void Win3D::initGL()
{
	glClearColor(mBackground[0],mBackground[1],mBackground[2],mBackground[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);*/
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POLYGON_SMOOTH);
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT, GL_FILL);

}

void Win3D::initLights()
{
	static float ambient[]             = {0.2, 0.2, 0.2, 1.0};
	static float diffuse[]             = {0.6, 0.6, 0.6, 1.0};
	static float front_mat_shininess[] = {60.0};
	static float front_mat_specular[]  = {0.2, 0.2,  0.2,  1.0};
	static float front_mat_diffuse[]   = {0.5, 0.28, 0.38, 1.0};
	static float lmodel_ambient[]      = {0.2, 0.2,  0.2,  1.0};
	static float lmodel_twoside[]      = {GL_FALSE};

	Eigen::Vector4f position(5.0,20,5,0.0);
	Eigen::Vector4f position1(-5.0,20,-5,0.0);
	Eigen::Matrix4f matrix;
	glGetFloatv(GL_MODELVIEW_MATRIX , matrix.data());
	position = matrix.transpose() * position;
	position1 = matrix * position;
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, &position[0]);

	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT,  lmodel_ambient);
	//glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);

	glEnable( GL_LIGHT1);
	glLightfv(GL_LIGHT1,GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1,GL_POSITION, &position1[0]);
	glEnable(GL_LIGHTING);
	//glEnable(GL_COLOR_MATERIAL);

	//glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, front_mat_shininess);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  front_mat_specular);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   front_mat_diffuse);

	/*glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);*/
	glEnable(GL_NORMALIZE);

}

void accFrustum(GLdouble left, GLdouble right, GLdouble bottom,
	GLdouble top, GLdouble nearPlane, GLdouble farPlane, GLdouble pixdx,
	GLdouble pixdy, GLdouble eyedx, GLdouble eyedy,
	GLdouble focus)
{
	GLdouble xwsize, ywsize;
	GLdouble dx, dy;
	GLint viewport[4];

	glGetIntegerv (GL_VIEWPORT, viewport);

	xwsize = right - left;
	ywsize = top - bottom;
	dx = -(pixdx*xwsize/(GLdouble) viewport[2] + eyedx * nearPlane / focus);
	dy = -(pixdy*ywsize/(GLdouble) viewport[3] + eyedy*nearPlane/focus);

	glFrustum (left + dx, right + dx, bottom + dy, top + dy,
		nearPlane, farPlane);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef (-eyedx, -eyedy, 0.0);
}

void accPerspective(GLdouble fovy, GLdouble aspect,
	GLdouble nearPlane, GLdouble farPlane, GLdouble pixdx, GLdouble pixdy,
	GLdouble eyedx, GLdouble eyedy, GLdouble focus)
{
	GLdouble fov2,left,right,bottom,top;
	fov2 = ((fovy*M_PI) / 180.0) / 2.0;

	top = nearPlane / (cosf(fov2) / sinf(fov2));
	bottom = -top;
	right = top * aspect;
	left = -right;

	accFrustum (left, right, bottom, top, nearPlane, farPlane,
		pixdx, pixdy, eyedx, eyedy, focus);
}

void Win3D::capturing()
{
	glClear(GL_ACCUM_BUFFER_BIT);
	for(int jitter=0; jitter<4; jitter++){
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		accPerspective(mPersp, (double)mWinWidth/mWinHeight,0.1,10.0, j4[jitter].x, j4[jitter].y, 0.0, 0.0, 1.0);
		gluLookAt(mEye[0], mEye[1], mEye[2],0.0,0.0,-1.0,0.0,1.0,0.0);

		initGL();

		mTrackBall.applyGLRotation();
		glScalef(mZoom,mZoom,mZoom);
		glTranslatef(mTrans[0]*0.001, mTrans[1]*0.001, mTrans[2]*0.001);

		initLights();
		draw();

		glAccum(GL_ACCUM, 0.25);
	}
	glAccum( GL_RETURN, 1.0);
}
