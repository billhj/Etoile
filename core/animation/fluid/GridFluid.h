/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Fluid.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include "math/MathHead.h"

namespace Etoile
{
	struct FluidUnit
	{
		FluidUnit()
		{
			_density = 0;
		}

		float _density;
		Vec3f _pos;
		Vec3f _velocity;

	};

	class GridFluid
	{
	public:
		GridFluid()
		{
		}	

		~GridFluid()
		{
			_units.clear();
		}

		void buildGrid(unsigned int x, unsigned int y, unsigned int z)
		{
			_x = x;
			_y = y;
			_z = z;
			for(unsigned int i = 0 ; i < x * y * z; ++i)
			{
				_units.push_back(FluidUnit());
				_previous_units.push_back(FluidUnit());
			}

		}

		void update()
		{
			solveVelocity();
			solveDensity();
			for(unsigned int i = 1; i < _x - 1; ++i)
			{
				for(unsigned int j = 1; j < _y - 1; ++j)
				{
					for(unsigned int k = 1; k < _z - 1; ++k)
					{

						//std::cout<<i<< " " <<j <<" " <<k <<"density" << _units[getArrayIndex(i,j,k)]._density <<" velocity "<<_units[getArrayIndex(i,j,k)]._velocity <<std::endl;
					}	
				}
			}
		}

	
		void solveVelocity()
		{
			addVelocitySource(_units, _previous_units, 0.4);
			solveVelocityDiffuse(_previous_units, _units, 0, 0.4);
			solveVelocityProject(_previous_units, _units);
			solveVelocityAdvection(_units, _previous_units, 0.4);
			solveVelocityProject(_units, _previous_units);
		}

		void solveDensity()
		{
			addDensitySource(_units, _previous_units, 0.4);
			solveDensityDiffuse(_previous_units, _units, 0, 0.4);
			solveDensityAdvection(_units, _previous_units, 0.4);
		}

		Vec3i getSize()
		{
			return Vec3i(_x,_y,_z);
		}

		FluidUnit& getFluidUnit(Vec3i id)
		{
			return _units[getArrayIndex(id.x(),id.y(),id.z())];
		}

		FluidUnit& getFluidUnit(int i, int j, int k)
		{
			return _units[getArrayIndex(i,j,k)];
		}

		void addSourceDensity(Vec3i idx, float density)
		{
			_units[getArrayIndex(idx.x(),idx.y(),idx.z())]._density = density;
			std::cout<<"addSource:"<<idx<<" "<<_units[getArrayIndex(idx.x(),idx.y(),idx.z())]._density<<std::endl;
		}

		void addForceVelocity(Vec3i idx, Vec3f velocity)
		{
			_units[getArrayIndex(idx.x(),idx.y(),idx.z())]._velocity = velocity;
			std::cout<<"addForce:"<<idx<<" "<<_units[getArrayIndex(idx.x(),idx.y(),idx.z())]._velocity<<std::endl;
		}
	private:

		
		void addVelocitySource(std::vector<FluidUnit>& source_velocity, const std::vector<FluidUnit>& newsource_velocity, float dt)
		{
			for(unsigned int i = 0; i < source_velocity.size(); ++i)
			{
				source_velocity[i]._velocity += newsource_velocity[i]._velocity * dt;
			}
		}

		void addDensitySource(std::vector<FluidUnit>& source_density, const std::vector<FluidUnit>& newsource_density, float dt)
		{
			for(unsigned int i = 0; i < source_density.size(); ++i)
			{
				source_density[i]._density += newsource_density[i]._density * dt;
			}
		}


		void solveVelocityDiffuse(std::vector<FluidUnit>& source_velocity, std::vector<FluidUnit>& previous_source_velocity, float viscousRate, float dt)
		{
			float v_accelarationWeight =  viscousRate * dt * _x * _y * _z;

			for (int l = 0 ; l < LINEARSOLVERTIMES ; l++ ) 
			{
				for(unsigned int i = 1; i < _x - 1; ++i)
				{
					for(unsigned int j = 1; j < _y - 1; ++j)
					{
						for(unsigned int k = 1; k < _z - 1; ++k)
						{
							source_velocity[getArrayIndex(i,j,k)]._velocity = (previous_source_velocity[getArrayIndex(i,j,k)]._velocity + 
								(source_velocity[getArrayIndex(i+1,j,k)]._velocity + source_velocity[getArrayIndex(i-1,j,k)]._velocity + 
								source_velocity[getArrayIndex(i,j+1,k)]._velocity + source_velocity[getArrayIndex(i,j-1,k)]._velocity +
								source_velocity[getArrayIndex(i,j,k+1)]._velocity + source_velocity[getArrayIndex(i,j,k-1)]._velocity)
								* v_accelarationWeight) / (1 + 6 * v_accelarationWeight) ;
							//std::cout<<i<< "" <<j <<"" <<k <<"previous " << _units[getArrayIndex(i,j,k)]._previous_velocity <<"current "<<_units[getArrayIndex(i,j,k)]._velocity <<std::endl;
						}	
					}
				}
			}
			//solveVelocityBoundry();
		}

		void solveDensityDiffuse(std::vector<FluidUnit>& source_density, std::vector<FluidUnit>& previous_source_density, float diffuseRate, float dt)
		{
			float d_accelarationWeight =  diffuseRate * dt * _x * _y * _z;

			for (int l = 0 ; l < LINEARSOLVERTIMES ; l++ ) 
			{
				for(unsigned int i = 1; i < _x - 1; ++i)
				{
					for(unsigned int j = 1; j < _y - 1; ++j)
					{
						for(unsigned int k = 1; k < _z - 1; ++k)
						{
							source_density[getArrayIndex(i,j,k)]._density = (previous_source_density[getArrayIndex(i,j,k)]._density + 
								(source_density[getArrayIndex(i+1,j,k)]._density + source_density[getArrayIndex(i-1,j,k)]._density + 
								source_density[getArrayIndex(i,j+1,k)]._density + source_density[getArrayIndex(i,j-1,k)]._density +
								source_density[getArrayIndex(i,j,k+1)]._density + source_density[getArrayIndex(i,j,k-1)]._density) * d_accelarationWeight) / (1 + 6 * d_accelarationWeight) ;
							//std::cout<<i<< " " <<j <<" " <<k <<"previous" << _units[getArrayIndex(i,j,k)]._previous_velocity <<"current"<<_units[getArrayIndex(i,j,k)]._velocity <<std::endl;
						}	
					}
				}
			}
			//solveDensityBoundry();
		}

	

		void solveVelocityAdvection(std::vector<FluidUnit>& source_velocity, std::vector<FluidUnit>& previous_source_velocity, float dt)
		{

			float dtx,dty,dtz;
			dtx = (_x - 2) * dt;
			dty = (_y - 2) * dt;
			dtz = (_z - 2) * dt;
			for(unsigned int i = 1; i < _x - 2; ++i)
			{
				for(unsigned int j = 1; j < _y - 2; ++j)
				{
					for(unsigned int k = 1; k < _z - 2; ++k)
					{
						{
							int i0, j0, k0, i1, j1, k1;
							float s0, t0, s1, t1, u1, u0;
							float x = i - dtx * previous_source_velocity[getArrayIndex(i,j,k)]._velocity.x(); 
							float y = j - dty * previous_source_velocity[getArrayIndex(i,j,k)]._velocity.y(); 
							float z = k - dtz * previous_source_velocity[getArrayIndex(i,j,k)]._velocity.z();

							if (x<0.5f) x=0.5f; if (x>_x-2+0.5f) x=_x-2+0.5f; i0=(int)x; i1=i0+1;
							if (y<0.5f) y=0.5f; if (y>_y-2+0.5f) y=_y-2+0.5f; j0=(int)y; j1=j0+1;
							if (z<0.5f) z=0.5f; if (z>_z-2+0.5f) z=_z-2+0.5f; k0=(int)z; k1=k0+1;

							s1 = x-i0; s0 = 1-s1; t1 = y-j0; t0 = 1-t1; u1 = z-k0; u0 = 1-u1;
							//bilinear interpolation
							source_velocity[getArrayIndex(i,j,k)]._velocity = ( previous_source_velocity[getArrayIndex(i0,j0,k0)]._velocity * t0 * u0 + previous_source_velocity[getArrayIndex(i0,j1,k0)]._velocity * t1 * u0 + 
								previous_source_velocity[getArrayIndex(i0,j0,k1)]._velocity * t0 * u1 + previous_source_velocity[getArrayIndex(i0,j1,k1)]._velocity * t1 * u1 ) * s0 +
								( previous_source_velocity[getArrayIndex(i1,j0,k0)]._velocity * t0 * u0 + previous_source_velocity[getArrayIndex(i1,j1,k0)]._velocity * t1 * u0 + 
								previous_source_velocity[getArrayIndex(i1,j0,k1)]._velocity * t0 * u1 + previous_source_velocity[getArrayIndex(i1,j1,k1)]._velocity * t1 * u1 )* s1 ;
						}

					}	
				}
			}
			//solveVelocityBoundry();
		}

		void solveDensityAdvection(std::vector<FluidUnit>& source_density, std::vector<FluidUnit>& previous_source_density, float dt)
		{

			float dtx,dty,dtz;
			dtx = (_x - 2) * dt;
			dty = (_y - 2) * dt;
			dtz = (_z - 2) * dt;
			for(unsigned int i = 1; i < _x - 2; ++i)
			{
				for(unsigned int j = 1; j < _y - 2; ++j)
				{
					for(unsigned int k = 1; k < _z - 2; ++k)
					{
						{
							int i0, j0, k0, i1, j1, k1;
							float s0, t0, s1, t1, u1, u0;
							float x = i - dtx * source_density[getArrayIndex(i,j,k)]._velocity.x(); 
							float y = j - dty * source_density[getArrayIndex(i,j,k)]._velocity.y(); 
							float z = k - dtz * source_density[getArrayIndex(i,j,k)]._velocity.z();

							if (x<0.5f) x=0.5f; if (x>_x-2+0.5f) x=_x-2+0.5f; i0=(int)x; i1=i0+1;
							if (y<0.5f) y=0.5f; if (y>_y-2+0.5f) y=_y-2+0.5f; j0=(int)y; j1=j0+1;
							if (z<0.5f) z=0.5f; if (z>_z-2+0.5f) z=_z-2+0.5f; k0=(int)z; k1=k0+1;

							s1 = x-i0; s0 = 1-s1; t1 = y-j0; t0 = 1-t1; u1 = z-k0; u0 = 1-u1;
							//bilinear interpolation
							source_density[getArrayIndex(i,j,k)]._density = s0 * ( t0 * u0 * previous_source_density[getArrayIndex(i0,j0,k0)]._density + t1 * u0 * previous_source_density[getArrayIndex(i0,j1,k0)]._density + 
								t0 * u1 * previous_source_density[getArrayIndex(i0,j0,k1)]._density + t1 * u1 * previous_source_density[getArrayIndex(i0,j1,k1)]._density ) +
								s1 * ( t0 * u0 * previous_source_density[getArrayIndex(i1,j0,k0)]._density + t1 * u0 * previous_source_density[getArrayIndex(i1,j1,k0)]._density + 
								t0 * u1 * previous_source_density[getArrayIndex(i1,j0,k1)]._density + t1 * u1 * previous_source_density[getArrayIndex(i1,j1,k1)]._density );
							//std::cout<< i<<" " <<j<<" "<< k <<" "<<_units[getArrayIndex(i,j,k)]._density <<std::endl;
						}

					}	
				}
			}
			//solveDensityBoundry();
		}



		void solveVelocityBoundry()
		{
			for(unsigned int i = 0; i < _x; ++i)
			{
				for(unsigned int j = 0; j < _y; ++j)
				{
					for(unsigned int k = 0; k < _z; ++k)
					{
						if(i == 0 || j == 0 || k == 0)
						{
							_units[getArrayIndex(i,j,k)]._velocity = Vec3f();
						}
					}	
				}
			}
		}

		void solveDensityBoundry()
		{
			for(unsigned int i = 0; i < _x; ++i)
			{
				for(unsigned int j = 0; j < _y; ++j)
				{
					for(unsigned int k = 0; k < _z; ++k)
					{
						if(i == 0 || j == 0 || k == 0)
						{
							_units[getArrayIndex(i,j,k)]._density = 0;
						}
					}	
				}
			}
		}

		void solveVelocityProject(std::vector<FluidUnit>& source_velocity, std::vector<FluidUnit>& previous_source_velocity)
		{
			int size = _x * _y * _z;
			for(unsigned int i = 1; i < _x - 1; ++i)
			{
				for(unsigned int j = 1; j < _y - 1; ++j)
				{
					for(unsigned int k = 1; k < _z - 1; ++k)
					{
						previous_source_velocity[getArrayIndex(i,j,k)]._velocity[1] = ( (source_velocity[getArrayIndex(i+1,j,k)]._velocity.x() - source_velocity[getArrayIndex(i-1,j,k)]._velocity.x())/(float)_x +
							(source_velocity[getArrayIndex(i,j+1,k)]._velocity.y() - source_velocity[getArrayIndex(i,j-1,k)]._velocity.y())/(float)_y +
							(source_velocity[getArrayIndex(i,j,k+1)]._velocity.z() - source_velocity[getArrayIndex(i,j,k-1)]._velocity.z())/(float)_z
							) * (-1.0/3.0);
						previous_source_velocity[getArrayIndex(i,j,k)]._velocity[0] = 0;
						//std::cout<<"project"<<i<< " " <<j <<" " <<k <<"previous" << _units[getArrayIndex(i,j,k)]._velocity <<std::endl;
					}
				}
			}

			for(int loop = 0; loop < 20; ++loop){
				for(unsigned int i = 1; i < _x - 1; ++i)
				{
					for(unsigned int j = 1; j < _y - 1; ++j)
					{
						for(unsigned int k = 1; k < _z - 1; ++k)
						{
							previous_source_velocity[getArrayIndex(i,j,k)]._velocity[0] = (previous_source_velocity[getArrayIndex(i,j,k)]._velocity[1]+ 
								(previous_source_velocity[getArrayIndex(i+1,j,k)]._velocity[0] + previous_source_velocity[getArrayIndex(i-1,j,k)]._velocity[0] + 
								previous_source_velocity[getArrayIndex(i,j+1,k)]._velocity[0] + previous_source_velocity[getArrayIndex(i,j-1,k)]._velocity[0] +
								previous_source_velocity[getArrayIndex(i,j,k+1)]._velocity[0] + previous_source_velocity[getArrayIndex(i,j,k-1)]._velocity[0])) / 6.0 ;
							//std::cout<<"project"<<i<< " " <<j <<" " <<k <<"previous" << _units[getArrayIndex(i,j,k)]._velocity <<std::endl;
						}	
					}
				}
			}

			for(unsigned int i = 1; i < _x - 1; ++i)
			{
				for(unsigned int j = 1; j < _y - 1; ++j)
				{
					for(unsigned int k = 1; k < _z - 1; ++k)
					{
						source_velocity[getArrayIndex(i,j,k)]._velocity -= ( Vec3f(
							(previous_source_velocity[getArrayIndex(i+1,j,k)]._velocity[0] - previous_source_velocity[getArrayIndex(i-1,j,k)]._velocity[0]) * (_x - 2),
							(previous_source_velocity[getArrayIndex(i,j+1,k)]._velocity[0] - previous_source_velocity[getArrayIndex(i,j-1,k)]._velocity[0]) * (_y - 2),
							(previous_source_velocity[getArrayIndex(i,j,k+1)]._velocity[0] - previous_source_velocity[getArrayIndex(i,j,k-1)]._velocity[0]) * (_z - 2)
							) * 0.5 );
						//std::cout<<"project"<<i<< " " <<j <<" " <<k <<"previous" << _units[getArrayIndex(i,j,k)]._previous_velocity <<"current"<<_units[getArrayIndex(i,j,k)]._velocity <<std::endl;
						//std::cout<<i<< "" <<j <<"" <<k <<"previous " << _units[getArrayIndex(i,j,k)]._previous_velocity <<"current "<<_units[getArrayIndex(i,j,k)]._velocity <<std::endl;
					}
				}
			}

		}

	private:
		int getArrayIndex(int x, int y, int z)
		{
			return x * _x * _y + y * y + z;
		}
	private:
		std::vector<FluidUnit> _units;
		std::vector<FluidUnit> _previous_units;
		unsigned int _x, _y, _z;
		const static int LINEARSOLVERTIMES = 20;
	};

}
