/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Spring.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "Mass.h"

namespace Etoile
{

class Spring
{
public:
    Spring(float stiffness = 1.0, float damping = 0.3): _stiffness(stiffness), _damping(damping), _actived(true)
	{

	}
	~Spring(){}

    void setDamping(float damping){_damping = damping;}
    void setStiffness(float stiffness){ _stiffness = stiffness;}
	float getStiffness(){return _stiffness;}
    float getDamping(){return _damping;}

    void setActive(bool activity){ _actived = activity;}
    bool isActived(){return _actived;}

    Mass* getMass(unsigned int indexNodeInSpring)
	{
		if(_nodes.size() <= indexNodeInSpring) return NULL;
		return _nodes[indexNodeInSpring];
	}

    int getNodesSize(){ return _nodes.size();}

	void setReleasedLength(float releasedLength){ _releasedLength = releasedLength;}
    float getReleasedLength(){return _releasedLength;}
    virtual void computerInternalForce() = 0;

protected:

    std::vector<Mass*> _nodes;
    std::vector<Vec3f> _nodesInternalForces;

    float _stiffness;
    float _damping;
    union
    {
        float _releasedLength;
        float _releasedAngle;
    };
    float _tolerance;

    float _currentInternalForce;
    bool _actived;

};

typedef std::vector<Spring*> Springs;

}
