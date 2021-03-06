#ifndef RENDERER_OPENGLCAMERA_H
#define RENDERER_OPENGLCAMERA_H

class OpenGLCamera
{

public:
	OpenGLCamera() : public Camera {}
	virtual ~OpenGLCamera() {};
	virtual void set(const Vector3d& _Eye, const Vector3d& _look, const Vector3d& _up);
	virtual void slide(double _delX, double _delY, double _delZ, bool _bLocal = false);
	virtual void setFrustum(float _vAng, float _asp, float _nearD, float _farD);
	virtual void setOrtho(float _width, float _height, float _nearD, float _farD);


	virtual void roll(float _angle);
	virtual void pitch(float _angle);
	virtual void yaw(float _angle);
	virtual void localRotate(float _angle, AXIS _axis);
	virtual void globalRotate(float _angle, AXIS _axis);

};


#endif // #ifndef RENDERER_OPENGLCAMERA_H
