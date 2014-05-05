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
		
		}

		/**
		 *	 the input is the desire, current, output the torque
		 */
		void apply(float desire, float current, float speed, float accelaration, float& torque)
		{
			torque = (desire - current) * _kp - speed * _kd;
		}

		void setKp(float kp){ _kp = kp;}
		void setKd(float kd){ _kd = kd;}
	private:
		float _kp;
		float _kd;
	};
}

#endif //PDJOINT_MOTOR_H