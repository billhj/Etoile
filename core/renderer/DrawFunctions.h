#pragma once
/**
*/

#include "math/MathHead.h"

namespace Etoile
{
	class DrawFunctions
	{
	public:
		//counterclockwise
		Vec3f compute_normal(Vec3f p1, Vec3f p2, Vec3f p3)
		{
			Vec3f normal = (p2 - p1).cross3(p3 - p1);
			normal.normalize();
			return normal;
		}
		virtual void drawBone(Vec3f posParent, Vec3f pos, Vec3f originalDirection, Quaternionf orientation) = 0;

	};

}


