
#ifndef _DemoWindow_
#define _DemoWindow_

#include <stdarg.h>
#include "glutdemolib/Win3D.h"

class DemoWindow : public Win3D{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	DemoWindow(): Win3D() {
		initWindow(640, 480, "Demo");
		mBackground[0] = 0.1;
		mBackground[1] = 0.3;
		mBackground[2] = 0.5;
		mBackground[3] = 1.0;

		mSim = false;
		mPlay = false;
		mSimFrame = 0;
		mPlayFrame = 0;

		mPersp = 30.f;
		mTrans[2] = -1.f;
	}

	virtual void draw();
	virtual void keyboard(unsigned char key, int x, int y);
	virtual void displayTimer(int _val);
	void performPhysics();
	void drawScene();
	void drawSceneSimple();
protected:	

	int mSimFrame;
	bool mSim;
	int mPlayFrame;
	bool mPlay;

	double mTimeStep;
};

#endif
