#include "TimesCounter.h"
#include "../RTShaderSystem/GLBase/glhead.h"

/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

namespace Etoile
{
	TimesCounter::TimesCounter()
	{

	}

	TimesCounter::~TimesCounter()
	{

	}

	void TimesCounter::reset()
	{
		_index = 0;
		_starts.clear();
		_ends.clear();
		_dif.clear();
		_indexMap.clear();
	}

	void TimesCounter::begin(const std::string& name)
	{
		glFinish();
		Time start;
		_starts[name] = start.getCurrentTime();
		_indexMap[name] = _index;
		_index++;
	}

	void TimesCounter::end(const std::string& name)
	{
		glFinish();
		Time end;
		_ends[name] = end.getCurrentTime();
		double differenceTime = end.DiffTime(_starts[name]);
		_dif[name] = differenceTime;
	}
}
