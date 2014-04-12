#pragma once
/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Signal.h
* @brief 
* @date 1/2/2011
*/
#include "ESignal.h"

namespace Etoile
{

	typedef ESignal<float> FloatSignal;
	typedef ESignal<int> IntSignal;
	typedef ESignal<double> DoubleSignal;
	typedef ESignal<bool> BoolSignal;
	typedef ESignal<std::string> StringSignal;
	
	typedef ESignalPerformer<float> FloatSignalPerformer;
	typedef ESignalPerformer<int> IntSignalPerformer;
	typedef ESignalPerformer<double> DoubleSignalPerformer;
	typedef ESignalPerformer<bool> BoolSignalPerformer;
	typedef ESignalPerformer<std::string> StringSignalPerformer;

	typedef ESignalEmitter<float> FloatSignalEmitter;
	typedef ESignalEmitter<int> IntSignalEmitter;
	typedef ESignalEmitter<double> DoubleSignalEmitter;
	typedef ESignalEmitter<bool> BoolSignalEmitter;
	typedef ESignalEmitter<std::string> StringSignalEmitter;
}