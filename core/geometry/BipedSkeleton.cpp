/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file BipedSkeleton.cpp
* @brief 
* @date 1/2/2011
*/

#include "BipedSkeleton.h"

namespace Etoile
{
	BipedSkeleton::BipedSkeleton(const std::string& name) : Skeleton(name)
	{
		init();
	}

	void BipedSkeleton::init()
	{
		int root = this->createJoint("root");
		getJoint(root)->addDOF(ROTATION_X, DOFConstraint(-0.1,0.1));
		getJoint(root)->addDOF(ROTATION_Y, DOFConstraint(-0.1,0.1));
		getJoint(root)->addDOF(ROTATION_Z, DOFConstraint(-0.1,0.1));
		getJoint(root)->setLocalPosition(Vec3f(0, 10, 0));

		int hip_r = this->createJoint("hip_r", root);
		getJoint(hip_r)->addDOF(ROTATION_X, DOFConstraint(-1.5,0.5));
		getJoint(hip_r)->addDOF(ROTATION_Y, DOFConstraint(-0.5,0.5));
		getJoint(hip_r)->addDOF(ROTATION_Z, DOFConstraint(-0.01,0.01));
		getJoint(hip_r)->setLocalPosition(Vec3f(-2, 0, 0));

		int hip_l = this->createJoint("hip_l", root);
		getJoint(hip_l)->addDOF(ROTATION_X, DOFConstraint(-1.5,0.5));
		getJoint(hip_l)->addDOF(ROTATION_Y, DOFConstraint(-0.5,0.5));
		getJoint(hip_l)->addDOF(ROTATION_Z, DOFConstraint(-0.01,0.01));
		getJoint(hip_l)->setLocalPosition(Vec3f(2, 0, 0));

		int knee_r = this->createJoint("knee_r", hip_r);
		getJoint(knee_r)->addDOF(ROTATION_X, DOFConstraint(-0, 1.5));
		getJoint(knee_r)->addDOF(ROTATION_Y, DOFConstraint(-0.0,0.0));
		getJoint(knee_r)->addDOF(ROTATION_Z, DOFConstraint(-0.00,0.00));
		getJoint(knee_r)->setLocalPosition(Vec3f(0, -4, 0));

		int knee_l = this->createJoint("knee_l", hip_l);
		getJoint(knee_l)->addDOF(ROTATION_X, DOFConstraint(-0, 1.5));
		getJoint(knee_l)->addDOF(ROTATION_Y, DOFConstraint(-0.0,0.0));
		getJoint(knee_l)->addDOF(ROTATION_Z, DOFConstraint(-0.00,0.00));
		getJoint(knee_l)->setLocalPosition(Vec3f(0, -4, 0));

		int ankle_r = this->createJoint("ankle_r", knee_r);
		getJoint(ankle_r)->addDOF(ROTATION_X, DOFConstraint(-0.0, 0.5));
		getJoint(ankle_r)->addDOF(ROTATION_Y, DOFConstraint(-0.1,0.1));
		getJoint(ankle_r)->addDOF(ROTATION_Z, DOFConstraint(-0.00,0.00));
		getJoint(ankle_r)->setLocalPosition(Vec3f(0, -6, 0));

		int ankle_l = this->createJoint("ankle_l", knee_l);
		getJoint(ankle_l)->addDOF(ROTATION_X, DOFConstraint(-0.0, 0.5));
		getJoint(ankle_l)->addDOF(ROTATION_Y, DOFConstraint(-0.1,0.1));
		getJoint(ankle_l)->addDOF(ROTATION_Z, DOFConstraint(-0.00,0.00));
		getJoint(ankle_l)->setLocalPosition(Vec3f(0, -6, 0));

		int foot_r = this->createJoint("foot_r", ankle_r);
		getJoint(foot_r)->setLocalPosition(Vec3f(0, 0, 1));
		int foot_l = this->createJoint("foot_l", ankle_l);
		getJoint(foot_l)->setLocalPosition(Vec3f(0, 0, 1));

		int spine = this->createJoint("spine", root);
		getJoint(spine)->addDOF(ROTATION_X, DOFConstraint(-0.3, 0.3));
		getJoint(spine)->addDOF(ROTATION_Y, DOFConstraint(-0.1,0.1));
		getJoint(spine)->addDOF(ROTATION_Z, DOFConstraint(-0.1,0.1));
		getJoint(spine)->setLocalPosition(Vec3f(0, 3, -1));

		int chest = this->createJoint("chest", spine);
		getJoint(chest)->addDOF(ROTATION_X, DOFConstraint(-0.3, 0.3));
		getJoint(chest)->addDOF(ROTATION_Y, DOFConstraint(-0.1,0.1));
		getJoint(chest)->addDOF(ROTATION_Z, DOFConstraint(-0.1,0.1));
		getJoint(chest)->setLocalPosition(Vec3f(0, 3, 0.4));

		int neck = this->createJoint("neck", chest);
		getJoint(neck)->addDOF(ROTATION_X, DOFConstraint(-0.3, 0.3));
		getJoint(neck)->addDOF(ROTATION_Y, DOFConstraint(-0.5,0.5));
		getJoint(neck)->addDOF(ROTATION_Z, DOFConstraint(-0.1,0.1));
		getJoint(neck)->setLocalPosition(Vec3f(0, 2, 0));

		int head = this->createJoint("head", neck);
		getJoint(head)->setLocalPosition(Vec3f(0, 2, 0));

		int shoulder_l = this->createJoint("shoulder_l", chest);
		getJoint(shoulder_l)->addDOF(ROTATION_X, DOFConstraint(-2.7, 0.7));
		getJoint(shoulder_l)->addDOF(ROTATION_Y, DOFConstraint(-0.0,0.0));
		getJoint(shoulder_l)->addDOF(ROTATION_Z, DOFConstraint(-0.1,1.6));
		getJoint(shoulder_l)->setLocalPosition(Vec3f(2, 2, 0));
		int shoulder_r = this->createJoint("shoulder_r", chest);
		getJoint(shoulder_r)->addDOF(ROTATION_X, DOFConstraint(-2.7, 0.7));
		getJoint(shoulder_r)->addDOF(ROTATION_Y, DOFConstraint(-0.0,0.0));
		getJoint(shoulder_r)->addDOF(ROTATION_Z, DOFConstraint(-1.6,0.1));
		getJoint(shoulder_r)->setLocalPosition(Vec3f(-2, 2, 0));

		int elbow_r = this->createJoint("elbow_r", shoulder_r);
		getJoint(elbow_r)->addDOF(ROTATION_X, DOFConstraint(-2, 0.0));
		getJoint(elbow_r)->addDOF(ROTATION_Y, DOFConstraint(-0.01,0.01));
		getJoint(elbow_r)->addDOF(ROTATION_Z, DOFConstraint(-0.0,0.0));
		getJoint(elbow_r)->setLocalPosition(Vec3f(0, -3.2, 0));

		int elbow_l = this->createJoint("elbow_l", shoulder_l);
		getJoint(elbow_l)->addDOF(ROTATION_X, DOFConstraint(-2, 0.0));
		getJoint(elbow_l)->addDOF(ROTATION_Y, DOFConstraint(-0.01,0.01));
		getJoint(elbow_l)->addDOF(ROTATION_Z, DOFConstraint(-0.0,0.0));
		getJoint(elbow_l)->setLocalPosition(Vec3f(0, -3.2, 0));

		int wrist_r = this->createJoint("wrist_r", elbow_r);
		getJoint(wrist_r)->addDOF(ROTATION_X, DOFConstraint(0, 0.0));
		getJoint(wrist_r)->addDOF(ROTATION_Y, DOFConstraint(-0.01,0.01));
		getJoint(wrist_r)->addDOF(ROTATION_Z, DOFConstraint(-0.5,0.5));
		getJoint(wrist_r)->setLocalPosition(Vec3f(0, -3.5, 0));
		int wrist_l = this->createJoint("wrist_l", elbow_l);
		getJoint(wrist_l)->addDOF(ROTATION_X, DOFConstraint(0, 0.0));
		getJoint(wrist_l)->addDOF(ROTATION_Y, DOFConstraint(-0.01,0.01));
		getJoint(wrist_l)->addDOF(ROTATION_Z, DOFConstraint(-0.5,0.5));
		getJoint(wrist_l)->setLocalPosition(Vec3f(0, -3.5, 0));


		int hand_r = this->createJoint("hand_r", wrist_r);
		getJoint(hand_r)->setLocalPosition(Vec3f(0, -1.5, 0));
		int hand_l = this->createJoint("hand_l", wrist_l);
		getJoint(hand_l)->setLocalPosition(Vec3f(0, -1.5, 0));

		this->update();
	}
}