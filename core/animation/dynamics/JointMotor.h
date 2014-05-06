/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file JointMotor.h
* @brief 
* @date 1/2/2011
*/
#pragma once
#ifndef JOINT_MOTOR_H
#define JOINT_MOTOR_H

namespace Etoile
{
	class JointMotor
	{
	public:
		JointMotor()
		{
		
		}

		virtual void apply(float desire, float current, float speed, float accelaration, float& torque) = 0;

	private:
	};
}

#endif //JOINT_MOTOR_H