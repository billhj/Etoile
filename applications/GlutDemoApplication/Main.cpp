#include "MyWindow.h"
#include <gl/glut.h>

int main(int argc, char* argv[])
{
 
    MyWindow window;    
    glutInit(&argc, argv);
    window.initWindow(640, 480, "Demo");
    glutMainLoop();

    return 0;
}
