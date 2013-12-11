#pragma once
/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MeshSignal.h
* @brief 
* @date 1/2/2011
*/
#include "ESignal.h"
#include "geometry/Mesh.h"

namespace Etoile
{
	typedef ESignal<Mesh> MeshSignal;
	typedef ESignalPerformer<Mesh> MeshSignalPerformer;
	typedef ESignalEmitter<Mesh> MeshSignalEmitter;
	/*class MeshSignal : public ESignal<Mesh>
	{
	public:
		MeshSignal(void);
		virtual ~MeshSignal(void);
	};

	class MeshSignalPerformer : public ESignalPerformer<Mesh>
	{
	public:
		MeshSignalPerformer(void){}
		virtual ~MeshSignalPerformer(void){}
	};

	class MeshSignalEmitter : public ESignalEmitter<Mesh>
	{
	public:
		MeshSignalEmitter(void){}
		virtual ~MeshSignalEmitter(void){}
	};*/

}