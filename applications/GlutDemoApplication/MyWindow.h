
#ifndef _MYWINDOW_
#define _MYWINDOW_

#include <stdarg.h>
#include "Win3D.h"


class MyWindow : public Win3D{
public:
 MyWindow(): Win3D() {
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


 protected:	

    int mSimFrame;
    bool mSim;
    int mPlayFrame;
    bool mPlay;
 
    double mTimeStep;

};

#endif
