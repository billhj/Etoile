#include "Timer.h"

/**
 * @brief For tracking memory leaks under windows using the crtdbg
 */
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>

	#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
	#define new DEBUG_NEW
#endif

#if defined( _MSC_VER )

	bool		TimeWin32::ms_bInitialized = false;
	__int64		TimeWin32::ms_Frequency = 1000;

#endif // _MSC_VER
