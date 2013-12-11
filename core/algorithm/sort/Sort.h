/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Sort.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>

namespace Etoile
{
	template <class T> class QuickSort;
	template <class T> class MergeSort;
	template <class T> class BubbleSort;

	template <class T>
	class Sort
	{
	public:
		virtual std::vector<T> sort(std::vector<T>& list) = 0;
	};

	/*
	*	QuickSort: each iteration (compare all element in the list, separate into before and after two list, pivot is in the middle) can help pivot find the final position
	*	recusivelly find each position for each element, complexity nlog_2 n < c < n^2 * 0.5
	**/
	template <class T>
	class QuickSort : public Sort<T>
	{
	public:
		virtual std::vector<T> sort(std::vector<T>& list) override
		{
			if(list.size() <= 1)
			{
				return list;
			}
			std::vector<T> retureValueBefore;
			std::vector<T> retureValueAfter;
			T pivot = list[0];
			for(unsigned int i = 1; i < list.size(); ++i)
			{
				if(list[i] < pivot)
				{
					retureValueBefore.push_back(list[i]);
				}
				else
				{
					retureValueAfter.push_back(list[i]);
				}
			}
			return concatenate(sort(retureValueBefore), pivot, sort(retureValueAfter));
		}
	protected:
		//concatenate 2 sorted lists with pivot
		std::vector<T> concatenate(std::vector<T>& list1, T& pivot, std::vector<T>& list2)
		{
			std::vector<T> retureValue;
			for(unsigned int i = 0; i < list1.size(); ++i)
			{
				retureValue.push_back(list1[i]);
			}
			retureValue.push_back(pivot);
			for(unsigned int i = 0; i < list2.size(); ++i)
			{
				retureValue.push_back(list2[i]);
			}
			return retureValue;
		}
	};


	/*
	*	MergeSort: each iteration (separate into before and after two list), merge process is used to reposition the list
	*	recusivelly find each position for each element, complexity less nlog_2 n,  each merge max complexity = l1 + l2 - 1, (each level complexity = n, levels = log_2 n)
	**/
	template <class T>
	class MergeSort : public Sort<T>
	{
	public:
		virtual std::vector<T> sort(std::vector<T>& list) override
		{
			if(list.size() <= 1)
			{
				return list;
			}
			std::vector<T> retureValueBefore;
			std::vector<T> retureValueAfter;
			unsigned int middle= list.size() / 2;
			for(unsigned int i = 0; i < middle; ++i)
			{	
				retureValueBefore.push_back(list[i]);
			}
			for(unsigned int i = middle; i < list.size(); ++i)
			{
				retureValueAfter.push_back(list[i]);
			}

			retureValueBefore = sort(retureValueBefore);
			retureValueAfter = sort(retureValueAfter);
			return merge(retureValueBefore, retureValueAfter);
		}

	protected:
		//merge part does the ordering
		std::vector<T> merge(std::vector<T>& list1, std::vector<T>& list2)
		{
			std::vector<T> retureValue;
			while(list1.size() > 0 || list2.size() > 0)
			{
				if(list1.size() > 0 && list2.size() > 0)
				{
					if(list1[0] < list2[0])
					{
						retureValue.push_back(list1[0]);
						list1.erase(list1.begin());
					}
					else
					{
						retureValue.push_back(list2[0]);
						list2.erase(list2.begin());
					}
				}
				else if(list1.size() > 0)
				{
					retureValue.push_back(list1[0]);
					list1.erase(list1.begin());
				}
				else if(list2.size() > 0)
				{
					retureValue.push_back(list2[0]);
					list2.erase(list2.begin());
				}
			}
			return retureValue;
		}
	};


	/*
	*	BubbleSort: each iteration: compare n and n + 1, if( e(n) < e(n+1)) change place, then do n+1 and n+2, make the biggest into the end of the list
	*	advantge: no more memory takes, only do exchanges, complexity n^2
	**/
	template <class T>
	class BubbleSort : public Sort<T>
	{
	public:
		virtual std::vector<T> sort(std::vector<T>& list) override
		{
			std::vector<T> retureValue = list;
			int n = retureValue.size();
			while(n > 1)
			{
				for(unsigned int i = 0; i < n - 1; ++i)
				{
					if(retureValue[i] > retureValue[i+1])
					{
						T tmp = retureValue[i];
						retureValue[i] = retureValue[i+1];
						retureValue[i+1] = tmp;
					}
				}
				--n;
			}
			
			return retureValue;
		}
	};
}