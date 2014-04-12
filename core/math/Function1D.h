/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Function1D.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <math.h>

namespace Etoile
{
	class Function1D
	{
	protected:
		double _center;
	public:
		Function1D(): _center(0){}
		virtual double get(double x) = 0;
		virtual double getGradient(double x) = 0;
		void setCenter(double center){_center = center;}
		double getCenter(){return _center;}
	};

	class Linear : public Function1D
	{
	public:
		virtual double get(double x) override
		{
			return x;
		}

		virtual double getGradient(double x) override
		{
			return 1;
		}
	};

	class Sigmoid : public Function1D
	{
	public:
		virtual double get(double x) override
		{
			return 1/(1+exp(-(x - _center)));
		}

		virtual double getGradient(double x) override
		{
			return  x * ( 1 - x );
		}
	};

	class GaussianFunction1D : public Function1D
	{
	protected:
		double _sigma;
	public:
		GaussianFunction1D():Function1D(), _sigma(0){}
		virtual double get(double x) override
		{
			return exp(-pow(x - _center, 2) * 0.5 / pow(_sigma, 2));
		}
		void setSigma(double sigma){_sigma = sigma;}
		double getSigma(){return _sigma;}

		//wrong here needs to compute
		virtual double getGradient(double x) override
		{
			return  x * ( 1 - x );
		}
	};

	class EulerIntegration : public Function1D
	{
		double _lastY;
		double _h;
		double _t;
	public:
		virtual double get(double x) override
		{
			double value = _lastY + _h * f(_t, _lastY);
			_lastY = value;
			return value;
		}

		double get(double t, double x)
		{
			double value = _lastY + _h * f(_t, _lastY);
			_lastY = value;
			_t = t;
			return value;
		}

		virtual double f(double t, double y) = 0;
		virtual double getGradient(double x) override
		{
			return 1;
		}
	};

	class VerletIntegration : public Function1D
	{
		double _lastX;
		double _lastY;
		double __lastY;
		double _a;
	public:
		virtual double get(double x) override
		{
			double value = 2 * _lastY - __lastY + _a * x * x;
			__lastY = _lastY;
			_lastY = value;
			return value;
		}

		virtual double getGradient(double x) override
		{
			return 1;
		}
	};

}