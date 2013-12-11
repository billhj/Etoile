/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Ray.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "math/Vectors.h"

namespace Etoile
{
	/*! Ray structure. Contains all information about a ray including
	*  precomputed reciprocal direction. */
	template <class T>
	class Ray
	{
		/*! Default construction does nothing. */
		inline Ray() {}

		/*! Constructs a ray from origin, direction, and ray segment. Near
		*  has to be smaller than far. */
		inline Ray(const Vec3<T>& org, const Vec3<T>& dir, T near = 0.01f, T far = 10000.0f, T time = 0.0f)
			: _org(org), _dir(dir), _rdir(dir), _near(near), _far(far), _time(time) {}

		void setOrigine(const Vec3<T>& origine){ _org = origine;}
		const Vec3<T> getOrigine() const{ return _org;}
		void setDirection(const Vec3<T>& direction){ _dir = direction;}
		const Vec3<T> getDirection() const{ return _dir;}
		void setReciprocalRayDirection(const Vec3<T>& direction){ _rdir = direction;}
		const Vec3<T> getReciprocalRayDirection() const{ return _rdir;}
		void setNearPlane(T near){ _near = near;}
		const T getNearPlane() const{ return _near;}
		void setFarPlane(T far){ _far = far;}
		const T getFarPlane() const{ return _far;}
		void setMotionBlurTime(T time){ _time = time;}
		const T getMotionBlurTime() const{ return _time;}

		/*! Outputs ray to stream. */
		static inline std::ostream& operator<<(std::ostream& cout, const Ray& ray) {
			return cout << "{ org = " << ray.getOrigine() << ", dir = " << ray.getDirection() << ", rdir = " << ray.getReciprocalRayDirection() 
				<< ", near = " << ray.getNearPlane() << ", far = " << ray.getFarPlane() << ", time = " << ray.time << " }";
		}
	private:
		Vec3<T> _org;     //!< Ray origin
		Vec3<T> _dir;     //!< Ray direction
		Vec3<T> _rdir;    //!< Reciprocal ray direction
		T _near;    //!< Start of ray segment
		T _far;     //!< End of ray segment
		T _time;    //!< Time of this ray for motion blur.
	};

	typedef Ray<float> Rayf;
	typedef Ray<double> Rayd;

}

