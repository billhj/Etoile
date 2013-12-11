/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file SamplesGenerator.cpp
* @brief 
* @date 1/2/2011
*/

#include "SamplesGenerator.h"
#include <fstream>

/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

namespace Etoile
{
	SamplesGenerator::SamplesGenerator(const std::string& name): _name(name)
	{
	}

	SamplesGenerator::~SamplesGenerator()
	{

	}
	void SamplesGenerator::dumpPGM (const std::string & filename, unsigned int xSize, unsigned int ySize, unsigned int patternSize, unsigned int res )
	{
		unsigned int width =  res * patternSize;
		unsigned int size = width * width;
		std::vector<int> raster;
		raster.resize(size);
		ofstream out (filename.c_str ());
		out << "P2" << std::endl
			<< width << " " << width << std::endl << 255 << std::endl;
		for (unsigned int i = 0; i < width; i++){
			for(unsigned int j = 0; j < width; j++){
				if( (j) % res == 0 || i % res ==0 || i == width -1 || j == width - 1)
					raster[i * width +j] = 150;
				else
					raster[i * width +j] = 255;
			}
		}

		for(unsigned int l = 0; l < patternSize; ++l )
		{
			for(unsigned int k = 0; k < patternSize; ++k )
			{
				for(unsigned int j = 0; j < ySize; ++j)
				{
					for(unsigned int i = 0; i < xSize; ++i)
					{
						unsigned int index = l * patternSize * ySize * xSize + k * ySize * xSize + j * xSize + i;
						const Vec4d & s = samples[index];
						unsigned int x = unsigned int(k * res + (s.x()/2.0+0.5) * res);
						unsigned int y = unsigned int(l * res + (s.y()/2.0+0.5) * res);
						int vidx = y*width+x;
						raster[vidx] = 0;
					}
				}
			}
		}
		for (unsigned int i = 0; i < size; i++)
			out << raster[i] << endl;

		out.close ();
	}
}
