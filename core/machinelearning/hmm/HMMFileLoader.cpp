/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file HMMFileLoader.cpp
* @brief 
* @date 1/2/2011
*/

#include "HMMFileLoader.h"
#include <iostream>
#include <sstream>

namespace Etoile
{
	HMMFileLoader::HMMFileLoader(void)
	{

	}


	HMMFileLoader::~HMMFileLoader(void)
	{
	}

	bool HMMFileLoader::loadFromFile(const std::string& filepath, HMMModel* model)
	{
		std::fstream in( filepath.c_str(), std::ios_base::in );

		if (!in.is_open() || !in.good())
		{
			std::cerr << "[HMMFileLoader] : cannot not open file "
				<< filepath
				<< std::endl;
			return false;
		}

		return read(in, model);
	}

	bool HMMFileLoader::read(std::istream& in, HMMModel* model)
	{
		std::cout << "[HMMFileLoader] : read file"<<std::endl;

		std::string line;
		std::string keyWrd;
		int m = 0; //observationTypes
		int n = 0; //states
		int nO = 0; //observationList
		while( in && !in.eof() )
		{
			std::getline(in,line);
			if ( in.bad() ){
				std::cout << "  Warning! Could not read file properly!"<<std::endl;
				return false;
			}

			// comment
			if ( line.size() == 0 || line[0] == '#' || isspace(line[0]) || line.empty() ) {
				continue;
			}

			std::stringstream stream(line);
			stream >> keyWrd;
			if (keyWrd == "M=")
			{
				stream >> m;
				model->createObservations(m);
				std::cout<< "M= " <<m<<std::endl;
			}
			else if (keyWrd == "N=")
			{
				stream >> n;
				model->createStates(n);
				std::cout<< "N= " <<n<<std::endl;
			}
			else if (keyWrd == "A:")
			{
				std::cout<< "A:"<<std::endl;
				model->initStateTransitionMatrix();
				for(int i = 0; i < n; ++i)
				{
					std::getline(in,line);
					std::stringstream stream(line);
					for(int j = 0; j < n; ++j)
					{
						double value;
						stream >> value;
						HMMStateTransition t;
						t.setProbability(value);
						model->fillStateTransitionMatrix(i, j, t);
						std::cout<<value<<" ";
					}
					std::cout<<std::endl;
				}
			}
			else if (keyWrd == "B:")
			{
				std::cout<< "B:"<<std::endl;
				model->initStateObservationMatrix();
				for(int i = 0; i < n; ++i)
				{
					std::getline(in,line);
					std::stringstream stream(line);
					for(int j = 0; j < m; ++j)
					{
						double value;
						stream >> value;
						HMMStateObservation o;
						o.setProbability(value);
						model->fillStateObservationMatrix(i, j, o);
						std::cout<<value<<" ";
					}
					std::cout<<std::endl;
				}
			}
			else if (keyWrd == "pi:")
			{
				std::cout<< "pi:"<<std::endl;
				std::getline(in,line);
				std::stringstream stream(line);
				std::vector<double> pi;
				for(int i = 0; i < n; ++i)
				{
					double value;
					stream >> value;
					pi.push_back(value);
					std::cout<< value<<" ";
				}
				std::cout<<std::endl;
				model->setInitialProbabilityPi(pi);
			}
			else if (keyWrd == "T=")
			{
				stream >> nO;
				std::cout<< "T= "<<nO<<std::endl;
				std::getline(in,line);
				std::stringstream stream(line);
				std::vector<int> o;
				for(int i = 0; i < nO; ++i)
				{
					int value;
					stream >> value;
					o.push_back(value);
					std::cout<<value<<" ";
				}
				std::cout<<std::endl;
				model->setObservationList(o);
			}

		}

		return true;
	}

}