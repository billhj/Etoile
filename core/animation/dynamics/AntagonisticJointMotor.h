/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file AntagonisticJointMotor.h
* @brief 
* @date 1/2/2011
*/
#pragma once
#ifndef ANTAGONISTIC_JOINT_MOTOR_H
#define ANTAGONISTIC_JOINT_MOTOR_H

#include "JointMotor.h"

namespace Etoile
{
	class AntagonisticJointMotor : public JointMotor
	{
	public:
		AntagonisticJointMotor() : JointMotor()
		{
			_kh = 1000;
			_kl = 10;
		}

		/**
		 *	 the input is the desire, current, output the torque
		 */
		virtual void apply(float current, float speed, float accelaration, float& torque) override
		{
			torque = (_low - current) * _kl +  (_high - current) * _kh;
		}

		void setKl(float kl){ _kl = kl;}
		void setKh(float kh){ _kh = kh;}
		void setTensionConstant(float tc)
		{
			_kl = tc * _kl / (_kl + _kh);
			_kh = tc * _kh / (_kl + _kh);
		}
		float tensionConstant(){return _kl+_kh;}
		float Kl(){return _kl;}
		float Kh(){return _kh;}

		void computeParameters(float desire, float extorque)
		{
			_desire = desire;
			float tensionConstant = _kl+_kh;
			_kl = (tensionConstant * (_desire - _high) - extorque) / (_low - _high);
			_kh = tensionConstant - _kl;
		}

	private:
		float _kl;
		float _kh;
		float _low;
		float _high;
		float _desire;
		//desire needs to be the equilibrum
	};
}

#endif //ANTAGONISTIC_JOINT_MOTOR_H