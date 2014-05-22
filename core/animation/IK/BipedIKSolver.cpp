/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file BipedIKSolver.cpp
* @brief 
* @date 1/2/2011
*/
#include "BipedIKSolver.h"

namespace Etoile
{
	
	BipedIKSolver::BipedIKSolver(BipedSkeleton* skel) : _pSkeleton(skel)
	{
		
	}
	
	void BipedIKSolver::solve()
	{
	
	}

	void BipedIKSolver::addTarget(const std::string& jointname, Vec3f target)
	{
		_target[jointname] = target;
	}

	void BipedIKSolver::solveLeftArm(Vec3f baseOffset, Vec3f armOffset)
	{
		Joint* spine = _pSkeleton->getJoint("spine");
		Joint* chest = _pSkeleton->getJoint("chest");
		Joint* shoulder_l = _pSkeleton->getJoint("shoulder_l");
		Joint* elbow_l = _pSkeleton->getJoint("elbow_l");
		Joint* hand_l = _pSkeleton->getJoint("hand_l");

		Vec3f offset = armOffset - baseOffset;
		Vec3f currentEnd = hand_l->getWorldPosition();
		Vec3f target = currentEnd + offset;
		Vec3f distance = offset;
		int loop = 0;

		MatrixMNf jacobian(3, 3);
		std::vector<Vec3f> axis(3);
		while(loop < _maxLoop && distance.length() > _thresholdDistance)
		{
			//spine
			{
				Vec3f currentPos = spine->getWorldPosition();
				Vec3f vend = currentEnd - currentPos;
				vend.normalize();
				Vec3f vgoal = target - currentPos;
				vgoal.normalize();
				axis[0] = vend.cross3(vgoal);
                axis[0].normalize();
				Vec3f entry = axis[0].cross3(vend);
				entry.normalize();
				axis[0] = spine->getWorldRotation().inverse() * axis[0];
				axis[0].normalize();
				VecNf v(3, &entry[0]);
				jacobian.setColumn(0, v);
			}

			//shoulder
			{
				Vec3f currentPos = shoulder_l->getWorldPosition();
				Vec3f vend = currentEnd - currentPos;
				vend.normalize();
				Vec3f vgoal = target - currentPos;
				vgoal.normalize();
				axis[1] = vend.cross3(vgoal);
                axis[1].normalize();
				Vec3f entry = axis[1].cross3(vend);
				entry.normalize();
				axis[1] = shoulder_l->getWorldRotation().inverse() * axis[1];
				axis[1].normalize();
				VecNf v(3, &entry[1]);
				jacobian.setColumn(1, v);
			}

			//elbow
			{
				Vec3f currentPos = elbow_l->getWorldPosition();
				Vec3f vend = currentEnd - currentPos;
				vend.normalize();
				Vec3f vgoal = target - currentPos;
				vgoal.normalize();
				axis[2] = vend.cross3(vgoal);
                axis[2].normalize();
				Vec3f entry = axis[2].cross3(vend);
				entry.normalize();
				axis[2] = elbow_l->getWorldRotation().inverse() * axis[2];
				axis[2].normalize();
				VecNf v(3, &entry[2]);
				jacobian.setColumn(2, v);
			}

			//compute the transpose
			MatrixMNf transpose = jacobian.transpose();

			VecNf force(3);
			force[0] = distance.x() * _distanceStep;
			force[1] = distance.y() * _distanceStep;
			force[2] = distance.z() * _distanceStep;
			
			//computing q'
			VecNf q = transpose * force;

			{
                double r = q[0];
                Quaternionf rotation;
                rotation.setAxisAngle(axis[0], (float) r * _rotationStep);
                rotation.normalize();
                spine->rotate(rotation);
                spine->update();
				applyConstraint(spine, -1.14f, 1.14f,  -1.14f, 1.14f, -1.14f, 1.14f);
				spine->update();
            }
			{
				chest->update();
			}
			{
                double r = q[1];
                Quaternionf rotation;
                rotation.setAxisAngle(axis[1], (float) r * _rotationStep);
                rotation.normalize();
                shoulder_l->rotate(rotation);
                shoulder_l->update();
            }

            {
                double r = q[2];
                Quaternionf rotation;;
                rotation.setAxisAngle(axis[2], (float) r * _rotationStep);
                rotation.normalize();
                elbow_l->rotate(rotation);
                elbow_l->update();
                applyConstraint(elbow_l, -3.14f, 0.f,0.00f,0.00f,0.00f,0.00f);
                elbow_l->update();
            }
			hand_l->update();
			//hand_l->setLocalRotation(ankle_l->getWorldRotation().inverse());
			//hand_l->update();
		}
	}

	void BipedIKSolver::solveRightArm(Vec3f baseOffset, Vec3f armOffset)
	{
		Joint* spine = _pSkeleton->getJoint("spine");
		Joint* chest = _pSkeleton->getJoint("chest");
		Joint* shoulder_r = _pSkeleton->getJoint("shoulder_r");
		Joint* elbow_r = _pSkeleton->getJoint("elbow_r");
		Joint* hand_r = _pSkeleton->getJoint("hand_r");

		Vec3f offset = armOffset - baseOffset;
		Vec3f currentEnd = hand_r->getWorldPosition();
		Vec3f target = currentEnd + offset;
		Vec3f distance = offset;
		int loop = 0;

		MatrixMNf jacobian(3, 3);
		std::vector<Vec3f> axis(3);
		while(loop < _maxLoop && distance.length() > _thresholdDistance)
		{
			//spine
			{
				Vec3f currentPos = spine->getWorldPosition();
				Vec3f vend = currentEnd - currentPos;
				vend.normalize();
				Vec3f vgoal = target - currentPos;
				vgoal.normalize();
				axis[0] = vend.cross3(vgoal);
                axis[0].normalize();
				Vec3f entry = axis[0].cross3(vend);
				entry.normalize();
				axis[0] = spine->getWorldRotation().inverse() * axis[0];
				axis[0].normalize();
				VecNf v(3, &entry[0]);
				jacobian.setColumn(0, v);
			}

			//shoulder
			{
				Vec3f currentPos = shoulder_r->getWorldPosition();
				Vec3f vend = currentEnd - currentPos;
				vend.normalize();
				Vec3f vgoal = target - currentPos;
				vgoal.normalize();
				axis[1] = vend.cross3(vgoal);
                axis[1].normalize();
				Vec3f entry = axis[1].cross3(vend);
				entry.normalize();
				axis[1] = shoulder_r->getWorldRotation().inverse() * axis[1];
				axis[1].normalize();
				VecNf v(3, &entry[1]);
				jacobian.setColumn(1, v);
			}

			//elbow
			{
				Vec3f currentPos = elbow_r->getWorldPosition();
				Vec3f vend = currentEnd - currentPos;
				vend.normalize();
				Vec3f vgoal = target - currentPos;
				vgoal.normalize();
				axis[2] = vend.cross3(vgoal);
                axis[2].normalize();
				Vec3f entry = axis[2].cross3(vend);
				entry.normalize();
				axis[2] = elbow_r->getWorldRotation().inverse() * axis[2];
				axis[2].normalize();
				VecNf v(3, &entry[2]);
				jacobian.setColumn(2, v);
			}

			//compute the transpose
			MatrixMNf transpose = jacobian.transpose();

			VecNf force(3);
			force[0] = distance.x() * _distanceStep;
			force[1] = distance.y() * _distanceStep;
			force[2] = distance.z() * _distanceStep;
			
			//computing q'
			VecNf q = transpose * force;

			{
                double r = q[0];
                Quaternionf rotation;
                rotation.setAxisAngle(axis[0], (float) r * _rotationStep);
                rotation.normalize();
                spine->rotate(rotation);
                spine->update();
				applyConstraint(spine, -1.14f, 1.14f,  -1.14f, 1.14f, -1.14f, 1.14f);
				spine->update();
            }
			{
				chest->update();
			}
			{
                double r = q[1];
                Quaternionf rotation;
                rotation.setAxisAngle(axis[1], (float) r * _rotationStep);
                rotation.normalize();
                shoulder_r->rotate(rotation);
                shoulder_r->update();
            }

            {
                double r = q[2];
                Quaternionf rotation;;
                rotation.setAxisAngle(axis[2], (float) r * _rotationStep);
                rotation.normalize();
                elbow_r->rotate(rotation);
                elbow_r->update();
                applyConstraint(elbow_r, -3.14f, 0.f,0.00f,0.00f,0.00f,0.00f);
                elbow_r->update();
            }
			hand_r->update();
			//hand_r->setLocalRotation(ankle_l->getWorldRotation().inverse());
			//hand_r->update();
		}
	}

	void BipedIKSolver::solveLeftLeg(Vec3f baseOffset, Vec3f legOffset)
	{
		Joint* hip_l = _pSkeleton->getJoint("hip_l");
		Joint* knee_l = _pSkeleton->getJoint("knee_l");
		Joint* ankle_l = _pSkeleton->getJoint("ankle_l");
		Vec3f offset = legOffset - baseOffset;
		Vec3f currentEnd = ankle_l->getWorldPosition();
		Vec3f target = currentEnd + offset;
		Vec3f distance = offset;
		int loop = 0;

		MatrixMNf jacobian(3, 2);
		std::vector<Vec3f> axis(2);
		while(loop < _maxLoop && distance.length() > _thresholdDistance)
		{
			//hip
			{
				Vec3f currentPos = hip_l->getWorldPosition();
				Vec3f vend = currentEnd - currentPos;
				vend.normalize();
				Vec3f vgoal = target - currentPos;
				vgoal.normalize();
				axis[0] = vend.cross3(vgoal);
                axis[0].normalize();
				Vec3f entry = axis[0].cross3(vend);
				entry.normalize();
				axis[0] = hip_l->getWorldRotation().inverse() * axis[0];
				axis[0].normalize();
				VecNf v(3, &entry[0]);
				jacobian.setColumn(0, v);
			}

			//knee
			{
				Vec3f currentPos = knee_l->getWorldPosition();
				Vec3f vend = currentEnd - currentPos;
				vend.normalize();
				Vec3f vgoal = target - currentPos;
				vgoal.normalize();
				axis[1] = vend.cross3(vgoal);
                axis[1].normalize();
				Vec3f entry = axis[1].cross3(vend);
				entry.normalize();
				axis[1] = knee_l->getWorldRotation().inverse() * axis[1];
				axis[1].normalize();
				VecNf v(3, &entry[1]);
				jacobian.setColumn(1, v);
			}

			//compute the transpose
			MatrixMNf transpose = jacobian.transpose();

			VecNf force(3);
			force[0] = distance.x() * _distanceStep;
			force[1] = distance.y() * _distanceStep;
			force[2] = distance.z() * _distanceStep;
			
			//computing q'
			VecNf q = transpose * force;

			{
                double r = q[0];
                Quaternionf rotation;
                rotation.setAxisAngle(axis[0], (float) r * _rotationStep);
                rotation.normalize();
                hip_l->rotate(rotation);
                hip_l->update();
            }
            {
                double r = q[1];
                Quaternionf rotation;;
                rotation.setAxisAngle(axis[1], (float) r * _rotationStep);
                rotation.normalize();
                knee_l->rotate(rotation);
                knee_l->update();
                applyConstraint(knee_l, 0.f, 3.14f,0.00f,0.00f,0.00f,0.00f);
                knee_l->update();
            }
			ankle_l->update();
			ankle_l->setLocalRotation(ankle_l->getWorldRotation().inverse());
			ankle_l->update();
		}
	}

	void BipedIKSolver::solveRightLeg(Vec3f baseOffset, Vec3f legOffset)
	{
		Joint* hip_r = _pSkeleton->getJoint("hip_r");
		Joint* knee_r = _pSkeleton->getJoint("knee_r");
		Joint* ankle_r = _pSkeleton->getJoint("ankle_r");
		Vec3f offset = legOffset - baseOffset;
		Vec3f currentEnd = ankle_r->getWorldPosition();
		Vec3f target = currentEnd + offset;
		Vec3f distance = offset;
		int loop = 0;

		MatrixMNf jacobian(3, 2);
		std::vector<Vec3f> axis(2);
		while(loop < _maxLoop && distance.length() > _thresholdDistance)
		{
			//hip
			{
				Vec3f currentPos = hip_r->getWorldPosition();
				Vec3f vend = currentEnd - currentPos;
				vend.normalize();
				Vec3f vgoal = target - currentPos;
				vgoal.normalize();
				axis[0] = vend.cross3(vgoal);
                axis[0].normalize();
				Vec3f entry = axis[0].cross3(vend);
				entry.normalize();
				axis[0] = hip_r->getWorldRotation().inverse() * axis[0];
				axis[0].normalize();
				VecNf v(3, &entry[0]);
				jacobian.setColumn(0, v);
			}

			//knee
			{
				Vec3f currentPos = knee_r->getWorldPosition();
				Vec3f vend = currentEnd - currentPos;
				vend.normalize();
				Vec3f vgoal = target - currentPos;
				vgoal.normalize();
				axis[1] = vend.cross3(vgoal);
                axis[1].normalize();
				Vec3f entry = axis[1].cross3(vend);
				entry.normalize();
				axis[1] = knee_r->getWorldRotation().inverse() * axis[1];
				axis[1].normalize();
				VecNf v(3, &entry[1]);
				jacobian.setColumn(1, v);
			}

			//compute the transpose
			MatrixMNf transpose = jacobian.transpose();

			VecNf force(3);
			force[0] = distance.x() * _distanceStep;
			force[1] = distance.y() * _distanceStep;
			force[2] = distance.z() * _distanceStep;
			
			//computing q'
			VecNf q = transpose * force;

			{
                double r = q[0];
                Quaternionf rotation;
                rotation.setAxisAngle(axis[0], (float) r * _rotationStep);
                rotation.normalize();
                hip_r->rotate(rotation);
                hip_r->update();
            }
            {
                double r = q[1];
                Quaternionf rotation;;
                rotation.setAxisAngle(axis[1], (float) r * _rotationStep);
                rotation.normalize();
                knee_r->rotate(rotation);
                knee_r->update();
                applyConstraint(knee_r, 0.f, 3.14f,0.00f,0.00f,0.00f,0.00f);
                knee_r->update();
            }
			ankle_r->update();
			ankle_r->setLocalRotation(ankle_r->getWorldRotation().inverse());
			ankle_r->update();
		}
	}





	void BipedIKSolver::applyConstraint(Joint* j, float xMin, float xMax, float yMin, float yMax, float zMin, float zMax)
	{
        Quaternionf q = j->getLocalRotation();
        Vec3f v = q.getEulerAngleXYZ();
        if(v.x() < xMin)
		{
            v[0] = xMin;
        }
        if(v.y() < yMin)
		{
            v[1] = yMin;
        }
        if(v.z() < zMin)
		{
            v[2] = zMin;
        }
        if(v.x() > xMax)
		{
            v[0] = xMax;
        }
        if(v.y() > yMax)
		{
            v[1] = yMax;
        }
        if(v.z() > zMax)
		{
            v[2] = zMax;
        }
        q.fromEulerXYZ(v.x(), v.y(), v.z());
        j->setLocalRotation(q);
    }
}