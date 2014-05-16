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
#include <cmath>
#include <iostream>

namespace Etoile
{
	class AntagonisticJointMotor : public JointMotor
	{
	public:
		AntagonisticJointMotor() : JointMotor()
		{
			_tension = 50;
			_low = -3.14;
			_high = 0;
		}

		/**
		 *	 the input is the desire, current, output the torque
		 */
		virtual void apply(float current, float speed, float accelaration, float& torque) override
		{
			std::cout<<"target: "<< _key_end <<std::endl;
			float floor = _key_end - _key_start;
			//floor = (abs(floor) < 0.001) ? 0.0001 : floor;
			float ratio = (current - _key_start) / floor;
			float kl = ratio * (_kl_end - _kl_start) + _kl_start;
			float kh = ratio * (_kh_end - _kh_start) + _kh_start;
			//std::cout<<"kl: "<< kl<<" kh: "<< kh <<std::endl;
			torque = (_low - current) * kl +  (_high - current) * kh;
			//torque *= 1.2;
		}

		void setTensionConstant(float tc)
		{
			float ratio = tc / _tension;
			_kl_start = _kl_start * ratio;
			_kl_end = _kl_end * ratio;
			_kh_start = _kh_start * ratio;
			_kh_end = _kh_end * ratio;
			_tension = tc;
		}
		float tensionConstant(){return _tension;}


		void computeStartParameters(float key, float torque)
		{
			_key_start = key;
			float floor = _low - _high;
			//floor = floor == 0 ? -0.0001 : floor;
			_kl_start = (_tension * (key - _high) + torque) / floor;
			_kh_start = _tension - _kl_start;
			{
				float torque = (_low - key) * _kl_start +  (_high - key) * _kh_start;
				std::cout<<"generated torque: "<< torque <<std::endl;
			}
		}

		void computeEndParameters(float key, float torque)
		{
			_key_end = key;
			float floor = _low - _high;
			//floor = floor == 0 ? -0.0001 : floor;
			_kl_end = (_tension * (key - _high) + torque) / floor;
			_kh_end = _tension - _kl_end;
			{
				float torque = (_low - key) * _kl_end +  (_high - key) * _kh_end;
				std::cout<<"generated torque: "<< torque <<std::endl;
			}
		}

		void setLow(float low){_low = low;}
		void setHigh(float high){_high = high;}
		float low(){return _low;}
		float high(){return _high;}
	private:
		float _tension;

		float _kl_start;
		float _kh_start;
		float _key_start;
		float _kl_end;
		float _kh_end;
		float _key_end;

		float _low;
		float _high;
		//desire needs to be the equilibrum
	};
}

#endif //ANTAGONISTIC_JOINT_MOTOR_H