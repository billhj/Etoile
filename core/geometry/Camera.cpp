/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Camera.cpp
* @brief 
* @date 1/2/2011
*/

#include "Camera.h"
/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

namespace Etoile
{
	
		Camera::Camera(const Vec3f& position, const Quaternionf& orientation): SceneNode(position, orientation)
		{
			reset();
		}

		Camera::Camera(const Vec3f& direction, const Vec3f& up, const Vec3f& position): SceneNode()
		{
			reset();
			this->setPosition(position);
			this->setupCameraOrientation(direction, up);
		}

		Camera::Camera(): SceneNode()
		{
			reset();
		}

		void Camera::reset()
		{
			_type = PERSPECTIVE;
			_fieldOfView = M_PI / 4.0;
			_nearPlane = 0.1f;
			_farPlane = 10000.0f;
			_screenWidth = 800;
			_screenHeight = 600;
		}

		CameraType Camera::type(){return _type;}
		void Camera::setType(CameraType type){_type = type;}
		float Camera::getAspectRatio() const{return static_cast<float>(_screenWidth) / static_cast<float>(_screenHeight);}
		void Camera::setZNearPlane(float zNear){_nearPlane = zNear;}
		float Camera::getZNearPlane() const{return _nearPlane;}
		void Camera::setZFarPlane(float zFar){_farPlane = zFar;}
		float Camera::getZFarPlane() const{return _farPlane;}
		void Camera::setFieldOfView(float fov){_fieldOfView = fov;}
		float Camera::getFieldOfView() const{return _fieldOfView;}

		int Camera::getWidth() const{return _screenWidth;}
		void Camera::setWidth(int screenWidth){_screenWidth = screenWidth;}
		int Camera::getHeight() const{return _screenHeight;}
		void Camera::setHeight(int _screenHeight){_screenHeight = _screenHeight;}

		void Camera::setTarget(const Vec3f& target)
		{
			setupCameraOrientation(target - getPosition(), getUpVector());
		}
		void Camera::setUpVector(const Vec3f& up)
		{
			setupCameraOrientation(getViewDirection(), up);
		}
		Vec3f Camera::getUpVector() const{return this->getOrientation() * Vec3f(0,1,0);}
		void Camera::setViewDirection(const Vec3f& direction)
		{
			setupCameraOrientation(direction, getUpVector());
		}
		Vec3f Camera::getViewDirection() const{return this->getOrientation() * Vec3f(0,0,-1);}

		void Camera::setPerspective(int width, int height, float near, float far, float fieldOfView)
		{
			this->setWidth(width);
			this->setHeight(height);
			this->setZNearPlane(near);
			this->setZFarPlane(far);
			this->setFieldOfView(fieldOfView);
			this->setType(CameraType::PERSPECTIVE);
			computeProjectionMatrix();
		}
		void Camera::setOrthogonal(int width, int height, float near, float far)
		{
			this->setWidth(width);
			this->setHeight(height);
			this->setZNearPlane(near);
			this->setZFarPlane(far);
			this->setType(CameraType::ORTHOGRAPHIC);
			computeProjectionMatrix();
		}

		Matrix4f Camera::getModelViewMatrix() const{return _modelviewMatrix;}
		Matrix4f Camera::getProjectionMatrix() const{return _projectionMatrix;}
		float* Camera::getGLModelViewMatrix() const{return _modelviewMatrix.getGLMatrix();}
		float* Camera::getGLProjectionMatrix() const{return _projectionMatrix.getGLMatrix();}

		void Camera::setupCameraOrientation(const Vec3f& direction, const Vec3f& upVector)
		{
			if (direction.length() < 1E-10)
				return;

			Vec3f xAxis = direction.cross3(upVector);
			if (xAxis.length() < 1E-10)
			{
				// target is aligned with upVector, this means a rotation around X axis
				// X axis is then unchanged, let's keep it !
				xAxis = Vec3f(1.0, 0.0, 0.0); //computeGlobalPosition(Vec3f(1.0, 0.0, 0.0));
			}

			Quaternionf q;
			q.setFromRotatedBasis(xAxis, xAxis.cross3(direction), -direction);
			setOrientation(q);
		}

		void Camera::computeTransformationMatrix()
		{
			computeProjectionMatrix();
			computeModelViewMatrix();
		}

		void Camera::computeProjectionMatrix()
		{
			switch (_type)
			{
			case CameraType::PERSPECTIVE:
				{
					const float f = 1.0/tan(_fieldOfView/2.0);
					_projectionMatrix[0][0]  = f/getAspectRatio();
					_projectionMatrix[1][1]  = f;
					_projectionMatrix[2][2] = (_nearPlane + _farPlane) / (_nearPlane - _farPlane);
					_projectionMatrix[3][2] = -1.0;
					_projectionMatrix[2][3] = 2.0 * _nearPlane * _farPlane / (_nearPlane - _farPlane);
					_projectionMatrix[3][3] = 0.0;
					// same as gluPerspective( 180.0*fieldOfView()/M_PI, aspectRatio(), zNear(), zFar() );
					break;
				}
			case CameraType::ORTHOGRAPHIC:
				{
					_projectionMatrix[0][0]  = 1.0/static_cast<float>(_screenWidth);
					_projectionMatrix[1][1]  = 1.0/static_cast<float>(_screenHeight);
					_projectionMatrix[2][2] = -2.0/(_farPlane - _nearPlane);
					_projectionMatrix[3][2] = 0.0;
					_projectionMatrix[2][3] = (_farPlane + _nearPlane)/(_nearPlane - _farPlane);
					_projectionMatrix[3][3] = 1.0;
					// same as glOrtho( -w, w, -h, h, zNear(), zFar() );
					break;
				}
			}
		}

		void Camera::computeModelViewMatrix()
		{
			const Quaternionf q = getOrientation();
			q.getInverseMatrix(_modelviewMatrix);      //inverse matrix for camera that is modelview for obj
			Vec3f t = q.inverseRotate(getPosition());  // distance in project space  
			_modelviewMatrix[0][3] = -t[0];
			_modelviewMatrix[1][3] = -t[1];
			_modelviewMatrix[2][3] = -t[2];
			_modelviewMatrix[3][3] = 1.0;
		}

		FrontViewCamera::FrontViewCamera(const Vec3f& position): Camera(Vec3f(0,0,-1),Vec3f(0,1,0),position)
		{
		
		}

		LeftViewCamera::LeftViewCamera(const Vec3f& position): Camera(Vec3f(1,0,0),Vec3f(0,1,0),position)
		{
		
		}

		TopViewCamera::TopViewCamera(const Vec3f& position): Camera(Vec3f(0,-1,0),Vec3f(0,0,-1),position)
		{
		
		}
}