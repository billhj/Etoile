/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ShaderSocket.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "TemplateSocket.h"
#include "module/signals/ShaderSignal.h"

namespace Etoile
{
	typedef TemplateInputSocket<ShaderObject> ShaderInputSocket;

	typedef TemplateOutputSocket<ShaderObject> ShaderOutputSocket;
}