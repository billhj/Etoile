/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Quaternion.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <stdio.h>
#include "Vec3.h"
#include "Matrix.h"

#ifndef M_PI
#define M_PI 3.14159265f
#endif

namespace Etoile
{
#ifndef THRESHOLD_ROTATION
#define THRESHOLD_ROTATION 0.01
#endif

	template <class T>
	class Quaternion
	{
	private:
		/*! The internal data representation is private, use operator[] to access values. */
		T q[4];

	public:

		Quaternion()
		{ 
			q[0]=q[1]=q[2]=0.0;  
			q[3]=1.0; 
		}

		Quaternion(const Vec3<T>& axis, T angle)
		{
			setAxisAngle(axis, angle);
		}

		Quaternion(T  q0, T  q1, T  q2, T  q3)
		{ 
			q[0]=q0;    q[1]=q1;    q[2]=q2;    q[3]=q3; 
		}

		/*! Copy constructor. */
		Quaternion(const Quaternion& Q)
		{ 
			for (int i=0; i<4; ++i) 
			{
				q[i] = Q.q[i]; 
			}
		}

		/*! Sets the Quaternion as a rotation of axis \p axis and angle \p angle (in radians).
		\p axis does not need to be normalized. A null \p axis will result in an identity Quaternion. */
		void setAxisAngle(const Vec3<T> & axis, T  angle)
		{
			const T norm = axis.length();
			if (norm < 1E-8)
			{
				// Null rotation
				q[0] = 0.0;      q[1] = 0.0;      q[2] = 0.0;      q[3] = 1.0;
			}
			else
			{
				const T  sin_half_angle = sin(angle / 2.0f);
				q[0] = sin_half_angle * axis[0]/norm;
				q[1] = sin_half_angle * axis[1]/norm;
				q[2] = sin_half_angle * axis[2]/norm;
				q[3] = cos(angle / 2.0f);
			}
		}

		/*! Sets the Quaternion value. See the Quaternion(T , T , T , T ) constructor documentation. */
		void setValue(T  q0, T  q1, T  q2, T  q3)
		{ 
			q[0]=q0;    
			q[1]=q1;    
			q[2]=q2;    
			q[3]=q3; 
		}

		void setValue(const Quaternion& Q)
		{ 
			for (int i=0; i<4; ++i)
			{ 
				q[i] = Q.q[i]; 
			}
		}


		/*
		**
		* operator.
		*/
		friend ostream& operator<<(ostream& o, const Quaternion& Q)
		{
			return o << Q[0] << '\t' << Q[1] << '\t' << Q[2] << '\t' << Q[3];
		}

		T  operator[](int i) const 
		{ 
			return q[i]; 
		}

		T & operator[](int i) 
		{ 
			return q[i]; 
		}

		Quaternion operator *(T  scale) const
		{ 
			return Quaternion(q[0]*scale,q[1]*scale,q[2]*scale,q[3]*scale); 
		}

		Quaternion operator /(T  scale) const
		{ 
			return Quaternion(q[0]/scale,q[1]/scale,q[2]/scale,q[3]/scale); 
		}


		Quaternion &operator =(const Quaternion &q)		
		{ 
			this->q[0] = q[0]; 
			this->q[1] = q[1]; 
			this->q[2] = q[2];  
			this->q[3] = q[3];
			return *this; 
		}

		Quaternion operator +(const Quaternion &q) const	
		{ 
			return Quaternion(this->q[0]+q[0], this->q[1]+ q[1], this->q[2]+q[2],  this->q[3]+q[3]);
		}

		Quaternion operator -(const Quaternion &q) const	
		{ 
			return Quaternion(this->q[0]-q[0], this->q[1]-q[1], this->q[2]-q[2], this->q[3]-q[3]); 
		}

		friend Quaternion operator*(const Quaternion& a, const Quaternion& b)
		{
			return Quaternion(a.q[3]*b.q[0] + b.q[3]*a.q[0] + a.q[1]*b.q[2] - a.q[2]*b.q[1],
				a.q[3]*b.q[1] + b.q[3]*a.q[1] + a.q[2]*b.q[0] - a.q[0]*b.q[2],
				a.q[3]*b.q[2] + b.q[3]*a.q[2] + a.q[0]*b.q[1] - a.q[1]*b.q[0],
				a.q[3]*b.q[3] - b.q[0]*a.q[0] - a.q[1]*b.q[1] - a.q[2]*b.q[2]);
		}

		friend Quaternion operator/(const Quaternion& a, const Quaternion& b)
		{
			Quaternion p(b); 
			p.invert(); 
			return a * p;
		}


		bool operator==(const Quaternion &q)
		{
			if(q[3]==q.q[3] && q[2]==q.q[2] &&q[1]==q.q[1] &&q[0]==q.q[0])
				return true;
			return false;
		}

		bool operator!=(const Quaternion &q)
		{
			if(q[3]==q.q[3] && q[2]==q.q[2] &&q[1]==q.q[1] &&q[0]==q.q[0])
				return false;
			return true;
		}

		Quaternion& operator*=(const Quaternion &q)
		{
			*this = (*this)*q;
			return *this;
		}

		Quaternion& operator/=(const Quaternion &q)
		{
			*this = (*this)*q.inverse();
			return *this;
		}

		friend Vec3<T>  operator*(const Quaternion& q, const Vec3<T> & v)
		{
			return q.rotate(v);
		}

		T x()
		{
			return q[0];
		}

		T y()
		{
			return q[1];
		}

		T z()
		{
			return q[2];
		}

		T w()
		{
			return q[3];
		}

		//Matrix
		void setFromRotationMatrix(const Matrix3<T> m)
		{
			// Compute one plus the trace of the matrix
			const T  onePlusTrace = 1.0 + m[0][0] + m[1][1] + m[2][2];

			if (onePlusTrace > 1E-5)
			{
				// Direct computation
				const T  s = sqrt(onePlusTrace) * 2.0;
				q[0] = (m[2][1] - m[1][2]) / s;
				q[1] = (m[0][2] - m[2][0]) / s;
				q[2] = (m[1][0] - m[0][1]) / s;
				q[3] = 0.25 * s;
			}
			else
			{
				// Computation depends on major diagonal term
				if ((m[0][0] > m[1][1])&(m[0][0] > m[2][2]))
				{ 
					const T  s = sqrt(1.0 + m[0][0] - m[1][1] - m[2][2]) * 2.0; 
					q[0] = 0.25 * s;
					q[1] = (m[0][1] + m[1][0]) / s; 
					q[2] = (m[0][2] + m[2][0]) / s; 
					q[3] = (m[2][1] - m[1][2]) / s;
				}
				else
					if (m[1][1] > m[2][2])
					{ 
						const T  s = sqrt(1.0 + m[1][1] - m[0][0] - m[2][2]) * 2.0; 
						q[0] = (m[0][1] + m[1][0]) / s; 
						q[1] = 0.25 * s;
						q[2] = (m[1][2] + m[2][1]) / s; 
						q[3] = (m[0][2] - m[2][0]) / s;
					}
					else
					{ 
						const T  s = sqrt(1.0 + m[2][2] - m[0][0] - m[1][1]) * 2.0; 
						q[0] = (m[0][2] + m[2][0]) / s; 
						q[1] = (m[1][2] + m[2][1]) / s; 
						q[2] = 0.25 * s;
						q[3] = (m[1][0] - m[0][1]) / s;
					}
			}
			normalize();
		}


		void setFromRotatedBasis(const Vec3<T> & X, const Vec3<T> & Y, const Vec3<T> & Z)
		{
			Matrix3<T>  m;
			T  normX = X.length();
			T  normY = Y.length();
			T  normZ = Z.length();

			for (int i=0; i<3; ++i)
			{
				m[i][0] = X[i] / normX;
				m[i][1] = Y[i] / normY;
				m[i][2] = Z[i] / normZ;
			}

			setFromRotationMatrix(m);
		}


		//@{
		Vec3<T>  axis() const
		{
			Vec3<T>  res = Vec3<T>(q[0], q[1], q[2]);
			const T sinus = res.length();
			if (sinus > 1E-8)
				res /= sinus;
			return (acos(q[3]) <= M_PI/2.0) ? res : -res;
		}

		T  angle() const
		{
			const T angle = 2.0 * acos(q[3]);
			return (angle <= M_PI) ? angle : 2.0*M_PI - angle;
		}

		void getAxisAngle(Vec3<T> & axis, T & angle) const
		{
			angle = 2.0*acos(q[3]);
			axis = Vec3<T>(q[0], q[1], q[2]);
			const T sinus = axis.length();
			if (sinus > 1E-8)
				axis /= sinus;

			if (angle > M_PI)
			{
				angle = 2.0*M_PI - angle;
				axis = -axis;
			}
		}




		Vec3<T> rotate(const Vec3<T> & v) const{
			Vec3<T> vn(v);
			Quaternion vecQuat, resQuat;
			vecQuat[0] = vn.x();
			vecQuat[1] = vn.y();
			vecQuat[2] = vn.z();
			vecQuat[3] = 0.0f;

			resQuat = vecQuat * inverse();
			resQuat = *this * resQuat;
			Vec3<T> re = Vec3<T>(resQuat[0], resQuat[1], resQuat[2]);

			return re;
		}

		Vec3<T> inverseRotate(const Vec3<T> & v) const
		{
			return inverse().rotate(v);
		}


		Quaternion conjugate( void ) const
		{
			return Quaternion(-q[0], -q[1], -q[2], q[3]);;
		}


		void invert() { q[0] = -q[0]; q[1] = -q[1]; q[2] = -q[2]; }

		Quaternion inverse() const 
		{ 
			T  scalar = 1 / ( q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
			return conjugate() * scalar; 
		}


		void negate() { invert(); q[3] = -q[3]; }

		T norm()
		{
			return  (sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]));
		}

		T normalize()
		{
			const T norm = sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]);
			for (int i=0; i<4; ++i)
				q[i] /= norm;
			return norm;
		}

		Quaternion normalized() const
		{
			T Q[4];
			const T norm = sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]);
			for (int i=0; i<4; ++i)
			{
				Q[i] = q[i] / norm;
			}
			return Quaternion(Q[0], Q[1], Q[2], Q[3]);
		}



		/*! @name Associated matrix */
		//@{
		const Matrix4<T> matrix() const
		{
			static Matrix4<T> m;
			getMatrix(m);
			return m;
		}

		void getMatrix(Matrix4<T>&  m) const
		{
			const T  q00 = 2.0 * q[0] * q[0];
			const T  q11 = 2.0 * q[1] * q[1];
			const T  q22 = 2.0 * q[2] * q[2];

			const T  q01 = 2.0 * q[0] * q[1];
			const T  q02 = 2.0 * q[0] * q[2];
			const T  q03 = 2.0 * q[0] * q[3];

			const T  q12 = 2.0 * q[1] * q[2];
			const T  q13 = 2.0 * q[1] * q[3];

			const T  q23 = 2.0 * q[2] * q[3];
			
			m[0][0] = 1.0 - q11 - q22;
			m[0][1] =        q01 - q23;
			m[0][2] =        q02 + q13;

			m[1][0] =        q01 + q23;
			m[1][1] = 1.0 - q22 - q00;
			m[1][2] =        q12 - q03;

			m[2][0] =        q02 - q13;
			m[2][1] =        q12 + q03;
			m[2][2] = 1.0 - q11 - q00;

			m[0][3] = 0.0;
			m[1][3] = 0.0;
			m[2][3] = 0.0;

			m[3][0] = 0.0;
			m[3][1] = 0.0;
			m[3][2] = 0.0;
			m[3][3] = 1.0;
		}

		void getRotationMatrix(Matrix3<T>&  m) const
		{
			const T  q00 = 2.0 * q[0] * q[0];
			const T  q11 = 2.0 * q[1] * q[1];
			const T  q22 = 2.0 * q[2] * q[2];

			const T  q01 = 2.0 * q[0] * q[1];
			const T  q02 = 2.0 * q[0] * q[2];
			const T  q03 = 2.0 * q[0] * q[3];

			const T  q12 = 2.0 * q[1] * q[2];
			const T  q13 = 2.0 * q[1] * q[3];

			const T  q23 = 2.0 * q[2] * q[3];

			m[0][0] = 1.0 - q11 - q22;
			m[0][1] =        q01 - q23;
			m[0][2] =        q02 + q13;

			m[1][0] =        q01 + q23;
			m[1][1] = 1.0 - q22 - q00;
			m[1][2] =        q12 - q03;

			m[2][0] =        q02 - q13;
			m[2][1] =        q12 + q03;
			m[2][2] = 1.0 - q11 - q00;
		}

		const Matrix4<T> inverseMatrix() const
		{
			static Matrix4<T> m;
			getInverseMatrix(m);
			return m;
		}

		void getInverseMatrix(Matrix4<T>& m) const
		{
			inverse().getMatrix(m);
		}

		void getInverseRotationMatrix(Matrix3<T>& m) const
		{
			inverse().getInverseRotationMatrix(M);
		}


		/*! @name linear lerp interpolation */
		static Quaternion lerp(const Quaternion &q1, const Quaternion &q2, T t) 
		{ 
			return (q1*(1-t) + q2*t).normalized(); 
		}

		/*! @name spherical linear Slerp interpolation */
		static Quaternion slerp(const Quaternion& a, const Quaternion& b, T t, bool allowFlip=true)
		{
			T cosAngle = Quaternion::dot(a, b);

			T c1, c2;
			// Linear interpolation for close orientations
			if ((1.0 - fabs(cosAngle)) < THRESHOLD_ROTATION)
			{
				c1 = 1.0f - t;
				c2 = t;
			}
			else
			{
				// Spherical interpolation
				T angle    = acos(fabs(cosAngle));
				T sinAngle = sin(angle);
				c1 = sin(angle * (1.0f - t)) / sinAngle;
				c2 = sin(angle * t) / sinAngle;
			}

			// Use the shortest path
			if (allowFlip && (cosAngle < 0.0))
				c1 = -c1;

			return Quaternion(c1*a[0] + c2*b[0], c1*a[1] + c2*b[1], c1*a[2] + c2*b[2], c1*a[3] + c2*b[3]);
		}

		static Quaternion squad(const Quaternion& a, const Quaternion& tgA, const Quaternion& tgB, const Quaternion& b, T t)
		{
			Quaternion ab = Quaternion::slerp(a, b, t);
			Quaternion tg = Quaternion::slerp(tgA, tgB, t, false);
			return Quaternion::slerp(ab, tg, 2.0*t*(1.0-t), false);
		}


		//! Shoemake-Bezier interpolation using De Castlejau algorithm
		static Quaternion bezier(const Quaternion &q1,const Quaternion &q2,const Quaternion &a,const Quaternion &b, T t)
		{
			// level 1
			Quaternion q11= slerp(q1,a,t),
				q12= slerp(a,b,t),
				q13= slerp(b,q2,t);		
			// level 2 and 3
			return slerp(slerp(q11,q12,t), slerp(q12,q13,t), t);
		}

		//! Given 3 quaternions, qn-1,qn and qn+1, calculate a control point to be used in spline interpolation
		static Quaternion spline(const Quaternion &qnm1,const Quaternion &qn,const Quaternion &qnp1)
		{
			Quaternion qni(qn.inverse());	
			return qn * (( (qni*qnm1).log()+(qni*qnp1).log() )/(-4)).exp();
		}

		static T  dot(const Quaternion& a, const Quaternion& b) 
		{ 
			return a[0]*b[0] + a[1]*b[1] + a[2]*b[2] + a[3]*b[3]; 
		}

		Quaternion log()
		{
			T len = sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2]);

			if (len < 1E-6)
				return Quaternion(q[0], q[1], q[2], 0.0);
			else
			{
				T coef = acos(q[3]) / len;
				return Quaternion(q[0]*coef, q[1]*coef, q[2]*coef, 0.0);
			}
		}

		Quaternion exp()
		{
			T theta = sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2]);

			if (theta < 1E-6)
				return Quaternion(q[0], q[1], q[2], cos(theta));
			else
			{
				T coef = sin(theta) / theta;
				return Quaternion(q[0]*coef, q[1]*coef, q[2]*coef, cos(theta));
			}
		}

		static Quaternion lnDif(const Quaternion& a, const Quaternion& b)
		{
			Quaternion dif = a.inverse()*b;
			dif.normalize();
			return dif.log();
		}

		static Quaternion squadTangent(const Quaternion& before, const Quaternion& center, const Quaternion& after)
		{
			Quaternion l1 = Quaternion::lnDif(center,before);
			Quaternion l2 = Quaternion::lnDif(center,after);
			Quaternion e;
			for (int i=0; i<4; ++i)
			{
				e.q[i] = -0.25 * (l1.q[i] + l2.q[i]);
			}
			e = center*(e.exp());

			// if (Quaternion::dot(e,b) < 0.0)
			// e.negate();

			return e;
		}


		/*! @name Random Quaternion */
		static Quaternion randomQuaternion()
		{
			// The rand() function is not very portable and may not be available on your system.
			// Add the appropriate include or replace by an other random function in case of problem.
			T seed = rand()/(T)RAND_MAX;
			T r1 = sqrt(1.0 - seed);
			T r2 = sqrt(seed);
			T t1 = 2.0 * M_PI * (rand()/(T)RAND_MAX);
			T t2 = 2.0 * M_PI * (rand()/(T)RAND_MAX);
			return Quaternion(sin(t1)*r1, cos(t1)*r1, sin(t2)*r2, cos(t2)*r2);
		}

		// Convert from Z Y X Euler Angles individual RZ - RY - RX,  mulipli RZ* RY * RX   
		void fromEulerXYZ(T roll, T pitch, T yaw)
		{
			T y = yaw  / 2.0;
			T p = pitch  / 2.0;
			T r = roll  / 2.0;

			T sinp = sin(p);
			T siny = sin(y);
			T sinr = sin(r);
			T cosp = cos(p);
			T cosy = cos(y);
			T cosr = cos(r);

			q[0] = sinr * cosp * cosy - cosr * sinp * siny;
			q[1] = cosr * sinp * cosy + sinr * cosp * siny;
			q[2] = cosr * cosp * siny - sinr * sinp * cosy;
			q[3] = cosr * cosp * cosy + sinr * sinp * siny;

			normalize();

		}



		void fromEulerXYZByAngle(T roll, T pitch, T yaw){	
			const T PIOVER180 = 3.14159265 / 180.0;
			T y = yaw * PIOVER180 / 2.0;
			T p = pitch * PIOVER180 / 2.0;
			T r = roll * PIOVER180 / 2.0;

			T sinp = sin(p);
			T siny = sin(y);
			T sinr = sin(r);
			T cosp = cos(p);
			T cosy = cos(y);
			T cosr = cos(r);

			q[0] = sinr * cosp * cosy - cosr * sinp * siny;
			q[1] = cosr * sinp * cosy + sinr * cosp * siny;
			q[2] = cosr * cosp * siny - sinr * sinp * cosy;
			q[3] = cosr * cosp * cosy + sinr * sinp * siny;

			normalize();
		}


		// Convert from Z Y X Euler Angles individual RZ - RY - RX,  mulipli RZ * RY * RX  
		Vec3<T>  getEulerAngleXYZ()const
		{
			//atan2
			T roll = atan2(2 * (q[3]* q[0] + q[1]* q[2]),   1 - 2*( q[0] * q[0] + q[1] * q[1]));
			T v = q[3] * q[1] - q[2] * q[0];
			if(v > 0.5) v = 0.5;
			if(v < -0.5) v = -0.5;
			T pitch = asin(2* ( v ));
			T yaw = atan2( 2 * ( q[3] * q[2] +  q[0] * q[1]), 1 - 2*( q[1] * q[1] + q[2] * q[2]));

			if(v == 0.5)
			{
				roll = 2 * atan2(q[0], q[3]);
				pitch = M_PI * 0.5;
				yaw = 0;
			}
			else if(v == - 0.5)
			{
				roll = -2 * atan2(q[0], q[3]);
				pitch = -M_PI * 0.5;
				yaw = 0;
			}
			return Vec3<T>( roll, pitch, yaw);
		}

		Vec3<T> getEulerAngleXYZBYAngle() const
		{
			const T PIOVER180 =  180.0 /3.14159265;
			Vec3<T> xyz = getEulerAngleXYZ();
			return Vec3<T>( xyz.x() * PIOVER180, xyz.y() * PIOVER180, xyz.z() * PIOVER180);
		}

		void fromEulerZYZ(T a, T b, T c)
		{
			q[3] = -cos( (a - c) * 0.5) * sin(b * 0.5);
			q[0] = -sin( (a - c) * 0.5) * sin(b * 0.5);
			q[1] = -sin( (a + c) * 0.5) * cos(b * 0.5);
			q[2] =  sin( (a + c) * 0.5) * cos(b * 0.5);
		}

		Vec3<T>  getEulerAngleZYZ()const
		{
			T a = atan2( (q[3] *q[1] + q[0]*q[2]), (q[0]*q[1] - q[3]*q[2]));
			T b = acos(-q[3] * q[3] - q[0] *q[0] + q[1] * q[1] + q[2] *q[2]);
			T c = - atan2( (q[3] *q[1] - q[0]*q[2]), (q[0]*q[1] + q[3]*q[2]));
			return Vec3<T>(a,b,c);
		}

		static Quaternion fromRotatedTriangle(const Vec3<T>& a_original_v, const Vec3<T>& a_v, const Vec3<T>& b_original_v, const Vec3<T>& b_v)
		{
			Vec3<T> a_original = a_original_v.normalized();
			Vec3<T> a = a_v.normalized();
			Vec3<T> b_original = b_original_v.normalized();
			Vec3<T> b = b_v.normalized();
			Vec3<T> n_original = a_original.cross3(b_original);
			Vec3<T> n = a.cross3(b);
			n_original.normalize();
			n.normalize();

			Quaternion q1;
			Vec3<T> axis_a =  a_original.cross3(a);
			//Vec3<T> axis_a =  a.cross3(a_original);
			axis_a.normalize();
			T cosTheta_a = a_original.dot3(a);
			if(axis_a.length() < 0.01)
			{
				if((a_original - a).length() > THRESHOLD_ROTATION)
				{
					q1.setAxisAngle(n_original, 3.1415926);
				}
			}else
			{
				q1.setAxisAngle(axis_a, acos(cosTheta_a));
			}

			//std::cout<<"a1" <<a_original << a << q1 * a_original<<std::endl;
			Vec3<T> b_2 = q1 * b_original;
			b_2.normalize();
			Quaternion q2;
			
			Vec3<T> n_b2 = a.cross3(b_2);
			Vec3<T> n_b = a.cross3(b);
			Vec3<T> axis_n = n_b2.cross3(n_b);
			T cosTheta_n = n_b2.dot3(n_b);
			if(axis_n.length() < 0.01)
			{
				if((n_b2 - n_b).length() > THRESHOLD_ROTATION)
				{
					q2.setAxisAngle(a, 3.1415926);
				}
			}else
			{
				q2.setAxisAngle(axis_n, acos(cosTheta_n));
			}

			Quaternion final = q2 * q1;
			final.normalize();
			//std::cout<<"a " << a << final * a_original<<std::endl;
			//std::cout<<"b " << b << final * b_original<<std::endl;
			return final;
		}
	};

	typedef Quaternion<float> Quaternionf;
	typedef Quaternion<double> Quaterniond;
}