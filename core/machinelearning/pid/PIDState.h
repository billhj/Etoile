/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file PIDState.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "../../math/MathHead.h"

namespace Etoile
{
	template<class T>
	class PIDState
	{
	protected:
		double _iGain,     // integral gain
			_pGain,     // proportional gain
			_dGain;     // derivative gain
		T _state;
		T _iStateError;  // Integrator Error state
		T _iMin, _iMax;
	public:
		PIDState()
		{
			_pGain = 0.5;
			_iGain = 0.5;
			_dGain = 0.5;
		}

		T getMinIntegralErrorState()
		{
			return _iMin;
		}

		T getMaxIntegralErrorState()
		{
			return _iMax;
		}

		void setMinIntegralErrorState(T& min)
		{
			_iMin = min;
		}

		void setMaxIntegralErrorState(T& max)
		{
			_iMax = max;
		}

		void setProportionalGain(double pGain)
		{
			_pGain = pGain;
		}
		double getProportionalGain(){return _pGain;}

		void setIntegralGain(double iGain)
		{
			_iGain = iGain;
		}
		double getIntegralGain(){return _iGain;}

		void setDerivativeGain(double dGain)
		{
			_dGain = dGain;
		}
		double getDerivativeGain(){return _dGain;}

		T& getIntegralErrorState()
		{
			return _iStateError;
		}

		void updateIntegralErrorState(T& currentError)
		{
			_iStateError = _iStateError + currentError;
		}

		T& getState()
		{
			return _state;
		}

		void updateState(T& state)
		{
			_state = state;
		}
	};

	typedef PIDState<Vec3d> PIDStateVec3d;
}