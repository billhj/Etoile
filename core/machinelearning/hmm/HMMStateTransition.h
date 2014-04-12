/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file HMMStateTransition.h
* @brief 
* @date 1/2/2011
*/

#pragma once

namespace Etoile
{
	/*
	*
	*	the probability between state to state by transition
	**/
	class HMMStateTransition
	{
	public:
		HMMStateTransition(void);
		~HMMStateTransition(void);
		void setProbability(double value){_value = value;}
		double getProbability(){return _value;}
	private:
		double _value;
	};

}