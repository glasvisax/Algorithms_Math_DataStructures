#pragma once
#include "Vector.h"
#include "Matrix.h" 

using namespace structs;
namespace lin_alg
{
	template <size_t N>
	using VectorN = Matrix<1, N>;

	template <size_t N>
	VectorN<N> Kramer_method(Matrix<N, N> A, const VectorN<N>& B)
	{
		VectorN<N> res;
		VectorN<N> inter;
		float	   detA = det(A);
		if (detA == 0.0f)
		{
			throw std::exception("det equals to zero, solution could not finded");
		}
		for (size_t j = 0; j < N; ++j)
		{
			inter.elem(0, j) = A.elem(j, 0);
			A.elem(j, 0) = B.elem(0, j);
		}

		for (size_t i = 0; i < N; ++i)
		{
			res.elem(0, i) = det(A) / detA;
			for (size_t j = 0; j < N; ++j)
			{
				A.elem(j, i) = inter.elem(0, j);
				if (i + 1 < N)
				{
					inter.elem(0, j) = A.elem(j, i + 1);
					A.elem(j, i + 1) = B.elem(0, j);
				}
			}
		}

		return res;
	}

	template <size_t N>
	VectorN<N> Matrix_method(const Matrix<N, N>& A, const Matrix<1, N>& B)
	{
		Matrix<N, N> inv = getInversed(A);
		auto		 res = getTransposed(inv * getTransposed(B));
		return res;
	}

	template <size_t N>
	VectorN<N> Gauss_method(Matrix<N, N> A, Matrix<1, N> B)
	{
		// if det == 0
		float detA = det(A);
		if (detA == 0.0f)
		{
			throw std::exception("det equals to zero, solution could not finded");
		}
		// swap row if main elements equals to zero
		for (size_t i = 0; i < N; ++i)
		{
			float main_el = A.elem(i, i);
			if (main_el == 0)
			{
				for (size_t j = 0; j < N; ++j)
				{
					if (A.elem(j, i) != 0 && A.elem(i, j) != 0)
					{
						SwapRows(A, i, j);
					}
				}
			}
		}
		// forward step
		for (size_t i = 0; i < N; ++i)
		{
			float main_el = A.elem(i, i);
			for (size_t j = 0; j < N; ++j)
			{
				if (j == i)
				{
					A.elem(i, j) = 1;
					continue;
				}
				A.elem(i, j) = A.elem(i, j) / main_el;
			}

			B.elem(0, i) = B.elem(0, i) / main_el;
			for (size_t n = i + 1; n < N; ++n)
			{
				float factor = -A.elem(n, i);
				A.elem(n, i) = 0;
				for (size_t j = i + 1; j < N; ++j)
				{
					A.elem(n, j) += A.elem(i, j) * factor;
				}
				B.elem(0, n) += B.elem(0, i) * factor;
			}
		}
		// back step
		for (size_t i = N - 2; i != size_t(-1); --i)
		{
			for (size_t j = N - 1; j > i; --j)
			{
				B.elem(0, i) += -B.elem(0, i + 1) * A.elem(i, j);
			}
		}
		return B;
	}

} // namespace lin_alg