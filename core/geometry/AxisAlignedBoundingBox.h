/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file AxisAlignedBoundingBox.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include "../math/Vec3.h"

namespace Etoile
{
	template <class T>
	class AxisAlignedBoundingBox
	{
	public: 
		AxisAlignedBoundingBox(){}
		AxisAlignedBoundingBox(Vec3<T> minimum, Vec3<T> maximum){setInterval(minimum, maximum);}
		AxisAlignedBoundingBox(const std::vector<Vec3<T>>& positions)
		{
			build(positions);
		}

		void build(const std::vector<Vec3<T>>& positions)
		{
			T minimumX = 9999999999;
			T minimumY = 9999999999;
			T minimumZ = 9999999999;

			T maximumX = -9999999999;
			T maximumY = -9999999999;
			T maximumZ = -9999999999;

			for(unsigned int i = 0; i < positions.size(); i++)
			{
				const Vec3<T> position = positions[i];
				if (position[0] < minimumX)
				{
					minimumX = position[0];
				}

				if (position[0] > maximumX)
				{
					maximumX = position[0];
				}

				if (position[1] < minimumY)
				{
					minimumY = position[1];
				}

				if (position[1] > maximumY)
				{
					maximumY = position[1];
				}

				if (position[2] < minimumZ)
				{
					minimumZ = position[2];
				}

				if (position[2] > maximumZ)
				{
					maximumZ = position[2];
				}
			}
			Vec3<T> minimum(minimumX, minimumY, minimumZ);
			Vec3<T> maximum(maximumX, maximumY, maximumZ);

			if ( maximum < minimum)
			{
				_minimum = maximum;
				_maximum = minimum;
			}else
			{
				_minimum = minimum;
				_maximum = maximum;
			}
		}

		void build(const std::vector<AxisAlignedBoundingBox<T>*>& aabbs)
		{
			T minimumX = 9999999999;
			T minimumY = 9999999999;
			T minimumZ = 9999999999;

			T maximumX = -9999999999;
			T maximumY = -9999999999;
			T maximumZ = -9999999999;

			for(unsigned int i = 0; i < aabbs.size(); i++)
			{
				const Vec3<T> minimum = aabbs[i]->minimum();
				const Vec3<T> maximum = aabbs[i]->maximum();
				if (minimum[0] < minimumX)
				{
					minimumX = minimum[0];
				}

				if (maximum[0] > maximumX)
				{
					maximumX = maximum[0];
				}

				if (minimum[1] < minimumY)
				{
					minimumY = minimum[1];
				}

				if (maximum[1] > maximumY)
				{
					maximumY = maximum[1];
				}

				if (minimum[2] < minimumZ)
				{
					minimumZ = minimum[2];
				}

				if (maximum[2] > maximumZ)
				{
					maximumZ = maximum[2];
				}
			}
			Vec3<T> minimum(minimumX, minimumY, minimumZ);
			Vec3<T> maximum(maximumX, maximumY, maximumZ);

			if ( maximum < minimum)
			{
				_minimum = maximum;
				_maximum = minimum;
			}else
			{
				_minimum = minimum;
				_maximum = maximum;
			}
		}

		void setInterval(Vec3<T> minimum, Vec3<T> maximum){_minimum = minimum; _maximum = maximum;}
		bool isInclude(Vec3<T>& pos)
		{
			if(pos.x() < _maximum.x() && pos.y() < _maximum.y() && pos.z() < _maximum.z() && pos.x() > _minimum.x() && pos.y() > _minimum.y() && pos.z() > _minimum.z())
			{
				return true;
			}
			return false;
		}
		const Vec3<T> minimum()
		{
			return _minimum;
		}
		const Vec3<T> maximum()
		{
			return _maximum;
		}
		const Vec3<T> center()
		{
			return (_minimum + _maximum) * 0.5;
		}

	private:
		Vec3<T> _minimum;
		Vec3<T> _maximum;
	};

	typedef AxisAlignedBoundingBox<float> AxisAlignedBoundingBoxf;
	typedef AxisAlignedBoundingBox<double> AxisAlignedBoundingBoxd;
}