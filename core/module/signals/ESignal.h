/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ESignal.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>

namespace Etoile
{
	template<class T>
	class ESignal
	{
	protected:
		T* _pt;
	public:
		ESignal()
		{
			_pt = NULL;
		}

		ESignal(T* t)
		{
			_pt = t;
		}

		void set(T* t)
		{
			_pt = t;
		}

		T* get(){return _pt;}
	};

	template<class T>
	class ESignalPerformer
	{
	public:
		virtual void perform(T* signal) = 0;
	};

	template<class T>
	class ESignalEmitter
	{
	protected:
		std::vector<ESignalPerformer<T>*> _performers;
	public:
		ESignalEmitter()
		{
		}

		std::vector<ESignalPerformer<T>*>& getPerformers(){return _performers;}
		void addSignalPerformer(ESignalPerformer<T>* performer)
		{
			if(!isESignalPerformer(performer))
				_performers.push_back(performer);
		}

		bool isESignalPerformer(ESignalPerformer<T>* performer)
		{
			for(unsigned int i = 0; i < _performers.size(); ++i)
			{
				ESignalPerformer<T>* p = _performers[i];
				if(p == performer)
				{
					return true;
				}
			}
			return false;
		}

		void removeSignalPerformer(ESignalPerformer<T>* performer)
		{
			for(unsigned int i = 0; i < _performers.size(); ++i)
			{
				ESignalPerformer<T>* p = _performers[i];
				if(p == performer)
				{
					_performers.erase(_performers.begin() + i);
				}
			}
		}

		void clearSignalPerformers()
		{
			_performers.clear();
		}

		virtual void signalEmit(T* signal)
		{
			for(unsigned int i = 0; i < _performers.size(); ++i)
			{
				ESignalPerformer<T>* p = _performers[i];
				if(p != NULL)
				{
					p->perform(signal);
				}
			}
		}
	};
}