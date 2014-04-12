#pragma once

#include "Timer.h"
#include <string>
#include <map>
/*
*
*
*/

namespace Etoile
{
	class TimesCounter
	{
		TimesCounter();
		~TimesCounter();
	public:
		static TimesCounter* getInstance()
		{
			static TimesCounter tc;
			return &tc;
		}
		void reset();
		void begin(const std::string& name);
		void end(const std::string& name);

		std::map<std::string, double>const &  getTimesDifferenceMap()const
		{
			return _dif;
		}

		std::map<std::string, int>&  getIndexMap()
		{
			return _indexMap;
		}

		double getDurationTime(const std::string& name)
		{
			if(_dif.find(name) != _dif.end())
				return _dif[name];
			return 0;
		}

	private:
		std::map<std::string, double> _starts;
		std::map<std::string, double> _ends;
		std::map<std::string, double> _dif;
		std::map<std::string, int> _indexMap;
		int _index;
	};


}
