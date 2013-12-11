/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file HMMStateObservation.h
* @brief 
* @date 1/2/2011
*/

#pragma once

namespace Etoile
{
	/*
	*
	*	the probability between state to observation
	**/
	class HMMStateObservation
	{
	public:
		HMMStateObservation(void);
		~HMMStateObservation(void);
		void setProbability(double value){_value = value;}
		double getProbability(){
			return _value;
		}
	private:
		double _value;
	};

}