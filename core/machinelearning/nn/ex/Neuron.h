/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Neuron.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "../../math/Function1D.h"

namespace Etoile
{
	class Neuron
	{
	protected:
		double _input;
		double _output;
	public:
		Neuron()
		{
			_input = 0;
			_output = 0;
		}

		void setInput(double value)
		{
			_input = value;
		}

		/*
		* accumulate the inputs
		**/
		void addInput(double value)
		{
			_input += value;
		}

		double getInput(){return _input;}

		void setOutput(double value)
		{
			_output = value;
		}

		double getOutput()
		{
			return _output;
		}

		virtual void compute() = 0;
	};

	class InputNeuron : public Neuron
	{
	public:
		virtual void compute() override
		{
			_output = _input;
		}
	};

	class HiddenNeuron : public Neuron
	{
	protected:
		Function1D* _pFunction;
	public:
		HiddenNeuron() : Neuron()
		{
			_pFunction = new Sigmoid();
		}

		~HiddenNeuron()
		{
			delete _pFunction;
		}

		virtual void compute() override
		{
			_output = _pFunction->get(_input);
		}

		void setActivationFunction(Function1D* f)
		{
			if(f != NULL)
			{
				delete _pFunction;
				_pFunction = f;
			}
		}
	
		Function1D* getActivationFunction(){return _pFunction;}
	};

	class OutputNeuron : public HiddenNeuron
	{
	};
}