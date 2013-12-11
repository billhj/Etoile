/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file HMMModel.cpp
* @brief 
* @date 1/2/2011
*/

#include "HMMModel.h"

namespace Etoile
{
	HMMModel::HMMModel(void)
	{
	}


	HMMModel::~HMMModel(void)
	{
	}

	int HMMModel::addState(HMMState* state)
	{
		std::map<std::string, int>::iterator itor = _stateIdx.find(state->getId());
		if(itor != _stateIdx.end())
		{
			_states[itor->second] = state;
		}
		_states.push_back(state);
		int idx = _states.size() - 1;
		_stateIdx[state->getId()] = idx;
		return idx;
	}

	int HMMModel::addObservation(HMMObservation* observation)
	{
		std::map<std::string, int>::iterator itor = _observationIdx.find(observation->getId());
		if(itor != _observationIdx.end())
		{
			_observations[itor->second] = observation;
		}
		_observations.push_back(observation);
		int idx = _observations.size() - 1;
		_observationIdx[observation->getId()] = idx;
		return idx;
	}

	void HMMModel::createStates(int number)
	{
		for(int i = 0; i < number; ++i)
		{
			HMMState* state = new HMMState();
			addState(state);
		}
	}
	void HMMModel::createObservations(int number)
	{
		for(int i = 0; i < number; ++i)
		{
			HMMObservation* observation = new HMMObservation();
			addObservation(observation);
		}
	}

	void HMMModel::initStateTransitionMatrix()
	{
		int size = _states.size();
		_stateTransitionMatrix.initMatrix(size, size);
	}

	void HMMModel::initStateObservationMatrix()
	{
		_stateObservationMatrix.initMatrix(_states.size(), _observations.size());
	}

	void HMMModel::fillStateObservationMatrix(int state, int observation, HMMStateObservation& value)
	{
		_stateObservationMatrix.setStateObservationProbability(state, observation, value);
	}

	void HMMModel::fillStateObservationMatrix(const std::string& state, const std::string& observation, HMMStateObservation& value)
	{
		std::map<std::string, int>::iterator itor = _stateIdx.find(state);
		std::map<std::string, int>::iterator itor2 = _observationIdx.find(observation);
		if(itor != _stateIdx.end() && itor2 != _observationIdx.end())
		{
			_stateObservationMatrix.setStateObservationProbability(itor->second, itor2->second, value);

		}
	}

	void HMMModel::fillStateTransitionMatrix(int startState, int endState, HMMStateTransition& value)
	{
		_stateTransitionMatrix.setTransitionProbability(startState, endState, value);
	}

	void HMMModel::fillStateTransitionMatrix(const std::string& startState, const std::string& endState, HMMStateTransition& value)
	{
		std::map<std::string, int>::iterator itor = _stateIdx.find(startState);
		std::map<std::string, int>::iterator itor2 = _stateIdx.find(endState);
		if(itor != _stateIdx.end() && itor2 != _stateIdx.end())
		{
			_stateTransitionMatrix.setTransitionProbability(itor->second, itor2->second, value);

		}
	}
}