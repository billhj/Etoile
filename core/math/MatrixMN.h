/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MatrixMN.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "VecN.h"
//#include "jama/jama_svd.h"
#include <limits>
#include <iomanip>

namespace Etoile
{
	template <class T>
	class MatrixMN
	{
	public:
		MatrixMN() : rows(0), columns(0), elements(NULL)
		{

		}

		MatrixMN(unsigned int rows, unsigned int columns) : elements(NULL)
		{
			setSize(rows, columns);
			makeZero();
		}

		MatrixMN(unsigned int rows, unsigned int columns, T* values) : elements(NULL)
		{
			setSize(rows, columns);
			makeZero();
			setElements(rows, columns, values);
		}

		MatrixMN(const MatrixMN& m)
		{
			elements = NULL;
			setSize(m.rows, m.columns);
			setElements(m.rows, m.columns, m.elements);
		}

		~MatrixMN()
		{
			if(elements != NULL)
			{
				delete []elements;
			}

			elements = NULL;
		}

		MatrixMN& operator=(const MatrixMN& m)
		{
			if(elements)
			{
				delete []elements;
			}
			setSize(m.rows, m.columns);
			setElements(m.rows, m.columns, m.elements);
			return *this;
		}

		void setSize(unsigned int rows, unsigned int columns)
		{
			this->rows  = rows;
			this->columns = columns;

			if(elements)
			{
				delete []elements;
			}

			elements = new T[rows * columns];
		}

		void setElements(unsigned int rows, unsigned int columns, T* values)
		{
			assert(rows <= this->rows && columns <= this->columns);
			memcpy(elements, values, rows * columns * sizeof(T));
		}

		unsigned int getNoOfRows() const
		{
			return rows;
		}

		unsigned int getNoOfColumns() const
		{
			return columns;
		}

		T& operator()(unsigned int row, unsigned int column)
		{
			assert(row < rows && column < columns);
			//return *(elements + column * rows + row);
			return *(elements + column + row * columns);
		}

		T  operator()(unsigned int row, unsigned int column) const
		{
			assert(row < rows && column < columns);
			//return *(elements + column * rows + row);
			return *(elements + column + row * columns);
		}

		void setDiagonalEntries(T value)
		{
			for(unsigned int i = 0; i < rows; i++)
			{
				operator()(i, i) = value;
			}
		}

		void setDiagonalEntries(const VecN<T>& v)
		{
			assert(v.getSize() >= rows);
			for(unsigned int i = 0; i < rows; i++)
			{
				operator()(i, i) = v[i];
			}
		}

		void addToDiagonal(T value)
		{
			for(unsigned int i = 0; i < rows; i++)
			{
				operator()(i, i) += value;
			}
		}

		void setColumn(unsigned int column, const VecN<T>& v)
		{
			assert(column < columns && v.getSize() >= rows);
			//T* to = elements + column * rows;
			for(unsigned int i = 0; i < rows; i++)
			{
				operator()(i, column) = v[i];
				//*(to + i) = v[i];
			}
		}

		void setRow(unsigned int row, const VecN<T>& v)
		{
			assert(row < columns && v.getSize() >= columns);

			//T *to = elements + row;
			for(unsigned int i = 0; i < columns; i++)
			{
				//*(to) = v[i];
				//to += rows;
				operator()(row, i) = v[i];
			}
		}

		void makeIdentity()
		{
			setDiagonalEntries(1);
		}

		void makeZero()
		{
			for(unsigned int i = 0; i < rows; i++)
			{
				for(unsigned int j = 0; j < columns; j++)
				{
					operator()(i, j) = 0.0;
				}
			}
		}

		MatrixMN transpose() const
		{
			MatrixMN t(columns, rows);
			for(unsigned int i = 0; i < rows; i++)
			{
				for(unsigned int j = 0; j < columns; j++)
				{
					t(j, i) = operator()(i, j);
				}
			}

			return t;
		}
		MatrixMN inverse() const
		{
			//assert non singular
			assert(columns == rows);
			//using the LU decomposition algorithm
			//A*inv(A) = I -> LUinv(A) = I
			//L(U*inv(A)) = I , Y = U*inv(A)
			//LY = I -> we compute Y
			//and from Y we compute inv(A)

			MatrixMN L(rows, columns);
			MatrixMN U(rows, columns);
			MatrixMN inverse(rows, columns);
			MatrixMN I(rows, columns);
			I.makeIdentity();

			LUDecomposition(L, U);

			for(unsigned int i = 0; i < rows; i++)
			{
				VecN<T> y(rows);
				for(unsigned int j = 0; j < rows; j++)
				{
					T sum = 0;
					for(unsigned int k = 0; k < j; k++)
					{
						sum += L(j, k) * y[k];
					}

					y[j] = I(j, i) - sum;
				}

				for(int j = rows - 1; j >= 0; j--)
				{
					T sum = 0;
					for(unsigned int k = j + 1; k < rows; k++)
					{
						sum += U(j, k) * inverse(k, i);
					}

					inverse(j, i) = (y[j] - sum) / U(i, j);
				}
			}

			return inverse;
		}

		MatrixMN pseudoInverse() const
		{
			MatrixMN U, V, UT;
			VecN<T> W;

			unsigned int m = rows, n = columns;

			U.setSize(m, n);
			U.makeZero();
			W.setSize(n);
			W.makeZero();
			V.setSize(n, n);
			V.makeZero();

			SVD(U, W, V);

			UT = U.transpose();

			for(unsigned int i = 0; i < n; i++)
			{
				if(W[i] > std::numeric_limits<double>::epsilon())
				{
					W[i] = 1 / W[i];
				}
				else
				{
					W[i] = 0;
				}
			}

			MatrixMN ps(n, m);

			for(unsigned int  i = 0; i < n; i++)
			{
				for(unsigned int j = 0; j < m; j++)
				{
					for(unsigned int k = 0; k < n; k++)
					{
						ps(i, j) += V(i, k) * UT(k, j) * W[k];
					}
				}
			}

			return ps;
		}

		void forwardSubstitution(VecN<T>& x, const VecN<T>& b)
		{
			x[0] = b[0];
			for(unsigned int i = 1; i < rows; i++)
			{
				T sum = 0;
				for(unsigned int j = 0; j < i; j++)
				{
					sum += x[j] * operator()(i, j);
				}

				x[i] = b[i] - sum;
			}
		}
		void backwardSubstitution(VecN<T>& x, const VecN<T>& b)
		{
			x[rows - 1] = b[rows - 1] / operator()(rows - 1, rows - 1);

			for(int i = rows - 2; i >= 0; i--)
			{
				T sum = 0;
				for(unsigned int j = i; j < rows; j++)
				{
					sum += x[j] * operator()(i, j);
				}

				x[i] = (b[i]  - sum) / operator()(i, i);
			}

		}

		void solveSystem(VecN<T>& x, const VecN<T>& b)
		{
			MatrixMN L(rows, columns), U(rows, columns);
			LUDecomposition(L, U);
			VecN<T> y(rows);
			L.forwardSubstitution(y, b);
			U.backwardSubstitution(x, y);
		}

		void LUDecomposition(MatrixMN& L, MatrixMN& U) const
		{
			for(unsigned int i = 0; i < rows; i++)
			{
				U(0, i) = operator()(0, i);
				L(i, 0) = operator()(i, 0) / operator()(0, 0);
			}

			for(unsigned int i = 1; i < rows; i++)
			{
				for (unsigned int j = i; j < columns; j++)
				{
					T sum = 0;

					for(unsigned int k = 0; k < i; k++)
					{
						sum += L(i, k) * U(k, j);
					}

					U(i, j) = operator()(i, j) - sum;
				}

				L(i, i) = 1.0;

				for(unsigned int j = i + 1; j < rows; j++)
				{
					T sum = 0;

					for(unsigned int k = 0; k < i; k++)
					{
						sum += L(j, k) * U(k, i);
					}

					L(j, i) = (operator()(i, j) - sum) / U(i, i);
				}
			}
		}

		void SVD(MatrixMN& U, VecN<T>& S, MatrixMN& V) const
		{

			int m = rows;
			int n = columns;
			int nu = std::min(m,n);

			VecN<T> e(n);
			VecN<T> work(m);
			MatrixMN A(*this);
			// boolean
			int i=0, j=0, k=0;

			// Reduce A to bidiagonal form, storing the diagonal elements
			// in s and the super-diagonal elements in e.

			int nct = std::min(m-1,n);
			int nrt = std::max(0,std::min(n-2,m));
			for (k = 0; k < std::max(nct,nrt); k++)
			{
				if (k < nct)
				{

					// Compute the transformation for the k-th column and
					// place the k-th diagonal in S[k].
					// Compute 2-norm of k-th column without under/overflow.
					S[k] = 0;
					for (i = k; i < m; i++)
					{
						S[k] = hypot(S[k],A(i,k));
					}
					if (S[k] != 0.0)
					{
						if (A(k,k) < 0.0)
						{
							S[k] = -S[k];
						}
						for (i = k; i < m; i++)
						{
							A(i,k) /= S[k];
						}
						A(k,k) += 1.0;
					}
					S[k] = -S[k];
				}
				for (j = k+1; j < n; j++)
				{
					if ((k < nct) && (S[k] != 0.0))
					{

						// Apply the transformation.

						double t(0.0);
						for (i = k; i < m; i++)
						{
							t += A(i,k)*A(i,j);
						}
						t = -t/A(k,k);
						for (i = k; i < m; i++)
						{
							A(i,j) += t*A(i,k);
						}
					}

					// Place the k-th row of A into e for the
					// subsequent calculation of the row transformation.

					e[j] = A(k,j);
				}
				if (k < nct)
				{

					// Place the transformation in U for subsequent back
					// multiplication.

					for (i = k; i < m; i++)
					{
						U(i,k) = A(i,k);
					}
				}
				if (k < nrt) {

					// Compute the k-th row transformation and place the
					// k-th super-diagonal in e[k].
					// Compute 2-norm without under/overflow.
					e[k] = 0;
					for (i = k+1; i < n; i++)
					{
						e[k] = hypot(e[k],e[i]);
					}
					if (e[k] != 0.0)
					{
						if (e[k+1] < 0.0)
						{
							e[k] = -e[k];
						}
						for (i = k+1; i < n; i++)
						{
							e[i] /= e[k];
						}
						e[k+1] += 1.0;
					}
					e[k] = -e[k];
					if ((k+1 < m) & (e[k] != 0.0))
					{

						// Apply the transformation.

						for (i = k+1; i < m; i++)
						{
							work[i] = 0.0;
						}
						for (j = k+1; j < n; j++)
						{
							for (i = k+1; i < m; i++)
							{
								work[i] += e[j]*A(i,j);
							}
						}
						for (j = k+1; j < n; j++)
						{
							double t(-e[j]/e[k+1]);
							for (i = k+1; i < m; i++)
							{
								A(i,j) += t*work[i];
							}
						}
					}
					// Place the transformation in V for subsequent
					// back multiplication.

					for (i = k+1; i < n; i++)
					{
						V(i,k) = e[i];
					}
				}
			}

			// Set up the final bidiagonal matrix or order p.

			int p = std::min(n,m+1);
			if (nct < n)
			{
				S[nct] = A(nct,nct);
			}
			if (m < p)
			{
				S[p-1] = 0.0;
			}
			if (nrt+1 < p)
			{
				e[nrt] = A(nrt,p-1);
			}
			e[p-1] = 0.0;

			// If required, generate U.

			for (j = nct; j < nu; j++)
			{
				for (i = 0; i < m; i++)
				{
					U(i,j) = 0.0;
				}
				U(j,j) = 1.0;
			}
			for (k = nct-1; k >= 0; k--)
			{
				if (S[k] != 0.0)
				{
					for (j = k+1; j < nu; j++)
					{
						double t(0.0);
						for (i = k; i < m; i++)
						{
							t += U(i,k)*U(i,j);
						}
						t = -t/U(k,k);
						for (i = k; i < m; i++)
						{
							U(i,j) += t*U(i,k);
						}
					}
					for (i = k; i < m; i++ )
					{
						U(i,k) = -U(i,k);
					}
					U(k,k) = 1.0 + U(k,k);
					for (i = 0; i < k-1; i++)
					{
						U(i,k) = 0.0;
					}
				}
				else
				{
					for (i = 0; i < m; i++)
					{
						U(i,k) = 0.0;
					}
					U(k,k) = 1.0;
				}
			}

			// If required, generate V.

			for (k = n-1; k >= 0; k--)
			{
				if ((k < nrt) & (e[k] != 0.0))
				{
					for (j = k+1; j < nu; j++)
					{
						double t(0.0);
						for (i = k+1; i < n; i++)
						{
							t += V(i,k)*V(i,j);
						}
						t = -t/V(k+1,k);
						for (i = k+1; i < n; i++)
						{
							V(i,j) += t*V(i,k);
						}
					}
				}
				for (i = 0; i < n; i++)
				{
					V(i,k) = 0.0;
				}
				V(k,k) = 1.0;
			}

			// Main iteration loop for the singular values.

			int pp = p-1;
			int iter = 0;
			double eps(pow(2.0,-52.0));
			while (p > 0)
			{
				int k=0;
				int kase=0;

				// Here is where a test for too many iterations would go.

				// This section of the program inspects for
				// negligible elements in the s and e arrays.  On
				// completion the variables kase and k are set as follows.

				// kase = 1     if s(p) and e[k-1] are negligible and k<p
				// kase = 2     if s(k) is negligible and k<p
				// kase = 3     if e[k-1] is negligible, k<p, and
				//              s(k), ..., s(p) are not negligible (qr step).
				// kase = 4     if e(p-1) is negligible (convergence).

				for (k = p-2; k >= -1; k--)
				{
					if (k == -1)
					{
						break;
					}
					if (fabs(e[k]) <= eps*(fabs(S[k]) + fabs(S[k+1])))
					{
						e[k] = 0.0;
						break;
					}
				}

				if (k == p-2)
				{
					kase = 4;
				}
				else
				{
					int ks;
					for (ks = p-1; ks >= k; ks--)
					{
						if (ks == k)
						{
							break;
						}
						double t( (ks != p ? fabs(e[ks]) : 0.) +
							(ks != k+1 ? fabs(e[ks-1]) : 0.));
						if (fabs(S[ks]) <= eps*t)
						{
							S[ks] = 0.0;
							break;
						}
					}

					if (ks == k)
					{
						kase = 3;
					}
					else
						if (ks == p-1)
						{
							kase = 1;
						}
						else
						{
							kase = 2;
							k = ks;
						}
				}
				k++;

				// Perform the task indicated by kase.

				switch (kase)
				{
					// Deflate negligible s(p).
				case 1:
					{
						double f(e[p-2]);
						e[p-2] = 0.0;
						for (j = p-2; j >= k; j--)
						{
							double t( hypot(S[j],f));
							double cs(S[j]/t);
							double sn(f/t);
							S[j] = t;
							if (j != k)
							{
								f = -sn*e[j-1];
								e[j-1] = cs*e[j-1];
							}

							for (i = 0; i < n; i++)
							{
								t = cs*V(i,j) + sn*V(i,p-1);
								V(i,p-1) = -sn*V(i,j) + cs*V(i,p-1);
								V(i,j) = t;
							}

						}
					}
					break;

					// Split at negligible s(k).

				case 2:
					{
						double f(e[k-1]);
						e[k-1] = 0.0;
						for (j = k; j < p; j++)
						{
							double t(hypot(S[j],f));
							double cs( S[j]/t);
							double sn(f/t);
							S[j] = t;
							f = -sn*e[j];
							e[j] = cs*e[j];
							for (i = 0; i < m; i++)
							{
								t = cs*U(i,j) + sn*U(i,k-1);
								U(i,k-1) = -sn*U(i,j) + cs*U(i,k-1);
								U(i,j) = t;
							}
						}
					}
					break;

					// Perform one qr step.

				case 3:
					{
						// Calculate the shift.

						double scale = std::max(std::max(std::max(std::max(
							fabs(S[p-1]),fabs(S[p-2])),fabs(e[p-2])),
							fabs(S[k])),fabs(e[k]));
						double sp = S[p-1]/scale;
						double spm1 = S[p-2]/scale;
						double epm1 = e[p-2]/scale;
						double sk = S[k]/scale;
						double ek = e[k]/scale;
						double b = ((spm1 + sp)*(spm1 - sp) + epm1*epm1)/2.0;
						double c = (sp*epm1)*(sp*epm1);
						double shift = 0.0;
						if ((b != 0.0) || (c != 0.0))
						{
							shift = sqrt(b*b + c);
							if (b < 0.0)
							{
								shift = -shift;
							}
							shift = c/(b + shift);
						}
						double f = (sk + sp)*(sk - sp) + shift;
						double g = sk*ek;

						// Chase zeros.

						for (j = k; j < p-1; j++)
						{
							double t = hypot(f,g);
							double cs = f/t;
							double sn = g/t;
							if (j != k)
							{
								e[j-1] = t;
							}
							f = cs*S[j] + sn*e[j];
							e[j] = cs*e[j] - sn*S[j];
							g = sn*S[j+1];
							S[j+1] = cs*S[j+1];
							for (i = 0; i < n; i++)
							{
								t = cs*V(i,j) + sn*V(i,j+1);
								V(i,j+1) = -sn*V(i,j) + cs*V(i,j+1);
								V(i,j) = t;
							}
							t = hypot(f,g);
							cs = f/t;
							sn = g/t;
							S[j] = t;
							f = cs*e[j] + sn*S[j+1];
							S[j+1] = -sn*e[j] + cs*S[j+1];
							g = sn*e[j+1];
							e[j+1] = cs*e[j+1];
							if (j < m-1)
							{
								for (i = 0; i < m; i++)
								{
									t = cs*U(i,j) + sn*U(i,j+1);
									U(i,j+1) = -sn*U(i,j) + cs*U(i,j+1);
									U(i,j) = t;
								}
							}
						}
						e[p-2] = f;
						iter = iter + 1;
					}
					break;

					// Convergence.

				case 4:
					{
						// Make the singular values positive.

						if (S[k] <= 0.0)
						{
							S[k] = (S[k] < 0.0 ? -S[k] : 0.0);
							for (i = 0; i <= pp; i++)
							{
								V(i,k) = -V(i,k);
							}
						}

						// Order the singular values.

						while (k < pp)
						{
							if (S[k] >= S[k+1])
							{
								break;
							}
							double t = S[k];
							S[k] = S[k+1];
							S[k+1] = t;
							if (k < n-1)
							{
								for (i = 0; i < n; i++)
								{
									t = V(i,k+1); V(i,k+1) = V(i,k); V(i,k) = t;
								}
							}
							if (k < m-1)
							{
								for (i = 0; i < m; i++)
								{
									t = U(i,k+1); U(i,k+1) = U(i,k); U(i,k) = t;
								}
							}
							k++;
						}
						iter = 0;
						p--;

					}
					break;
				}
			}
		}


		VecN<T> operator*(const VecN<T>& v)
		{
			assert(v.getSize() == columns);
			VecN<T> r(rows);
			for(unsigned int i = 0; i < rows; i++ )
			{
				for(unsigned int j = 0; j < columns; j++)
				{
					r[i] += operator()(i,j) * v[j];
				}
			}

			return r;
		}
		friend VecN<T> operator* (const VecN<T>& v, const MatrixMN& m)
		{
			assert(v.getSize() == m.rows);
			VecN<T> r(m.columns);
			for(unsigned int i = 0; i < m.columns; i++ )
			{
				for(unsigned int j = 0; j < m.rows; j++)
				{
					r[i] += m(j, i) * v[j];
				}
			}

			return r;
		}

		MatrixMN operator*(const MatrixMN& r)
		{
			assert(columns == r.rows);
			MatrixMN result(rows, r.columns);

			for(unsigned int i = 0; i < rows; i++)
			{
				for(unsigned int j = 0; j < r.columns; j++)
				{
					for(unsigned int  k = 0; k < columns; k++)
					{
						result(i, j) += operator()(i, k) * r(k, j);
					}
				}
			}

			return result;
		}

		friend std::ostream& operator <<( std::ostream& o, const MatrixMN& m )
		{
			for(unsigned int i = 0; i < m.rows; i++ )
			{
				for(unsigned int j = 0; j < m.columns; j++)
				{
					o << std::right << std::setw(12) <<  m(i, j) << " ";
				}

				o << "\n";
			}

			return o;
		}

		T* getElements(){return elements;}
	private:
		unsigned int rows, columns;
		T* elements;
	};

	typedef MatrixMN<int> MatrixMNi;
	typedef MatrixMN<float> MatrixMNf;
	typedef MatrixMN<double> MatrixMNd;
}