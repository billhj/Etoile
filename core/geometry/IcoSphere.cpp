

#include "icosphere.h"

#include <map>

//--------------------------------------------------------------------------------
// icosahedron data
//--------------------------------------------------------------------------------
namespace Etoile
{
#define X .525731112119133606
#define Z .850650808352039932

	static float vdata[12][3] = {
		{-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
		{0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
		{Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
	};

	static int tindices[20][3] = {
		{0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
		{8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
		{7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
		{6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} };
		//--------------------------------------------------------------------------------

		IcoSphere::IcoSphere(unsigned int levels) : Primitive() , _level(levels)
		{
			init();
		}

		void IcoSphere::init()
		{
			// init with an icosahedron
			for (int i = 0; i < 12; i++)
				_vdata.push_back(Vec3f(vdata[i][0],vdata[i][1],vdata[i][2]));
			_indices_tmp.push_back(std::vector<int>());
			std::vector<int>& indices = _indices_tmp.back();
			for (int i = 0; i < 20; i++)
			{
				for (int k = 0; k < 3; k++)
					indices.push_back(tindices[i][k]);
			}
			_listIds.push_back(0);

			while(_indices_tmp.size()<_level)
				subdivide();
			for(int i = 0; i < _indices_tmp.size(); ++i)
			{
				std::vector<int>& indxs = _indices_tmp[i];
				for(int j = 0; j < indxs.size(); ++j)
				{
					_vertices_index_face.push_back((indxs)[j]);
				}
			}
		}
		void IcoSphere::subdivide(void)
		{
			typedef unsigned long long Key;
			std::map<Key,int> edgeMap;
			const std::vector<int>& indices = _indices_tmp.back();
			_indices_tmp.push_back(std::vector<int>());
			std::vector<int>& refinedIndices = _indices_tmp.back();
			int end = indices.size();
			for (int i=0; i<end; i+=3)
			{
				int ids0[3],  // indices of outer vertices
					ids1[3];  // indices of edge vertices
				for (int k=0; k<3; ++k)
				{
					int k1 = (k+1)%3;
					int e0 = indices[i+k];
					int e1 = indices[i+k1];
					ids0[k] = e0;
					if (e1>e0)
						std::swap(e0,e1);
					Key edgeKey = Key(e0) | (Key(e1)<<32);
					std::map<Key,int>::iterator it = edgeMap.find(edgeKey);
					if (it==edgeMap.end())
					{
						ids1[k] = _vdata.size();
						edgeMap[edgeKey] = ids1[k];
						_vdata.push_back( (_vdata[e0]+_vdata[e1]).normalized() );
					}
					else
						ids1[k] = it->second;
				}
				refinedIndices.push_back(ids0[0]); refinedIndices.push_back(ids1[0]); refinedIndices.push_back(ids1[2]);
				refinedIndices.push_back(ids0[1]); refinedIndices.push_back(ids1[1]); refinedIndices.push_back(ids1[0]);
				refinedIndices.push_back(ids0[2]); refinedIndices.push_back(ids1[2]); refinedIndices.push_back(ids1[1]);
				refinedIndices.push_back(ids1[0]); refinedIndices.push_back(ids1[1]); refinedIndices.push_back(ids1[2]);
			}
			_listIds.push_back(0);
		}

}

