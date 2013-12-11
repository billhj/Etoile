/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file DataSet.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>

namespace Etoile
{
	//DataSet retrieval approach enum
	enum Training_Approach_Type{ NONE = 0, STATIC, GROWING, WINDOWING };

	struct DataEntry
	{
	public:
		double* _inputs;	//all the input patterns
		double* _outputs;		//all the output targets

	public:
		DataEntry(double* p, double* t): _inputs(p), _outputs(t) {}

		~DataEntry()
		{				
			delete[] _inputs;
			delete[] _outputs;
		}
	};


	class DataSet
	{

		std::vector<DataEntry*> _data; //originalData
		std::vector<DataEntry*> _trainingSet;
		std::vector<DataEntry*> _generalizationSet;
		std::vector<DataEntry*> _validationSet;

		//data set creation approach and total number of DataSets
		Training_Approach_Type _creationApproach;
		double _trainingPercentage;
		double _generalizationPercentage;

		int _numDataSets;
		int _trainingDataEndIndex;
		
		//creation approach variables
		double _growingStepSize;			//step size - percentage of total set
		int _growingLastDataIndex;		//last index added to current DataSet
		int _windowingSetSize;			//initial size of set
		int _windowingStepSize;			//how many entries to move window by
		int _windowingStartIndex;		//window start index

	public:
		DataSet(): _creationApproach(NONE), _numDataSets(-1), _trainingPercentage(0.6), _generalizationPercentage(0.2)
		{		
		}

		~DataSet()
		{
			clear();
		}

		void clear()
		{
			for(unsigned int i = 0; i < _data.size(); ++i)
			{
				delete _data[i];
			}
			_data.clear();
			_trainingSet.clear();
			_generalizationSet.clear();
			_validationSet.clear();
		}

		std::vector<DataEntry*>& getTrainingSet(){return _trainingSet;}
		std::vector<DataEntry*>& getGeneralizationSet(){return _generalizationSet;}
		std::vector<DataEntry*>& getValidationSet(){return _validationSet;}
		std::vector<DataEntry*>& getDataSet(){return _data;}

		void buildDataSets()
		{
			//shuffle data
			std::random_shuffle(_data.begin(), _data.end());
			//split data set
			_trainingDataEndIndex = (int) (_trainingPercentage * _data.size() );
			int gSize = (int) ( ceil(_generalizationPercentage * _data.size()) );
			int vSize = (int) ( _data.size() - _trainingDataEndIndex - gSize );

			//training set
			createStaticTraingDataSet();
			//generalization set
			for ( int i = _trainingDataEndIndex; i < _trainingDataEndIndex + gSize; i++ ) 
			{
				_generalizationSet.push_back( _data[i] );
			}

			//validation set
			for ( int i = _trainingDataEndIndex + gSize; i < (int) _data.size(); i++ )
			{
				_validationSet.push_back( _data[i] );
			}
		}


		//select DataSet approach
		void setCreationApproach(Training_Approach_Type approach, double param1 = -1, double param2 = -1 )
		{
			//static
			if ( approach == STATIC )
			{
				_creationApproach = STATIC;

				//only 1 data set
				_numDataSets = 1;
			}

			//growing
			else if ( approach == GROWING )
			{			
				if ( param1 <= 100.0 && param1 > 0)
				{
					_creationApproach = GROWING;

					//step size
					_growingStepSize = param1 / 100;
					_growingLastDataIndex = 0;

					//number of sets
					_numDataSets = (int) ceil( 1 / _growingStepSize );				
				}
			}

			//windowing
			else if ( approach == WINDOWING )
			{
				//if initial size smaller than total entries and step size smaller than set size
				if ( param1 < _data.size() && param2 <= param1)
				{
					_creationApproach = WINDOWING;

					//params
					_windowingSetSize = (int) param1;
					_windowingStepSize = (int) param2;
					_windowingStartIndex = 0;			

					//number of sets
					_numDataSets = (int) ceil( (double) ( _trainingDataEndIndex - _windowingSetSize ) / _windowingStepSize ) + 1;
				}			
			}

		}

		//return number of DataSets
		int numberOfTrainingDataSets()
		{
			return _numDataSets;
		}

		void resetGrowingTrainingIndex()
		{
			_growingLastDataIndex = 0;
		}

		std::vector<DataEntry*> getNextGrowingTrainingDataSet()
		{
			//increase data set by step percentage
			_growingLastDataIndex += (int) ceil( _growingStepSize * _trainingDataEndIndex );		
			if ( _growingLastDataIndex > (int) _trainingDataEndIndex )
			{
				_growingLastDataIndex = _trainingDataEndIndex;
			}

			std::vector<DataEntry*> trainingSet;
			//training set
			for (int i = 0; i < _growingLastDataIndex; i++)
			{
				trainingSet.push_back(_data[i]);		
			}
			return trainingSet;
		}

		void resetWindowingTrainingIndex()
		{
			_windowingStartIndex = 0;
		}

		std::vector<DataEntry*> getNextWindowingTrainingDataSet()
		{
			//create end point
			int endIndex = _windowingStartIndex + _windowingSetSize;
			if ( endIndex > _trainingDataEndIndex )
			{ 
				endIndex = _trainingDataEndIndex;		
			}

			std::vector<DataEntry*> trainingSet;

			//training set
			for (int i = _windowingStartIndex; i < endIndex; i++)
			{
				trainingSet.push_back(_data[i]);
			}
			//increase start index
			_windowingStartIndex += _windowingStepSize;
			return trainingSet;
		}

	private:

		void createStaticTraingDataSet()
		{
			//training set
			for ( int i = 0; i < _trainingDataEndIndex; i++ )
			{
				_trainingSet.push_back(_data[i]);		
			}
		}

	};
}
