/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Gaussian.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <math.h>

namespace Etoile
{
	template <class T>
	class Gaussian
	{

	public:

		/**
		One step in the forward gaussian transform
		*/
		void forwardStep(T* input, const int length, const int HalfWindowSize, double sigma)
		{
			T* media = new T[length];
			double* weights = new double[HalfWindowSize + 1];
			calculateWeights(weights,HalfWindowSize, sigma);

			for(int x = 0; x < length; ++x)
			{
				T sum;
				double weightSum = 0;
				for(int i = -HalfWindowSize; i <= HalfWindowSize; ++i)
				{
					int index = x + i;
					if(index < length && index >= 0)
					{
						double weight = weights[ abs(i) ];
						sum += input[index] * weight;
						weightSum += weight;
					}
				}
				media[x] = (sum / weightSum);
			}

			for(int x = 0; x < length; ++x)
			{
				input[x] = media[x];
			}
			delete[] media;

		} // forwardStep



	protected:
		void calculateWeights(double* weights,const int HalfWindowSize, double sigma)
		{
			for(int i = 0; i <= HalfWindowSize; ++i)
			{
				weights[i] =  gaussianWeight(HalfWindowSize, sigma);
			}
		}

		double gaussianWeight(int x, double sigma)
		{
			double sigma_2 = sigma * sigma;
			double gaussianWeight = 1.0 / sqrt( 2.0 * M_PI * sigma_2) * pow(M_E, - x * x /( 2.0 *sigma_2) );	
			return gaussianWeight;
		}


	}; // Gaussian



	template <class T>
	class Gaussian2D{

	public:

		/**
		One step in the forward gaussian transform
		*/
		void forwardStep(const T* input, T* output, const int iwidth, const int iheight, const int HalfWindowSize, double sigma)
		{

			int size = iwidth * iheight;
			T* media = new T[size];
			double* weights = new double[HalfWindowSize + 1];
			calculateWeights(weights,HalfWindowSize, sigma);


#ifdef _COMPILE_FOR_GPU_CUDA_
			applyGaussianFilterParallel(input, output, iwidth, iheight, HalfWindowSize, weights);
			return;
#endif

			//horizontal
			for(int y = 0; y < iheight; ++y)
			{
				for(int x = 0; x < iwidth; ++x)
				{
					T sum;
					double weightSum = 0;
					for(int i = -HalfWindowSize; i <= HalfWindowSize; ++i)
					{
						int hOffset = x + i;
						if(hOffset < iwidth && hOffset >= 0)
						{
							int index = y * iwidth + hOffset;
							double weight = weights[ abs(i) ];
							sum += input[index] * weight;
							weightSum += weight;
						}
					}
					media[ y * iwidth + x] = (sum / weightSum);
				}
			}

			//vertical

			for(int y = 0; y < iheight; ++y)
			{
				for(int x = 0; x < iwidth; ++x)
				{
					T sum;
					double weightSum = 0;
					for(int i = -HalfWindowSize; i <= HalfWindowSize; ++i)
					{
						int vOffset = y + i;
						if(vOffset < iheight && vOffset >= 0)
						{
							int index = vOffset * iwidth + x;
							double weight = weights[ abs(i) ];
							sum += media[index] * weight;
							weightSum += weight;
						}
					}
					output[ y * iwidth + x] = (sum / weightSum);
				}
			}
			delete[] media;

		} // forwardStep




		//input ==output
		/**
		One step in the forward gaussian transform
		*/
		void forwardStep(T* inout,const int iwidth, const int iheight, const int HalfWindowSize, double sigma)
		{

			int size = iwidth * iheight;
			T* media = new T[size];
			double* weights = new double[HalfWindowSize + 1];
			calculateWeights(weights,HalfWindowSize, sigma);

#ifdef _COMPILE_FOR_GPU_CUDA_
			applyGaussianFilterParallel(inout, inout, iwidth, iheight, HalfWindowSize, weights);
			return;
#endif

			//horizontal
			for(int y = 0; y < iheight; ++y)
			{
				for(int x = 0; x < iwidth; ++x)
				{
					T sum;
					double weightSum = 0;
					for(int i = -HalfWindowSize; i <= HalfWindowSize; ++i)
					{
						int hOffset = x + i;
						if(hOffset < iwidth && hOffset >= 0)
						{
							int index = y * iwidth + hOffset;
							double weight = weights[ abs(i) ];
							sum += inout[index] * weight;
							weightSum += weight;
						}
					}
					media[ y * iwidth + x] = (sum / weightSum);
				}
			}

			//vertical

			for(int y = 0; y < iheight; ++y)
			{
				for(int x = 0; x < iwidth; ++x)
				{
					T sum;
					double weightSum = 0;
					for(int i = -HalfWindowSize; i <= HalfWindowSize; ++i)
					{
						int vOffset = y + i;
						if(vOffset < iheight && vOffset >= 0)
						{
							int index = vOffset * iwidth + x;
							double weight = weights[ abs(i) ];
							sum += media[index] * weight;
							weightSum += weight;
						}
					}
					inout[ y * iwidth + x] = (sum / weightSum);
				}
			}
			delete[] media;

		} // forwardStep


	protected:

		virtual void applyGaussianFilterParallel(const T* input, T* output, const int iwidth, const int iheight, const int HalfWindowSize, double* weights)
		{
#ifdef _COMPILE_FOR_GPU_CUDA_
			applyGaussianFilter<T>(input, output, iwidth, iheight, HalfWindowSize, weights);
#endif	
		}



	private:
		void calculateWeights(double* weights,const int HalfWindowSize, double sigma)
		{
			for(int i = 0; i <= HalfWindowSize; ++i)
			{
				weights[i] =  gaussianWeight(HalfWindowSize, sigma);
			}
		}

		double gaussianWeight(int x, double sigma)
		{
			double sigma_2 = sigma * sigma;
			double gaussianWeight = 1.0 / sqrt( 2.0 * M_PI * sigma_2) * pow(M_E, - x * x /( 2.0 *sigma_2) );	
			return gaussianWeight;
		}

	protected:


	}; // Gaussian2D

	typedef Gaussian<double> Gaussianf;
	typedef Gaussian2D<double> Gaussian2Df;

}