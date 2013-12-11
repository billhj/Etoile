/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file HMMModel.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include <map>
#include "HMMState.h"
#include "HMMObservation.h"
#include "HMMStateObservationMatrix.h"
#include "HMMStateTransitionMatrix.h"

namespace Etoile
{
	class HMMModel
	{
	public:
		HMMModel(void);
		~HMMModel(void);
		int addState(HMMState*);
		int addObservation(HMMObservation*);
		void createStates(int number);
		void createObservations(int number);
		void initStateTransitionMatrix();
		void initStateObservationMatrix();
		void fillStateObservationMatrix(int state, int observation, HMMStateObservation& value);
		void fillStateObservationMatrix(const std::string& state, const std::string& observation, HMMStateObservation& value);
		void fillStateTransitionMatrix(int startState, int endState, HMMStateTransition& value);
		void fillStateTransitionMatrix(const std::string& startState, const std::string& endState, HMMStateTransition& value);
		HMMStateObservationMatrix& getStateObservationMatrix(){return _stateObservationMatrix;}
		HMMStateTransitionMatrix& getStateTransitionMatrix(){return _stateTransitionMatrix;}

		std::vector<HMMState*> getStates(){return _states;}
		std::vector<HMMObservation*> getObservations(){return _observations;}

		void setInitialProbabilityPi(std::vector<double>& pi){_pi = pi;}
		std::vector<double>& getInitialProbabilityPi(){return _pi;}
		void setObservationList(std::vector<int>& observation){ _o = observation; }
		std::vector<int>& getObservationList(){return _o;}

	private:
		std::map<std::string, int> _stateIdx;
		std::vector<HMMState*> _states;

		std::map<std::string, int> _observationIdx;
		std::vector<HMMObservation*> _observations;

		HMMStateObservationMatrix _stateObservationMatrix;
		HMMStateTransitionMatrix _stateTransitionMatrix;

		std::vector<double> _pi;
		std::vector<int> _o;
	};
}

