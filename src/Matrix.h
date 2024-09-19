#pragma once
#include <array>
#include <initializer_list>
#include <iostream>
#include <string>
#include <algorithm>
#include <deque>
#include <functional>
#include <type_traits>

namespace lin_alg
{
	// my own matrix and matrix operations implementation
	template <size_t N, size_t M>
	struct Matrix final
	{
		using matrix_t = std::array<std::array<float, M>, N>;

		Matrix() = default;

		Matrix(std::initializer_list<std::initializer_list<float>> il)
		{
			if (il.size() != N)
			{
				std::string er = "Rows amount ain't equal " + std::to_string(N);
				throw std::runtime_error(er.data());
			}
			for (size_t i = 0; i < N; ++i)
			{
				if ((il.begin() + i)->size() != M)
				{
					std::string er = "Cols num at row " + std::to_string(i) + "ain't equal to " + std::to_string(M);
					throw std::runtime_error(er.data());
				}
				for (size_t j = 0; j < M; ++j)
				{
					m_matrix[i][j] = *((il.begin() + i)->begin() + j);
				}
			}
		}

		void print()
		{
			for (size_t i = 0; i < N; ++i)
			{
				for (size_t j = 0; j < M; ++j)
				{
					std::cout << m_matrix[i][j] << " ";
				}
				std::cout << "\n";
			}
		}

		float& elem(size_t n, size_t m)
		{
			if (n < 0 || m < 0)
			{
				return m_matrix[0][0];
			}
			return m_matrix[n][m];
		}

		float elem(size_t n, size_t m) const
		{
			if (n < 0 || m < 0)
			{
				return m_matrix[0][0];
			}
			return m_matrix[n][m];
		}

	private:
		matrix_t m_matrix;
	};

	template <size_t N, size_t M>
	Matrix<M, N> getTransposed(const Matrix<N, M>& A) 
	{
		Matrix<M, N> res;
		for (size_t i = 0; i < N; ++i)
		{
			for (size_t j = 0; j < M; ++j)
			{
				res.elem(j, i) = A.elem(i, j);
			}
		}
		return res;
	}

	template <size_t N, size_t M>
	Matrix<N, M> operator+(Matrix<N, M> a, const Matrix<N, M>& b)
	{
		for (size_t i = 0; i < N; ++i)
		{
			for (size_t j = 0; j < M; ++j)
			{
				a.elem(i, j) += b.elem(i, j);
			}
		}
		return a;
	}

	template <size_t N, size_t M>
	Matrix<N, M> operator-(Matrix<N, M> a, const Matrix<N, M>& b)
	{
		for (size_t i = 0; i < N; ++i)
		{
			for (size_t j = 0; j < M; ++j)
			{
				a.elem(i, j) -= b.elem(i, j);
			}
		}
		return a;
	}

	template <size_t N, size_t M>
	Matrix<N, M> operator*(Matrix<N, M> a, float mult)
	{
		for (size_t i = 0; i < N; ++i)
		{
			for (size_t j = 0; j < M; ++j)
			{
				a.elem(i, j) *= mult;
			}
		}
		return a;
	}

	template <size_t N, size_t M>
	Matrix<N, M> operator*(float mult, Matrix<N, M> a)
	{
		return a * mult;
	}

	template <size_t N1, size_t M2, size_t MN>
	Matrix<N1, M2> operator*(const Matrix<N1, MN>& a, const Matrix<MN, M2>& b)
	{
		Matrix<N1, M2> res;
		for (size_t i = 0; i < N1; ++i)
		{
			for (size_t j = 0; j < M2; ++j)
			{
				float val = 0;
				for (size_t k = 0; k < MN; ++k)
				{
					val += a.elem(i, k) * b.elem(k, j);
				}
				res.elem(i, j) = val;
			}
		}
		return res;
	}

	template <size_t NM>
	float det(const Matrix<NM, NM>& m);

	template <size_t NM>
	float alg_comp(const Matrix<NM, NM>& m, size_t i_, size_t j_)
	{
		if (i_ >= NM || j_ >= NM)
			throw std::runtime_error("i or j out of matrix");

		size_t				   next_i = 0, next_j = 0;
		Matrix<NM - 1, NM - 1> res;
		for (size_t i = 0; i < NM - 1; ++i)
		{
			next_j = 0;
			if (next_i == i_)
			{
				next_i++;
			}

			for (size_t j = 0; j < NM - 1; ++j)
			{
				if (next_j == j_)
				{
					next_j++;
				}
				res.elem(i, j) = m.elem(next_i, next_j);
				next_j++;
			}
			next_i++;
		}

		return ((((i_ + 1) + (j_ + 1)) % 2 == 0) ? det(res) : (-1 * det(res)));
	}

	template <size_t NM>
	float mult_alg_comp(const Matrix<NM, NM>& m, size_t i_, size_t j_)
	{
		if (i_ >= NM || j_ >= NM)
			throw std::runtime_error("i or j out of matrix");

		size_t				   next_i = 0, next_j = 0;
		Matrix<NM - 1, NM - 1> res;
		for (size_t i = 0; i < NM - 1; ++i)
		{
			next_j = 0;
			if (next_i == i_)
			{
				next_i++;
			}

			for (size_t j = 0; j < NM - 1; ++j)
			{
				if (next_j == j_)
				{
					next_j++;
				}
				res.elem(i, j) = m.elem(next_i, next_j);
				next_j++;
			}
			next_i++;
		}

		return m.elem(i_, j_) * ((((i_ + 1) + (j_ + 1)) % 2 == 0) ? det(res) : (-1 * det(res)));
	}

	template <size_t NM>
	static float det(const Matrix<NM, NM>& m)
	{
		float res = 0;
		for (size_t i = 0; i < NM; ++i)
		{
			res += mult_alg_comp(m, 0, i);
		}
		return res;
	}

	template <>
	inline float det<1>(const Matrix<1, 1>& m)
	{
		return m.elem(0, 0);
	}

	template <>
	inline float det<2>(const Matrix<2, 2>& m)
	{
		return m.elem(0, 0) * m.elem(1, 1) - m.elem(0, 1) * m.elem(1, 0);
	}

	template <>
	inline float det<3>(const Matrix<3, 3>& m)
	{
		return m.elem(0, 0) * m.elem(1, 1) * m.elem(2, 2)
			+ m.elem(0, 1) * m.elem(1, 2) * m.elem(2, 0)
			+ m.elem(1, 0) * m.elem(2, 1) * m.elem(0, 2)
			- m.elem(0, 2) * m.elem(1, 1) * m.elem(2, 0)
			- m.elem(1, 0) * m.elem(0, 1) * m.elem(2, 2)
			- m.elem(0, 0) * m.elem(1, 2) * m.elem(2, 1);
	}

	template <size_t N>
	Matrix<N, N> getInversed(const Matrix<N, N>& A)
	{
		Matrix<N, N> res;
		float		 detA = det(A);
		if (detA == 0.0f)
		{
			throw std::runtime_error("det equals to zero, inverse matrix could not calculate");
		}
		for (size_t i = 0; i < N; ++i)
		{
			for (size_t j = 0; j < N; ++j)
			{
				auto inter = alg_comp(A, j, i) / detA;
				res.elem(i, j) = inter;
			}
		}
		return res;
	}

	template <>
	inline Matrix<2, 2> getInversed<2>(const Matrix<2, 2>& A)
	{
		float detA = det(A);
		if (detA == 0.0f)
		{
			throw std::runtime_error("det equals to zero, inverse matrix could not calculate");
		}

		Matrix<2, 2> res;
		res.elem(0, 0) = A.elem(1, 1) / detA;  // d / detA
		res.elem(0, 1) = -A.elem(0, 1) / detA; // -b / detA
		res.elem(1, 0) = -A.elem(1, 0) / detA; // -c / detA
		res.elem(1, 1) = A.elem(0, 0) / detA;  // a / detA

		return res;
	}

	template <size_t N, size_t M>
	void SwapRows(Matrix<N, M>& A, size_t first_row, size_t second_row)
	{
		for (size_t i = 0; i < M; ++i)
		{
			std::swap(A.elem(first_row, i), A.elem(second_row, i));
		}
	}
} // namespace lin_alg