/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file NeuralNetwork.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <ctime>
#include <vector>
#include <fstream>
#include <sstream>
#include "DataSet.h"

//Constant Defaults!
#define LEARNING_RATE 0.001
#define MOMENTUM 0.9
#define MAX_EPOCHS 1500
#define DESIRED_ACCURACY 90  

/*******************************************************************************************************************
*   We make it object orientated, can set different functions, not only sigmoid
*   original class NEURAL NETWORK CLASS
*	----------------------------------------------------------------------------------------------------------------
*	Classic Back-propagation Neural Network ( makes use of gradient descent )
*	Can toggle between stochastic and batch learning
*	----------------------------------------------------------------------------------------------------------------
*	Author: Bobby Anguelov (banguelov@cs.up.ac.za)
*	Downloaded From: takinginitiative.wordpress.co.za
*******************************************************************************************************************/

namespace Etoile
{
	class neuralNetwork
	{

		//private members
		//----------------------------------------------------------------------------------------------------------------
	private:

		//learning parameters
		double _learningRate;					// adjusts the step size of the weight update	
		double _momentum;						// improves performance of stochastic learning (don't use for batch)

		//number of neurons
		int _nInput, _nHidden, _nOutput;

		//neurons
		double* _inputNeurons;
		double* _hiddenNeurons;
		double* _outputNeurons;

		//weights
		double** _wInputHidden;
		double** _wHiddenOutput;

		//_epoch counter
		long _epoch;
		long _maxEpochs;

		//accuracy required
		double _desiredAccuracy;

		//change to weights
		double** _deltaInputHidden;
		double** _deltaHiddenOutput;

		//error gradients
		double* _hiddenErrorGradients;
		double* _outputErrorGradients;

		//accuracy stats per _epoch
		double _trainingSetAccuracy;
		double _validationSetAccuracy;
		double _generalizationSetAccuracy;
		double _trainingSetMSE;
		double _validationSetMSE;
		double _generalizationSetMSE;

		//batch learning flag
		bool _useBatch;

		//log file handle
		bool _logResults;
		std::fstream _logFile;
		int _logResolution;
		int _lastEpochLogged;

		//public methods
		//----------------------------------------------------------------------------------------------------------------
	public:

		//constructor
		neuralNetwork(int in, int hidden, int out) : _nInput(in), _nHidden(hidden), _nOutput(out), _epoch(0), _logResults(false), _logResolution(10), _lastEpochLogged(-10), _trainingSetAccuracy(0), _validationSetAccuracy(0), _generalizationSetAccuracy(0), _trainingSetMSE(0), _validationSetMSE(0), _generalizationSetMSE(0)
		{				
			//create neuron lists
			//--------------------------------------------------------------------------------------------------------
			_inputNeurons = new( double[in + 1] );
			for ( int i=0; i < in; i++ ) _inputNeurons[i] = 0;

			//create bias neuron
			_inputNeurons[in] = -1;

			_hiddenNeurons = new( double[hidden + 1] );
			for ( int i=0; i < hidden; i++ ) _hiddenNeurons[i] = 0;

			//create bias neuron
			_hiddenNeurons[hidden] = -1;

			_outputNeurons = new( double[out] );
			for ( int i=0; i < out; i++ ) _outputNeurons[i] = 0;

			//create weight lists (include bias neuron weights)
			//--------------------------------------------------------------------------------------------------------
			_wInputHidden = new( double*[in + 1] );
			for ( int i=0; i <= in; i++ ) 
			{
				_wInputHidden[i] = new (double[hidden]);
				for ( int j=0; j < hidden; j++ ) _wInputHidden[i][j] = 0;		
			}

			_wHiddenOutput = new( double*[hidden + 1] );
			for ( int i=0; i <= hidden; i++ ) 
			{
				_wHiddenOutput[i] = new (double[out]);			
				for ( int j=0; j < out; j++ ) _wHiddenOutput[i][j] = 0;		
			}

			//create delta lists
			//--------------------------------------------------------------------------------------------------------
			_deltaInputHidden = new( double*[in + 1] );
			for ( int i=0; i <= in; i++ ) 
			{
				_deltaInputHidden[i] = new (double[hidden]);
				for ( int j=0; j < hidden; j++ ) _deltaInputHidden[i][j] = 0;		
			}

			_deltaHiddenOutput = new( double*[hidden + 1] );
			for ( int i=0; i <= hidden; i++ ) 
			{
				_deltaHiddenOutput[i] = new (double[out]);			
				for ( int j=0; j < out; j++ ) _deltaHiddenOutput[i][j] = 0;		
			}

			//create error gradient storage
			//--------------------------------------------------------------------------------------------------------
			_hiddenErrorGradients = new( double[hidden + 1] );
			for ( int i=0; i <= hidden; i++ ) _hiddenErrorGradients[i] = 0;

			_outputErrorGradients = new( double[out + 1] );
			for ( int i=0; i <= out; i++ ) _outputErrorGradients[i] = 0;

			//initialize weights
			//--------------------------------------------------------------------------------------------------------
			initializeWeights();

			//default learning parameters
			//--------------------------------------------------------------------------------------------------------
			_learningRate = LEARNING_RATE; 
			_momentum = MOMENTUM; 

			//use stochastic learning by default
			_useBatch = false;

			//stop conditions
			//--------------------------------------------------------------------------------------------------------
			_maxEpochs = MAX_EPOCHS;
			_desiredAccuracy = DESIRED_ACCURACY;			
		}

		//destructor
		~neuralNetwork()
		{
			//delete neurons
			delete[] _inputNeurons;
			delete[] _hiddenNeurons;
			delete[] _outputNeurons;

			//delete weight storage
			for (int i=0; i <= _nInput; i++) delete[] _wInputHidden[i];
			delete[] _wInputHidden;

			for (int j=0; j <= _nHidden; j++) delete[] _wHiddenOutput[j];
			delete[] _wHiddenOutput;

			//delete delta storage
			for (int i=0; i <= _nInput; i++) delete[] _deltaInputHidden[i];
			delete[] _deltaInputHidden;

			for (int j=0; j <= _nHidden; j++) delete[] _deltaHiddenOutput[j];
			delete[] _deltaHiddenOutput;

			//delete error gradients
			delete[] _hiddenErrorGradients;
			delete[] _outputErrorGradients;

			//close log file
			if ( _logFile.is_open() ) _logFile.close();
		}

		//set learning parameters
		void setLearningParameters(double lr, double m)
		{
			_learningRate = lr;		
			_momentum = m;
		}

		//set max _epoch
		void setMaxEpochs(int max)
		{
			_maxEpochs = max;
		}

		//set desired accuracy 
		void setDesiredAccuracy(float d)
		{
			_desiredAccuracy = d;
		}

		//enable batch learning
		void _useBatchLearning()
		{
			_useBatch = true;
		}

		//enable stochastic learning
		void useStochasticLearning()
		{
			_useBatch = false;
		}

		//enable logging of training results
		void enableLogging(const char* filename, int resolution = 1)
		{
			//create log file 
			if ( ! _logFile.is_open() )
			{
				_logFile.open(filename, std::ios::out);

				if ( _logFile.is_open() )
				{
					//write log file header
					_logFile << "Epoch,Training Set Accuracy, Generalization Set Accuracy,Training Set MSE, Generalization Set MSE" << std::endl;

					//enable logging
					_logResults = true;

					//resolution setting;
					_logResolution = resolution;
					_lastEpochLogged = -resolution;
				}
			}
		}

		//resets the neural network
		void resetWeights()
		{
			//reinitialize weights
			initializeWeights();		
		}

		//feed data through network
		double* feedInput( double* inputs)
		{
			//feed data into the network
			feedForward(inputs);

			//return results
			return _outputNeurons;
		}

		//train the network
		void trainNetwork(std::vector<DataEntry*> trainingSet, std::vector<DataEntry*> generalizationSet, std::vector<DataEntry*> validationSet )
		{
			std::cout<< std::endl << " Neural Network Training Starting: " << std::endl
				<< "==========================================================================" << std::endl
				<< " LR: " << _learningRate << ", Momentum: " << _momentum << ", Max Epochs: " << _maxEpochs << std::endl
				<< " " << _nInput << " Input Neurons, " << _nHidden << " Hidden Neurons, " << _nOutput << " Output Neurons" << std::endl
				<< "==========================================================================" << std::endl << std::endl;

			//reset _epoch and log counters
			_epoch = 0;
			_lastEpochLogged = -_logResolution;

			//train network using training dataset for training and generalization dataset for testing
			//--------------------------------------------------------------------------------------------------------
			while (	( _trainingSetAccuracy < _desiredAccuracy || _generalizationSetAccuracy < _desiredAccuracy ) && _epoch < _maxEpochs )				
			{			
				//store previous accuracy
				double previousTAccuracy = _trainingSetAccuracy;
				double previousGAccuracy = _generalizationSetAccuracy;

				//use training set to train network
				runTrainingEpoch( trainingSet );

				//get generalization set accuracy and MSE
				_generalizationSetAccuracy = getSetAccuracy( generalizationSet );
				_generalizationSetMSE = getSetMSE( generalizationSet );

				//Log Training results
				if (_logResults && _logFile.is_open() && ( _epoch - _lastEpochLogged == _logResolution ) ) 
				{
					_logFile << _epoch << "," << _trainingSetAccuracy << "," << _generalizationSetAccuracy << "," << _trainingSetMSE << "," << _generalizationSetMSE << std::endl;
					_lastEpochLogged = _epoch;
				}

				//print out change in training /generalization accuracy (only if a change is greater than a percent)
				if ( ceil(previousTAccuracy) != ceil(_trainingSetAccuracy) || ceil(previousGAccuracy) != ceil(_generalizationSetAccuracy) ) 
				{	
					std::cout << "Epoch :" << _epoch;
					std::cout << " TSet Acc:" << _trainingSetAccuracy << "%, MSE: " << _trainingSetMSE ;
					std::cout << " GSet Acc:" << _generalizationSetAccuracy << "%, MSE: " << _generalizationSetMSE << std::endl;				
				}

				//once training set is complete increment _epoch
				_epoch++;

			}//end while

			//get validation set accuracy and MSE
			_validationSetAccuracy = getSetAccuracy(validationSet);
			_validationSetMSE = getSetMSE(validationSet);

			//log end
			_logFile << _epoch << "," << _trainingSetAccuracy << "," << _generalizationSetAccuracy << "," << _trainingSetMSE << "," << _generalizationSetMSE << std::endl << std::endl;
			_logFile << "Training Complete!!! - > Elapsed Epochs: " << _epoch << " Validation Set Accuracy: " << _validationSetAccuracy << " Validation Set MSE: " << _validationSetMSE << std::endl;

			//out validation accuracy and MSE
			std::cout << std::endl << "Training Complete!!! - > Elapsed Epochs: " << _epoch << std::endl;
			std::cout << " Validation Set Accuracy: " << _validationSetAccuracy << std::endl;
			std::cout << " Validation Set MSE: " << _validationSetMSE << std::endl << std::endl;
		}


		//private methods
		//----------------------------------------------------------------------------------------------------------------
	private:

		//initialize weights and weight changes
		void initializeWeights()
		{
			//init random number generator
			srand( (unsigned int) time(0) );

			//set weights between input and hidden to a random value between -0.5 and 0.5
			//--------------------------------------------------------------------------------------------------------
			for(int i = 0; i <= _nInput; i++)
			{		
				for(int j = 0; j < _nHidden; j++) 
				{
					//set weights to random values
					_wInputHidden[i][j] = (double)rand() / (RAND_MAX + 1) - 0.5;

					//create blank delta
					_deltaInputHidden[i][j] = 0;
				}
			}

			//set weights between input and hidden to a random value between -05 and 0.5
			//--------------------------------------------------------------------------------------------------------
			for(int i = 0; i <= _nHidden; i++)
			{		
				for(int j = 0; j < _nOutput; j++) 
				{
					//set weights to random values
					_wHiddenOutput[i][j] = (double)rand() / (RAND_MAX + 1) - 0.5;

					//create blank delta
					_deltaHiddenOutput[i][j] = 0;
				}
			}
		}

		//run a single training _epoch
		void runTrainingEpoch(std::vector<DataEntry*> trainingSet )
		{
			//incorrect patterns
			double incorrectPatterns = 0;
			double mse = 0;

			//for every training pattern
			for ( int tp = 0; tp < (int) trainingSet.size(); tp++)
			{						
				//feed inputs through network and backpropagate errors
				feedForward( trainingSet[tp]->_inputs );
				backpropagate( trainingSet[tp]->_outputs );	

				//pattern correct flag
				bool patternCorrect = true;

				//check all outputs from neural network against desired values
				for ( int k = 0; k < _nOutput; k++ )
				{					
					//pattern incorrect if desired and output differ
					if ( getRoundedOutputValue( _outputNeurons[k] ) != trainingSet[tp]->_outputs[k] ) patternCorrect = false;

					//calculate MSE
					mse += pow((_outputNeurons[k] - trainingSet[tp]->_outputs[k]), 2);
				}

				//if pattern is incorrect add to incorrect count
				if ( !patternCorrect ) incorrectPatterns++;	

			}//end for

			//if using batch learning - update the weights
			if ( _useBatch ) updateWeights();

			//update training accuracy and MSE
			_trainingSetAccuracy = 100 - (incorrectPatterns/trainingSet.size() * 100);
			_trainingSetMSE = mse / ( _nOutput * trainingSet.size() );
		}

		//feed input forward
		void feedForward( double *inputs )
		{
			//set input neurons to input values
			for(int i = 0; i < _nInput; i++) _inputNeurons[i] = inputs[i];

			//Calculate Hidden Layer values - include bias neuron
			//--------------------------------------------------------------------------------------------------------
			for(int j=0; j < _nHidden; j++)
			{
				//clear value
				_hiddenNeurons[j] = 0;				

				//get weighted sum of inputs and bias neuron
				for( int i=0; i <= _nInput; i++ ) _hiddenNeurons[j] += _inputNeurons[i] * _wInputHidden[i][j];

				//set to result of sigmoid
				_hiddenNeurons[j] = activationFunction( _hiddenNeurons[j] );			
			}

			//Calculating Output Layer values - include bias neuron
			//--------------------------------------------------------------------------------------------------------
			for(int k=0; k < _nOutput; k++)
			{
				//clear value
				_outputNeurons[k] = 0;				

				//get weighted sum of inputs and bias neuron
				for( int j=0; j <= _nHidden; j++ ) _outputNeurons[k] += _hiddenNeurons[j] * _wHiddenOutput[j][k];

				//set to result of sigmoid
				_outputNeurons[k] = activationFunction( _outputNeurons[k] );
			}
		}

		//modify weights according to ouput
		void backpropagate( double* desiredValues )
		{		
			//modify deltas between hidden and output layers
			//--------------------------------------------------------------------------------------------------------
			for (int k = 0; k < _nOutput; k++)
			{
				//get error gradient for every output node
				_outputErrorGradients[k] = getOutputErrorGradient( desiredValues[k], _outputNeurons[k] );

				//for all nodes in hidden layer and bias neuron
				for (int j = 0; j <= _nHidden; j++) 
				{				
					//calculate change in weight
					if ( !_useBatch ) _deltaHiddenOutput[j][k] = _learningRate * _hiddenNeurons[j] * _outputErrorGradients[k] + _momentum * _deltaHiddenOutput[j][k];
					else _deltaHiddenOutput[j][k] += _learningRate * _hiddenNeurons[j] * _outputErrorGradients[k];
				}
			}

			//modify deltas between input and hidden layers
			//--------------------------------------------------------------------------------------------------------
			for (int j = 0; j < _nHidden; j++)
			{
				//get error gradient for every hidden node
				_hiddenErrorGradients[j] = getHiddenErrorGradient( j );

				//for all nodes in input layer and bias neuron
				for (int i = 0; i <= _nInput; i++)
				{
					//calculate change in weight 
					if ( !_useBatch ) _deltaInputHidden[i][j] = _learningRate * _inputNeurons[i] * _hiddenErrorGradients[j] + _momentum * _deltaInputHidden[i][j];
					else _deltaInputHidden[i][j] += _learningRate * _inputNeurons[i] * _hiddenErrorGradients[j]; 

				}
			}

			//if using stochastic learning update the weights immediately
			if ( !_useBatch ) updateWeights();
		}

		//update weights
		void updateWeights()
		{
			//input -> hidden weights
			//--------------------------------------------------------------------------------------------------------
			for (int i = 0; i <= _nInput; i++)
			{
				for (int j = 0; j < _nHidden; j++) 
				{
					//update weight
					_wInputHidden[i][j] += _deltaInputHidden[i][j];	

					//clear delta only if using batch (previous delta is needed for momentum
					if (_useBatch) _deltaInputHidden[i][j] = 0;				
				}
			}

			//hidden -> output weights
			//--------------------------------------------------------------------------------------------------------
			for (int j = 0; j <= _nHidden; j++)
			{
				for (int k = 0; k < _nOutput; k++) 
				{					
					//update weight
					_wHiddenOutput[j][k] += _deltaHiddenOutput[j][k];

					//clear delta only if using batch (previous delta is needed for momentum)
					if (_useBatch)_deltaHiddenOutput[j][k] = 0;
				}
			}
		}

		//activation function
		virtual inline double activationFunction( double x )
		{
			//sigmoid function
			return 1/(1+exp(-x));
		}

		//get error gradient for ouput layer
		inline double getOutputErrorGradient(double desiredValue, double outputValue)
		{
			//return error gradient
			return outputValue * ( 1 - outputValue ) * ( desiredValue - outputValue );
		}

		//get error gradient for hidden layer
		double getHiddenErrorGradient( int j )
		{
			//get sum of hidden->output weights * output error gradients
			double weightedSum = 0;
			for( int k = 0; k < _nOutput; k++ ) weightedSum += _wHiddenOutput[j][k] * _outputErrorGradients[k];

			//return error gradient
			return _hiddenNeurons[j] * ( 1 - _hiddenNeurons[j] ) * weightedSum;
		}

		//round up value to get a boolean result
		int getRoundedOutputValue( double x )
		{
			if ( x < 0.1 ) return 0;
			else if ( x > 0.9 ) return 1;
			else return -1;
		}	
		//feed forward set of patterns and return error
		double getSetAccuracy(std::vector<DataEntry*> set )
		{
			double incorrectResults = 0;

			//for every training input array
			for ( int tp = 0; tp < (int) set.size(); tp++)
			{						
				//feed inputs through network and backpropagate errors
				feedForward( set[tp]->_inputs );

				//correct pattern flag
				bool correctResult = true;

				//check all outputs against desired output values
				for ( int k = 0; k < _nOutput; k++ )
				{					
					//set flag to false if desired and output differ
					if ( getRoundedOutputValue(_outputNeurons[k]) != set[tp]->_outputs[k] ) correctResult = false;
				}

				//inc training error for a incorrect result
				if ( !correctResult ) incorrectResults++;	

			}//end for

			//calculate error and return as percentage
			return 100 - (incorrectResults/set.size() * 100);
		}

		//feed forward set of patterns and return MSE
		double getSetMSE(std::vector<DataEntry*> set )
		{
			double mse = 0;

			//for every training input array
			for ( int tp = 0; tp < (int) set.size(); tp++)
			{						
				//feed inputs through network and backpropagate errors
				feedForward( set[tp]->_inputs );

				//check all outputs against desired output values
				for ( int k = 0; k < _nOutput; k++ )
				{					
					//sum all the MSEs together
					mse += pow((_outputNeurons[k] - set[tp]->_outputs[k]), 2);
				}		

			}//end for

			//calculate error and return as percentage
			return mse/(_nOutput * set.size());
		}
	};
}