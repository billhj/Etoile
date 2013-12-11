/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Camera.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "SceneObject.h"

namespace Etoile
{
	enum CameraType {PERSPECTIVE, ORTHOGRAPHIC};

	class Camera : public SceneObject
	{
		int _screenWidth, _screenHeight;  // size of the window, in pixels
		float _fieldOfView; // in radians
		float _nearPlane;
		float _farPlane;

		Matrix4f _modelviewMatrix;
		Matrix4f _projectionMatrix;

		CameraType _type;
	public:
		Camera(const Vec3f& position, const Quaternionf& orientation);
		Camera(const Vec3f& direction, const Vec3f& up, const Vec3f& position);
		Camera();
		void reset();
		CameraType type();
		void setType(CameraType type);
		float getAspectRatio() const;
		void setZNearPlane(float zNear);
		float getZNearPlane() const;
		void setZFarPlane(float zFar);
		float getZFarPlane() const;
		void setFieldOfView(float fov);
		float getFieldOfView() const;

		int getWidth() const;
		void setWidth(int screenWidth);
		int getHeight() const;
		void setHeight(int _screenHeight);

		void setTarget(const Vec3f& target);
		void setUpVector(const Vec3f& up);
		Vec3f getUpVector() const;
		void setViewDirection(const Vec3f& direction);
		Vec3f getViewDirection() const;

		void setPerspective(int width = 800, int height = 600, float near = 0.1f, float far = 10000.0f, float fieldOfView =  M_PI / 4.0);
		void setOrthogonal(int width = 800, int height = 600, float near = 0.1f, float far = 10000.0f);

		Matrix4f getModelViewMatrix() const;
		Matrix4f getProjectionMatrix() const;
		float* getGLModelViewMatrix() const;
		float* getGLProjectionMatrix() const;

		void setupCameraOrientation(const Vec3f& direction, const Vec3f& upVector);
		void computeTransformationMatrix();
		void computeProjectionMatrix();
		void computeModelViewMatrix();
	};


	class FrontViewCamera: public Camera
	{
	public:
		FrontViewCamera(const Vec3f& position);
	};

	class LeftViewCamera: public Camera
	{
	public:
		LeftViewCamera(const Vec3f& position);
	};

	class TopViewCamera: public Camera
	{
	public:
		TopViewCamera(const Vec3f& position);
	};
}