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
#include "Neuron.h"

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
	class NeuralNetwork
	{

		//private members
		//----------------------------------------------------------------------------------------------------------------
	private:

		//learning parameters
		double learningRate;					// adjusts the step size of the weight update	
		double momentum;						// improves performance of stochastic learning (don't use for batch)

		//number of neurons
		int nInput, nHidden, nOutput;

		//neurons
		std::vector<InputNeuron> _inputNeurons;
		std::vector<HiddenNeuron> _hiddenNeurons;
		std::vector<OutputNeuron> _outputNeurons;

		//weights
		double** wInputHidden;
		double** wHiddenOutput;

		//epoch counter
		long epoch;
		long maxEpochs;

		//accuracy required
		double desiredAccuracy;

		//change to weights
		double** deltaInputHidden;
		double** deltaHiddenOutput;

		//error gradients
		double* hiddenErrorGradients;
		double* outputErrorGradients;

		//accuracy stats per epoch
		double trainingSetAccuracy;
		double validationSetAccuracy;
		double generalizationSetAccuracy;
		double trainingSetMSE;
		double validationSetMSE;
		double generalizationSetMSE;

		//batch learning flag
		bool useBatch;

		//log file handle
		bool logResults;
		std::fstream logFile;
		int logResolution;
		int lastEpochLogged;

		//Function1D* _pFunction;
	public:

		NeuralNetwork(int in, int hidden, int out) : nInput(in), nHidden(hidden), nOutput(out), epoch(0), logResults(false), logResolution(10), lastEpochLogged(-10), trainingSetAccuracy(0), validationSetAccuracy(0), generalizationSetAccuracy(0), trainingSetMSE(0), validationSetMSE(0), generalizationSetMSE(0)
		{				
			//create neuron lists
			//--------------------------------------------------------------------------------------------------------
			_inputNeurons.resize(in + 1);
			//create bias neuron
			_inputNeurons[in].setOutput(-1);   //one more neuron use for bias, displace the center of the function

			_hiddenNeurons.resize(hidden + 1);
			//create bias neuron
			_hiddenNeurons[hidden].setOutput(-1); //use for bias, displace the center of the function

			_outputNeurons.resize(out);
		
			//create weight lists (include bias neuron weights)
			//--------------------------------------------------------------------------------------------------------
			wInputHidden = new( double*[in + 1] );
			for ( int i=0; i <= in; i++ ) 
			{
				wInputHidden[i] = new (double[hidden]);
				for ( int j=0; j < hidden; j++ ) wInputHidden[i][j] = 0;		
			}

			wHiddenOutput = new( double*[hidden + 1] );
			for ( int i=0; i <= hidden; i++ ) 
			{
				wHiddenOutput[i] = new (double[out]);			
				for ( int j=0; j < out; j++ ) wHiddenOutput[i][j] = 0;		
			}

			//create delta lists
			//--------------------------------------------------------------------------------------------------------
			deltaInputHidden = new( double*[in + 1] );
			for ( int i=0; i <= in; i++ ) 
			{
				deltaInputHidden[i] = new (double[hidden]);
				for ( int j=0; j < hidden; j++ ) deltaInputHidden[i][j] = 0;		
			}

			deltaHiddenOutput = new( double*[hidden + 1] );
			for ( int i=0; i <= hidden; i++ ) 
			{
				deltaHiddenOutput[i] = new (double[out]);			
				for ( int j=0; j < out; j++ ) deltaHiddenOutput[i][j] = 0;		
			}

			//create error gradient storage
			//--------------------------------------------------------------------------------------------------------
			hiddenErrorGradients = new( double[hidden + 1] );
			for ( int i=0; i <= hidden; i++ ) hiddenErrorGradients[i] = 0;

			outputErrorGradients = new( double[out + 1] );
			for ( int i=0; i <= out; i++ ) outputErrorGradients[i] = 0;

			//initialize weights
			//--------------------------------------------------------------------------------------------------------
			initializeWeights();

			//default learning parameters
			//--------------------------------------------------------------------------------------------------------
			learningRate = LEARNING_RATE; 
			momentum = MOMENTUM; 

			//use stochastic learning by default
			useBatch = false;

			//stop conditions
			//--------------------------------------------------------------------------------------------------------
			maxEpochs = MAX_EPOCHS;
			desiredAccuracy = DESIRED_ACCURACY;	
			//_pFunction = new Sigmoid();
		}

		//destructor
		~NeuralNetwork()
		{
			//delete neurons
			_inputNeurons.clear();
			_hiddenNeurons.clear();
			_outputNeurons.clear();

			//delete weight storage
			for (int i=0; i <= nInput; i++) delete[] wInputHidden[i];
			delete[] wInputHidden;

			for (int j=0; j <= nHidden; j++) delete[] wHiddenOutput[j];
			delete[] wHiddenOutput;

			//delete delta storage
			for (int i=0; i <= nInput; i++) delete[] deltaInputHidden[i];
			delete[] deltaInputHidden;

			for (int j=0; j <= nHidden; j++) delete[] deltaHiddenOutput[j];
			delete[] deltaHiddenOutput;

			//delete error gradients
			delete[] hiddenErrorGradients;
			delete[] outputErrorGradients;
			//delete _pFunction;
			//close log file
			if ( logFile.is_open() ) logFile.close();
		}

		/*void setActivationFunction(Function1D* f)
		{
			_pFunction = f;
		}*/

		void setHiddenLayerActivationFunction(Function1D* f)
		{
			for(unsigned int i = 0; i < _hiddenNeurons.size(); ++i)
			{
				_hiddenNeurons[i].setActivationFunction(f);
			}
		}

		void setOutputLayerActivationFunction(Function1D* f)
		{
			for(unsigned int i = 0; i < _outputNeurons.size(); ++i)
			{
				_outputNeurons[i].setActivationFunction(f);
			}
		}

		//set learning parameters
		void setLearningParameters(double lr, double m)
		{
			learningRate = lr;		
			momentum = m;
		}

		//set max epoch
		void setMaxEpochs(int max)
		{
			maxEpochs = max;
		}

		//set desired accuracy 
		void setDesiredAccuracy(float d)
		{
			desiredAccuracy = d;
		}

		//enable batch learning
		void useBatchLearning()
		{
			useBatch = true;
		}

		//enable stochastic learning
		void useStochasticLearning()
		{
			useBatch = false;
		}

		//enable logging of training results
		void enableLogging(const char* filename, int resolution = 1)
		{
			//create log file 
			if ( ! logFile.is_open() )
			{
				logFile.open(filename, std::ios::out);

				if ( logFile.is_open() )
				{
					//write log file header
					logFile << "Epoch,Training Set Accuracy, Generalization Set Accuracy,Training Set MSE, Generalization Set MSE" << endl;

					//enable logging
					logResults = true;

					//resolution setting;
					logResolution = resolution;
					lastEpochLogged = -resolution;
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
		double* feedInput(double* inputs)
		{
			//feed data into the network
			feedForward(inputs);

			//return results
			double* value = new double[nOutput];
			for(int i = 0; i < nOutput; ++i)
			{
				value[i] = _outputNeurons[i].getOutput();
			}
			return value;
		}

		//train the network
		void trainNetwork(std::vector<DataEntry*>& trainingSet, std::vector<DataEntry*>& generalizationSet, std::vector<DataEntry*>& validationSet )
		{
			std::cout<< std::endl << " Neural Network Training Starting: " << std::endl
				<< "==========================================================================" << std::endl
				<< " LR: " << learningRate << ", Momentum: " << momentum << ", Max Epochs: " << maxEpochs << std::endl
				<< " " << nInput << " Input Neurons, " << nHidden << " Hidden Neurons, " << nOutput << " Output Neurons" << std::endl
				<< "==========================================================================" << std::endl << std::endl;

			//reset epoch and log counters
			epoch = 0;
			lastEpochLogged = -logResolution;

			//train network using training DataSet for training and generalization DataSet for testing
			//--------------------------------------------------------------------------------------------------------
			while (	( trainingSetAccuracy < desiredAccuracy || generalizationSetAccuracy < desiredAccuracy ) && epoch < maxEpochs )				
			{			
				//store previous accuracy
				double previousTAccuracy = trainingSetAccuracy;
				double previousGAccuracy = generalizationSetAccuracy;

				//use training set to train network
				runTrainingEpoch(trainingSet);

				//get generalization set accuracy and MSE
				generalizationSetAccuracy = getSetAccuracy( generalizationSet );
				generalizationSetMSE = getSetMSE( generalizationSet );

				//Log Training results
				if (logResults && logFile.is_open() && ( epoch - lastEpochLogged == logResolution ) ) 
				{
					logFile << epoch << "," << trainingSetAccuracy << "," << generalizationSetAccuracy << "," << trainingSetMSE << "," << generalizationSetMSE << std::endl;
					lastEpochLogged = epoch;
				}

				//print out change in training /generalization accuracy (only if a change is greater than a percent)
				if ( ceil(previousTAccuracy) != ceil(trainingSetAccuracy) || ceil(previousGAccuracy) != ceil(generalizationSetAccuracy) ) 
				{	
					std::cout << "Epoch :" << epoch;
					std::cout << " TrainingSet Accuracy:" << trainingSetAccuracy << "%, MSE: " << trainingSetMSE ;
					std::cout << " GeneralizationSet Accuracy:" << generalizationSetAccuracy << "%, MSE: " << generalizationSetMSE << std::endl;				
				}

				//once training set is complete increment epoch
				epoch++;

			}//end while

			//get validation set accuracy and MSE
			validationSetAccuracy = getSetAccuracy(validationSet);
			validationSetMSE = getSetMSE(validationSet);

			//log end
			logFile << epoch << "," << trainingSetAccuracy << "," << generalizationSetAccuracy << "," << trainingSetMSE << "," << generalizationSetMSE << std::endl << std::endl;
			logFile << "Training Complete!!! - > Elapsed Epochs: " << epoch << " Validation Set Accuracy: " << validationSetAccuracy << " Validation Set MSE: " << validationSetMSE << std::endl;

			//out validation accuracy and MSE
			std::cout << std::endl << "Training Complete!!! - > Elapsed Epochs: " << epoch << std::endl;
			std::cout << " Validation Set Accuracy: " << validationSetAccuracy << std::endl;
			std::cout << " Validation Set MSE: " << validationSetMSE << std::endl << std::endl;
		}


		//private methods
		//----------------------------------------------------------------------------------------------------------------
	private:

		//initialize weights and weight changes
		void initializeWeights()
		{
			//init random number generator
			srand( (unsigned int) time(0) );

			//set weights between input and hidden to a random value between -05 and 0.5
			//--------------------------------------------------------------------------------------------------------
			for(int i = 0; i <= nInput; i++)
			{		
				for(int j = 0; j < nHidden; j++) 
				{
					//set weights to random values
					wInputHidden[i][j] = (double)rand() / (RAND_MAX + 1) - 0.5;

					//create blank delta
					deltaInputHidden[i][j] = 0;
				}
			}

			//set weights between input and hidden to a random value between -05 and 0.5
			//--------------------------------------------------------------------------------------------------------
			for(int i = 0; i <= nHidden; i++)
			{		
				for(int j = 0; j < nOutput; j++) 
				{
					//set weights to random values
					wHiddenOutput[i][j] = (double)rand() / (RAND_MAX + 1) - 0.5;

					//create blank delta
					deltaHiddenOutput[i][j] = 0;
				}
			}
		}

		//run a single training epoch
		void runTrainingEpoch(std::vector<DataEntry*>& trainingSet )
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
				for ( int k = 0; k < nOutput; k++ )
				{					
					//pattern incorrect if desired and output differ
					if ( getRoundedOutputValue( _outputNeurons[k].getOutput()) != trainingSet[tp]->_outputs[k] ) patternCorrect = false;

					//calculate MSE
					mse += pow((_outputNeurons[k].getOutput() - trainingSet[tp]->_outputs[k]), 2);
				}

				//if pattern is incorrect add to incorrect count
				if ( !patternCorrect ) incorrectPatterns++;	

			}//end for

			//if using batch learning - update the weights
			if ( useBatch ) updateWeights();

			//update training accuracy and MSE
			trainingSetAccuracy = 100 - (incorrectPatterns/trainingSet.size() * 100);
			trainingSetMSE = mse / ( nOutput * trainingSet.size() );
		}

		//feed input forward
		void feedForward( double *inputs )
		{
			//set input neurons to input values
			for(int i = 0; i < nInput; i++)
			{
				_inputNeurons[i].setInput(inputs[i]);
			}

			//Calculate Hidden Layer values - include bias neuron
			//--------------------------------------------------------------------------------------------------------
			for(int j=0; j < nHidden; j++)
			{
				//clear value
				_hiddenNeurons[j].setInput(0);				

				//get weighted sum of inputs and bias neuron
				for( int i=0; i <= nInput; i++ )
				{
					_hiddenNeurons[j].addInput(_inputNeurons[i].getOutput() * wInputHidden[i][j]);
				}

				//set to result of sigmoid
				_hiddenNeurons[j].compute();			
			}

			//Calculating Output Layer values - include bias neuron
			//--------------------------------------------------------------------------------------------------------
			for(int k=0; k < nOutput; k++)
			{
				//clear value
				_outputNeurons[k].setInput(0);				

				//get weighted sum of inputs and bias neuron
				for( int j=0; j <= nHidden; j++ )
				{ 
					_outputNeurons[k].addInput(_hiddenNeurons[j].getOutput() * wHiddenOutput[j][k]);
				}

				//set to result of sigmoid
				_outputNeurons[k].compute();
			}
		}

		//modify weights according to ouput
		void backpropagate( double* desiredValues )
		{		
			//modify deltas between hidden and output layers
			//--------------------------------------------------------------------------------------------------------
			for (int k = 0; k < nOutput; k++)
			{
				//get error gradient for every output node
				outputErrorGradients[k] = getOutputErrorGradient(_outputNeurons[k].getActivationFunction(), desiredValues[k], _outputNeurons[k].getOutput());

				//for all nodes in hidden layer and bias neuron
				for (int j = 0; j <= nHidden; j++) 
				{				
					//calculate change in weight
					if ( !useBatch ) deltaHiddenOutput[j][k] = learningRate * _hiddenNeurons[j].getOutput() * outputErrorGradients[k] + momentum * deltaHiddenOutput[j][k];
					else deltaHiddenOutput[j][k] += learningRate * _hiddenNeurons[j].getOutput() * outputErrorGradients[k];
				}
			}

			//modify deltas between input and hidden layers
			//--------------------------------------------------------------------------------------------------------
			for (int j = 0; j < nHidden; j++)
			{
				//get error gradient for every hidden node
				hiddenErrorGradients[j] = getHiddenErrorGradient(_hiddenNeurons[j].getActivationFunction(), j);

				//for all nodes in input layer and bias neuron
				for (int i = 0; i <= nInput; i++)
				{
					//calculate change in weight 
					if ( !useBatch ) deltaInputHidden[i][j] = learningRate * _inputNeurons[i].getOutput() * hiddenErrorGradients[j] + momentum * deltaInputHidden[i][j];
					else deltaInputHidden[i][j] += learningRate * _inputNeurons[i].getOutput() * hiddenErrorGradients[j]; 

				}
			}

			//if using stochastic learning update the weights immediately
			if ( !useBatch ) updateWeights();
		}

		//update weights
		void updateWeights()
		{
			//input -> hidden weights
			//--------------------------------------------------------------------------------------------------------
			for (int i = 0; i <= nInput; i++)
			{
				for (int j = 0; j < nHidden; j++) 
				{
					//update weight
					wInputHidden[i][j] += deltaInputHidden[i][j];	

					//clear delta only if using batch (previous delta is needed for momentum
					if (useBatch) deltaInputHidden[i][j] = 0;				
				}
			}

			//hidden -> output weights
			//--------------------------------------------------------------------------------------------------------
			for (int j = 0; j <= nHidden; j++)
			{
				for (int k = 0; k < nOutput; k++) 
				{					
					//update weight
					wHiddenOutput[j][k] += deltaHiddenOutput[j][k];

					//clear delta only if using batch (previous delta is needed for momentum)
					if (useBatch)deltaHiddenOutput[j][k] = 0;
				}
			}
		}

	/*	//activation function
		inline double activationFunction( double x )
		{
			//sigmoid function
			return _pFunction->get(x);
		}
		*/

		//get error gradient for ouput layer
		inline double getOutputErrorGradient(Function1D* f, double desiredValue, double outputValue)
		{
			//return error gradient
			return f->getGradient(outputValue) * ( desiredValue - outputValue );
		}

		//get error gradient for hidden layer
		double getHiddenErrorGradient(Function1D* f, int j )
		{
			//get sum of hidden->output weights * output error gradients
			double weightedSum = 0;
			for( int k = 0; k < nOutput; k++ )
			{ 
				weightedSum += wHiddenOutput[j][k] * outputErrorGradients[k];
			}

			//return error gradient
			return f->getGradient(_hiddenNeurons[j].getOutput()) * weightedSum;
		}

		//round up value to get a boolean result
		int getRoundedOutputValue( double x )
		{
			if ( x < 0.1 ) return 0;
			else if ( x > 0.9 ) return 1;
			else return -1;
		}	
		//feed forward set of patterns and return error
		double getSetAccuracy(std::vector<DataEntry*>& set )
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
				for ( int k = 0; k < nOutput; k++ )
				{					
					//set flag to false if desired and output differ
					if ( getRoundedOutputValue(_outputNeurons[k].getOutput()) != set[tp]->_outputs[k] ) correctResult = false;
				}

				//inc training error for a incorrect result
				if ( !correctResult ) incorrectResults++;	

			}//end for

			//calculate error and return as percentage
			return 100 - (incorrectResults/set.size() * 100);
		}

		//feed forward set of patterns and return MSE
		double getSetMSE (std::vector<DataEntry*> set )
		{
			double mse = 0;

			//for every training input array
			for ( int tp = 0; tp < (int) set.size(); tp++)
			{						
				//feed inputs through network and backpropagate errors
				feedForward( set[tp]->_inputs );

				//check all outputs against desired output values
				for ( int k = 0; k < nOutput; k++ )
				{					
					//sum all the MSEs together
					mse += pow((_outputNeurons[k].getOutput() - set[tp]->_outputs[k]), 2);
				}		

			}//end for

			//calculate error and return as percentage
			return mse/(nOutput * set.size());
		}
	};
}