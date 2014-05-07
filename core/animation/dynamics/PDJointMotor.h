/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file PDJointMotor.h
* @brief 
* @date 1/2/2011
*/
#pragma once
#ifndef PDJOINT_MOTOR_H
#define PDJOINT_MOTOR_H

#include "JointMotor.h"

namespace Etoile
{
	class PDJointMotor : public JointMotor
	{
	public:
		PDJointMotor() : JointMotor()
		{
			_kp = 1000;
			_kd = 10;
		}

		/**
		 *	 the input is the desire, current, output the torque
		 */
		virtual void apply(float current, float speed, float accelaration, float& torque) override
		{
			torque = (_desire - current) * _kp - speed * _kd;
		}

		void setKp(float kp){ _kp = kp;}
		void setKd(float kd){ _kd = kd;}
		float Kp(){return _kp;}
		float Kd(){return _kd;}

		void computeParameters(float desire, float extorque)
		{
			_desire = desire + extorque / _kp;
		}
	private:
		float _kp;
		float _kd;
		float _desire;
		//desire needs to be the equilibrum
	};
}

#endif //PDJOINT_MOTOR_H