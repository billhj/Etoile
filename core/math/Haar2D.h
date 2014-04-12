/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Haar2D.h
* @brief 
* @date 1/2/2011
*/

#pragma once

/**
Haar (flat line) wavelet.

As with all Lifting scheme wavelet transform functions, the
first stage of a transform step is the split stage.  The
split step moves the even element to the first half of an
N element region and the odd elements to the second half of the N
element region.

The Lifting Scheme version of the Haar transform uses a wavelet
function (predict stage) that "predicts" that an odd element will
have the same value as it preceeding even element.  Stated another
way, the odd element is "predicted" to be on a flat (zero slope
line) shared with the even point.  The difference between this
"prediction" and the actual odd value replaces the odd element.

The wavelet scaling function (a.k.a. smoothing function) used
in the update stage calculates the average between an even and
an odd element.

The merge stage at the end of the inverse transform interleaves
odd and even elements from the two halves of the array
(e.g., ordering them even<sub>0</sub>, odd<sub>0</sub>,
even<sub>1</sub>, odd<sub>1</sub>, ...)

This is a template version of the Haar wavelet.  The template must
be instantiated with an array or an object that acts like an array.
Objects that act like arrays define the left hand side and right
hand side index operators: [].


*/


#include <math.h>
#include "LiftBase.h"

namespace Etoile
{
	template <class T>
	class Haar2D
	{

	public:
		Haar2D():_threshold(0){}
		void setThreshold(double threshold)
		{
			_threshold = threshold;
		}

		void thresholdFunction(T* output, const int iwidth, const int iheight)
		{
			int count = 0;
			for(int index = 1; index < iheight * iwidth; ++index)
			{
				double len = output[index].length();
				if( len < _threshold)
				{
					output[index] = Vec4f(0);
					count++;
				}
			}
			std::cout<< "count: " << count<<std::endl;
			std::cout<<"compressing ratio: " <<count/ double(iheight * iwidth)<<std::endl;
		}
		/**
		One step in the forward wavelet transform, with normalization
		*/
		void forwardStep(const T* input, T* output, const int iwidth, const int iheight, const int iterations)
		{
			int size = iwidth * iheight;
			T* media = new T[size];

			int width = iwidth;
			int height = iheight;

			memcpy(output, input, size * sizeof(*input));

			for(int i = 0; i < iterations; ++i)
			{
				//memcpy(media, output, size * sizeof(output));
				//horizontal
				width = iwidth >> i;
				height = iheight >> i;

				int hOffset = width >> 1;

				for(int y = 0; y < height; ++y)
				{
					int sIndex, tIndex;
					tIndex = sIndex = y * iwidth;
					for(int x = 0; x < width; x += 2)
					{
						T a = output[sIndex];
						T b = output[sIndex + 1];
						media[tIndex] = (a + b) * 0.5;
						media[tIndex + hOffset] = a - b;
						sIndex += 2;
						tIndex ++;
					}
				}
				//vertical
				int vOffset = (height >> 1) * iwidth;

				for (int x = 0; x < width; x++) 
				{
					int sIndex, tIndex;
					sIndex = tIndex = x;
					for (int y = 0; y < height; y += 2) 
					{
						T a = media[sIndex];
						T b = media[sIndex + iwidth];
						output[tIndex] = (a + b) * 0.5;
						output[tIndex + vOffset] = a - b;
						sIndex += iwidth << 1;
						tIndex += iwidth;
					}
				}
				width = width << 1;
				height = height << 1;

			}

			delete[] media;

			thresholdFunction(output, iwidth, iheight);

		} // forwardStep


		/**
		One inverse wavelet transform step, with normalization
		*/
		void inverseStep(const T* input, T* output, const int iwidth, const int iheight, const int iterations)
		{

			int size = iwidth * iheight;
			T* media = new T[size];


			int width = iwidth;
			int height = iheight;
			memcpy(output, input, size * sizeof(input));

			for(int i = iterations - 1; i >=0; --i)
			{
				//memcpy(media, output, size * sizeof(output));
				width = iwidth >> i;
				height = iheight >> i;

				//vertical processing
				int vOffset = (height >> 1) * iwidth;
				for (int x = 0; x < width; x++) 
				{
					int sIndex, tIndex;
					sIndex = tIndex = x;
					for (int y = 0; y < height; y += 2) 
					{
						T a = output[sIndex];
						T b = output[sIndex + vOffset];
						media[tIndex] = a + b * 0.5;
						media[tIndex + iwidth] = a - b * 0.5;
						sIndex += iwidth;
						tIndex += iwidth << 1;
					}
				}
				//horizontal processing

				int hOffset = width >> 1;
				for (int y = 0; y < height; y++) 
				{
					int sIndex, tIndex;
					tIndex = sIndex = y * iwidth;
					for (int x = 0; x < width; x += 2) 
					{
						T a = media[sIndex];
						T b = media[sIndex + hOffset];
						output[tIndex] = a + b * 0.5;
						output[tIndex + 1] = a - b * 0.5;
						sIndex ++;
						tIndex += 2;
					}
				}

				width = width >> 1;
				height = height >> 1;
			}
			delete[] media;
		}  // inverseStep







		//input == output
		/**
		One step in the forward wavelet transform, with normalization
		*/
		void forwardStep(T* inout,const int iwidth, const int iheight, const int iterations)
		{
			int size = iwidth * iheight;
			T* media = new T[size];

			int width = iwidth;
			int height = iheight;


			for(int i = 0; i < iterations; ++i)
			{
				//memcpy(media, output, size * sizeof(output));
				//horizontal
				width = iwidth >> i;
				height = iheight >> i;

				int hOffset = width >> 1;

				for(int y = 0; y < height; ++y)
				{
					int sIndex, tIndex;
					tIndex = sIndex = y * iwidth;
					for(int x = 0; x < width; x += 2)
					{
						T a = inout[sIndex];
						T b = inout[sIndex + 1];
						media[tIndex] = (a + b) * 0.5;
						media[tIndex + hOffset] = a - b;
						sIndex += 2;
						tIndex ++;
					}
				}
				//vertical
				int vOffset = (height >> 1) * iwidth;

				for (int x = 0; x < width; x++) 
				{
					int sIndex, tIndex;
					sIndex = tIndex = x;
					for (int y = 0; y < height; y += 2) 
					{
						T a = media[sIndex];
						T b = media[sIndex + iwidth];
						inout[tIndex] = (a + b) * 0.5;
						inout[tIndex + vOffset] = a - b;
						sIndex += iwidth << 1;
						tIndex += iwidth;
					}
				}
				width = width << 1;
				height = height << 1;

			}

			delete[] media;

		} // forwardStep


		/**
		One inverse wavelet transform step, with normalization
		*/
		void inverseStep(T* inout, const int iwidth, const int iheight, const int iterations)
		{

			int size = iwidth * iheight;
			T* media = new T[size];


			int width = iwidth;
			int height = iheight;

			for(int i = iterations - 1; i >=0; --i)
			{
				//memcpy(media, output, size * sizeof(output));
				width = iwidth >> i;
				height = iheight >> i;

				//vertical processing
				int vOffset = (height >> 1) * iwidth;
				for (int x = 0; x < width; x++) 
				{
					int sIndex, tIndex;
					sIndex = tIndex = x;
					for (int y = 0; y < height; y += 2) 
					{
						T a = inout[sIndex];
						T b = inout[sIndex + vOffset];
						media[tIndex] = a + b * 0.5;
						media[tIndex + iwidth] = a - b * 0.5;
						sIndex += iwidth;
						tIndex += iwidth << 1;
					}
				}
				//horizontal processing

				int hOffset = width >> 1;
				for (int y = 0; y < height; y++) 
				{
					int sIndex, tIndex;
					tIndex = sIndex = y * iwidth;
					for (int x = 0; x < width; x += 2) 
					{
						T a = media[sIndex];
						T b = media[sIndex + hOffset];
						inout[tIndex] = a + b * 0.5;
						inout[tIndex + 1] = a - b * 0.5;
						sIndex ++;
						tIndex += 2;
					}
				}

				width = width >> 1;
				height = height >> 1;
			}
			delete[] media;
		}  // inverseStep

	protected:
		double _threshold;
	}; // haar2D
}