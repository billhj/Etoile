/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file SamplesGenerator.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <time.h>
#include <stdio.h>
#include <cmath>
#include <cstdlib> 
#include <iostream>
#include <vector>
#include "../math/Vectors.h"

namespace Etoile
{
	class SamplesGenerator
	{
	public:
		typedef Vec4d Sample;
		SamplesGenerator(const std::string& name);
		~SamplesGenerator();
		virtual void generateSamples(double lowerBound = -1.0f, double upperBound = 1.0f) = 0;
		void generateSameList(int index = 0)
		{
			srand(index);
		}
		std::vector<Sample> getSamples()
		{
			return samples;
		}
		void dumpPGM (const std::string & filename, unsigned int xSize, unsigned int ySize, unsigned int patternSize, unsigned int res = 100);

	protected:

		static Vec3d randomVec3 (double lowerBound = -1.0f, double upperBound = 1.0f) {
			return Vec3d (randomValue (lowerBound,upperBound), randomValue (lowerBound,upperBound), randomValue (lowerBound,upperBound));

		}
		static Vec4d randomVec4 (double lowerBound = -1.0f, double upperBound = 1.0f) {
			return Vec4d (randomValue (lowerBound,upperBound), randomValue (lowerBound,upperBound), randomValue (lowerBound,upperBound), randomValue (lowerBound,upperBound));

		}
		static double randomValue (double lowerBound = -1.0f, double upperBound = 1.0f) {
			return lowerBound + (double(rand() % 100000)/(100000 + 1)) * (upperBound - lowerBound);
		}

		std::vector<Sample> samples;

	public:
		void setPatternSize(int patternSize = 1)
		{
			_patternSize = patternSize;
		}

	protected:
		int _patternSize;
		std::string _name;
	};

	class LowDiscrepancySamplesGenerator : public SamplesGenerator
	{
	public:
		LowDiscrepancySamplesGenerator(const std::string& name):SamplesGenerator(name){}
		virtual Sample generateSample(const int l) const = 0 ;

		Vec4d mod(Vec4d v, Vec4d v2)
		{
			return Vec4d( modf(v.x(), &(v2[0])), modf(v.y(), &(v2[1])), modf(v.z(), &(v2[2])),modf(v.w(), &(v2[3])));
		}
		virtual void generateSamples(double lowerBound = -1.0f, double upperBound = 1.0f)
		{
			generateSameList();
			int incre = 0;
			for(int m = 0; m < _patternSize; ++m)
			{
				for(int n = 0; n < _patternSize; ++n)
				{
					const Vec4d offset = randomVec4 (0,1) ;//* ( m * _patternSize + n ) ;
					for(int l = 0; l < _xsize; ++l)
					{
						double range = upperBound - lowerBound;
						//Vec4d v = mod(Vec4d( Vec4d(generateSample(l)) + offset),( Vec4d(1,1,1,1))) * (range) + Vec4d(lowerBound,lowerBound,lowerBound,lowerBound);
						Vec4d v = mod(Vec4d( Vec4d(generateSample(incre))),( Vec4d(1,1,1,1))) * (range) + Vec4d(lowerBound,lowerBound,lowerBound,lowerBound);
						incre++;
						samples.push_back(v);
					}
				}
			}
		}

	protected:
		double radicalInverse(int n, int base) const
		{
			double value = 0;
			double invBase = 1.0/(double)(base), invBi = invBase;

			while(n > 0)
			{
				int d_i = (n % base);
				value += d_i * invBi;
				n /= base;
				invBi *= invBase;

			}
			return value;

		}

		double foldedRadicalInverse(int n, int base) const
		{
			double value = 0;
			double invBase = 1.0/(double)(base), invBi = invBase;

			int modOffset = 0;
			while(value + base * invBase != value)
			{
				int d_i = ( (n + modOffset) % base);
				value += d_i * invBi;
				n /= base;
				invBi *= invBase;
				++modOffset;
			}
			return value;

		}
	public:
		void setSize( const int xsize = 1, const int patternSize = 1)
		{
			_xsize = xsize, _patternSize = patternSize;

		}

	protected:

		int _xsize;

	};

	class QuadratureSamplesGenerator : public SamplesGenerator
	{
	public:
		QuadratureSamplesGenerator(const std::string& name):SamplesGenerator(name){}
		virtual Sample generateSample(const int l, const int k, const int j, const int i) const = 0 ;

		virtual void generateSamples(double lowerBound = -1.0f, double upperBound = 1.0f,bool intoTexture = true)
		{

			generateSameList();
			for(int m = 0; m < _patternSize; ++m)
			{
				for(int n = 0; n < _patternSize; ++n)
				{
					for(int i = 0; i <  _wsize; ++i)
					{
						for(int j = 0; j < _zsize; ++j)
						{
							for(int k = 0; k < _ysize; ++k)
							{
								for(int l = 0; l < _xsize; ++l)
								{
									double range = upperBound - lowerBound;
									Vec4d v = generateSample(l, k, j, i) * (range) + Vec4d(lowerBound,lowerBound,lowerBound,lowerBound);
									samples.push_back(v);
								}
							}
						}
					}
				}
			}
		}
	public:
		void setSize( const int xsize = 1, const int ysize = 1, const int zsize = 1, const int wsize = 1 ,const int patternSize = 1)
		{
			_xsize = xsize, _ysize = ysize, _zsize = zsize, _wsize = wsize, _patternSize = patternSize;
		}

	protected:
		int _xsize, _ysize,_zsize, _wsize;

	};

	class RandomSamplesGenerator : public QuadratureSamplesGenerator
	{

	public:
		RandomSamplesGenerator(const std::string& name):QuadratureSamplesGenerator(name){}
		virtual Sample generateSample(const int l, const int k, const int j, const int i) const
		{
			/*int n = l + k + j + i;
			n = n + 1;*/
			return randomVec4(0, 1);
		}
	};

	class RegularSamplesGenerator : public QuadratureSamplesGenerator
	{

	public:
		RegularSamplesGenerator(const std::string& name):QuadratureSamplesGenerator(name){}
		virtual Sample generateSample(const int l, const int k, const int j, const int i) const
		{
			const double x = (  (double(l)+0.5 ) /(double)(_xsize )     )  ;
			const double y = (  (double(k)+0.5 ) /(double)(_ysize )     )  ;
			const double z = (  (double(j)+0.5 ) /(double)(_zsize )     )  ;
			const double w = (  (double(i)+0.5 ) /(double)(_wsize )     )  ;
			return Vec4d(x, y, z, w);
		}
	};

	class JitteredSamplesGenerator : public QuadratureSamplesGenerator
	{

	public:

		JitteredSamplesGenerator(const std::string& name):QuadratureSamplesGenerator(name){}
		virtual Sample generateSample(const int l, const int k, const int j, const int i) const
		{
			const double x = (  (double(l)+ rand()/double(RAND_MAX) ) /(double)(_xsize )     ) ;
			const double y = (  (double(k)+ rand()/double(RAND_MAX) ) /(double)(_ysize )     ) ;
			const double z = (  (double(j)+ rand()/double(RAND_MAX) ) /(double)(_zsize )     ) ;
			const double w = (  (double(i)+ rand()/double(RAND_MAX) ) /(double)(_wsize )     ) ;

			return Vec4d(x, y, z, w);
		}
	};


	class HaltonSamplesGenerator : public LowDiscrepancySamplesGenerator
	{
	public:
		HaltonSamplesGenerator(const std::string& name):LowDiscrepancySamplesGenerator(name)
		{

		}

		virtual Sample generateSample(const int l) const
		{
			double x = radicalInverse(l + 10, 2)  ;
			double y = radicalInverse(l + 10, 3)  ;
			double z = radicalInverse(l + 10, 5)  ;
			double w = radicalInverse(l + 10, 7)  ;
			return Vec4d(x, y, z, w);
		}
	};

	class HammersleySamplesGenerator : public LowDiscrepancySamplesGenerator
	{
	public:
		HammersleySamplesGenerator(const std::string& name):LowDiscrepancySamplesGenerator(name)
		{

		}

		virtual Sample generateSample(const int l) const
		{
			double x = (double)(l)/(double)(_xsize) ;
			double y = radicalInverse(l + 10, 2)  ;
			double z = radicalInverse(l + 10, 3)  ;
			double w = radicalInverse(l + 10, 5)  ;

			return Vec4d(x, y, z, w);
		}

	};
}
