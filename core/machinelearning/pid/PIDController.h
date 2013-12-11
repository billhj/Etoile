/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file PIDController.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "PIDState.h"

namespace Etoile
{
	template<class T>
	class PIDController
	{
	protected:
		
	public:
		PIDController()
		{
		}

		T updatePID(PIDState<T>* state, T& currentObservation, T& desire)
		{
			T error = desire - currentObservation;
			T pTerm = updateProportional(state->getProportionalGain(), error);
			T iTerm = updateIntegral(state->getIntegralGain(), error, state->getMinIntegralErrorState(), state->getMaxIntegralErrorState());
			T dTerm = updateDerivative(state->getDerivativeGain(), state->getState(), currentObservation);
			T final = pTerm + iTerm - dTerm;  // dTerm need to close to Zero, so if +, then next time need to -
			state->updateState(currentObservation);
			return final;
		}

		T updatePD(PIDState<T>* state, T& currentObservation, T& desire)
		{
			T error = desire - currentObservation;
			T pTerm = updateProportional(state->getProportionalGain(), error);
			T dTerm = updateDerivative(state->getDerivativeGain(), state->getState(), currentObservation);
			T final = pTerm - dTerm;  // dTerm need to close to Zero, so if +, then next time need to -
			state->updateState(currentObservation);
			return final;
		}
		
		T updateProportional(double pGain, T& error)
		{
			T pTerm = error * pGain;
			return pTerm;
		}

		T updateIntegral(double iGain, T& iStateError, T& error, T iStateMin, T iStateMax)
		{
			iStateError += error;
			if(iStateError > iStateMax)
			{
				iStateError = iStateMax;
			}
			else if(iStateError < iStateMin)
			{
				iStateError = iStateMin;
			}
			T iTerm = iGain * iStateError;
			return iTerm;
		}

		T updateDerivative(double dGain, T& lastObservation, T& currentObservation)
		{
			T dTerm = dGain * (currentObservation - lastObservation);
			return dTerm;
		}

	};

	typedef PIDController<Vec3d> PIDControllerVec3d;
}