/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Assert.h
* @brief 
* @date 1/2/2011
*/

#pragma once

/**
* @original author Matthias Holländer
* The character codes in this file are Copyright (c) 2011 Matthias Holländer.
* All rights reserved.
*/
/**
* @file MxAssert.h
* @brief Contains Definitions for Assertion and Verify
* @date 17/08/2009
* @author Matthias Holländer
* 
* For now I will delegate everything to assert.h but using my macros
* This is, of course, also compiler-specific but should be separated
* from the rest to be used in all subproject
*/


// these might come in handy: __FILE__, __LINE__
// for now, delegate to the standard
// @todo: fix this:
#include <assert.h>

/*
Here are a few methods of how to get into the debugger:
memset( 0, 1, 1 );
abort();
*static_cast<char*>(0) = 0;
static_cast<void(*)()>(0)();
class LocalClass{};	throw LocalClass();
throw std::logic_error( "SomeMessage" );
//throw 0;					// method must throw()
__asm{ int 3; };
*/


/**
* @brief Helper macro for concatenating strings
*/
#define CAT2( a, b )				a##b

/**
* @brief Helper macro for concatenating strings. Use this one.
*/
#define CAT( a, b )				CAT2( a, b )




#if defined( _DEBUG )

// Debug version

#if defined( _MSC_VER )

/// Microsoft Visual C++ Compiler

#if defined( _WIN64 )

/**
* @brief Assert macro
* @param expr The expression to assert
* Win64-Bit break into debugger version
*/
#define Assert( expr )	\
	if ( !( expr ) ) { *static_cast< char* >( 0 ) = 0; }

#else
/**
* @brief Assert macro
* @param expr The expression to assert
* Similar to Game Programming Gems I: "Squeezing More Out of Assert" by Steve Rabin
* This was only tested for Microsoft Visual Studio
*/
#define Assert( expr )	\
	if ( !( expr ) ) { __asm int 3 }


#endif /// Win64/Win32

#else

/// All other

/**
* @brief Assert macro
* @param expr The expression to assert
*/
#define Assert( expr )	\
	assert( expr )

#endif // All other

/**
* @brief Verify macro
* @param expr The expression to verify
*/
#define Verify( expr )	\
	if ( ( expr ) == 0 ) {	\
	Assert( 0 && ( CAT( "Verify failed ", __FILE__ ))); \
	}


#else

// Release version

/**
* @brief Assert macro
* @param expr The expression to assert
*/
#define Assert( expr )

/**
* @brief Verify macro
* @param expr The expression to verify
*/
#define Verify( expr )			expr


#endif // _DEBUG
