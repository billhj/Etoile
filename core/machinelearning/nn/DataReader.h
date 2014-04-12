/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file DataReader.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include <fstream>
#include <string>
#include "DataSet.h"

namespace Etoile
{	
	//data reader class
	class DataReader
	{
	public:

		//constructor
		DataReader()
		{				
		}

		//destructor
		~DataReader()
		{
		}

		//read data from file
		bool loadDataFromFile(const std::string& filename, int nInput, int nOutput, std::vector<DataEntry*>& data)
		{
			//clear any previous data
			for (int i=0; i < (int) data.size(); i++ )
			{
				delete data[i];		
			}
			data.clear();
			//open file for reading
			std::fstream inputFile;
			inputFile.open(filename, std::ios::in);	

			if ( inputFile.is_open() )
			{
				std::string line = "";
				//read data
				while ( !inputFile.eof() )
				{
					getline(inputFile, line);				
					//process line
					if (line.length() > 2 ) processLine(line, nInput, nOutput, data);
				}		
				//print success
				std::cout << "Data File Read Complete >> data size: " << data.size() << std::endl;
				//close file
				inputFile.close();
				return true;
			}
			else return false;
		}

	
		//private methods
		//----------------------------------------------------------------------------------------------------------------
	
	private:
		void processLine( std::string &line, int nInputs, int nOutputs, std::vector<DataEntry*>& data)
		{
			//create new pattern and target
			double* pattern = new double[nInputs];
			double* target = new double[nOutputs];

			//store inputs		
			char* cstr = new char[line.size()+1];
			char* t;
			strcpy(cstr, line.c_str());

			//tokenise
			int i = 0;
			t=strtok (cstr,",");

			while ( t!=NULL && i < (nInputs + nOutputs) )
			{	
				if ( i < nInputs ) pattern[i] = atof(t);
				else target[i - nInputs] = atof(t);

				//move token onwards
				t = strtok(NULL,",");
				i++;			
			}

			/*cout << "pattern: ";
			for (int i=0; i < _nInputs; i++) 
			{
			cout << pattern[i] << ",";
			}

			cout << " target: ";
			for (int i=0; i < _nTargets; i++) 
			{
			cout << target[i] << " ";
			}
			cout << endl;*/

			//add to records
			data.push_back(new DataEntry(pattern, target));		
		}
	};

}