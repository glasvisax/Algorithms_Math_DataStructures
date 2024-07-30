#pragma once
#include <array>
#include <initializer_list>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <deque>

// Моя имплементация структуры данных Матрицы и операций над ней (в процессе)
template <size_t N, size_t M>
struct Matrix final
{
	using matrix_t = std::array<std::array<int, M>, N>;

	Matrix() = default;

	Matrix(std::initializer_list<std::initializer_list<int>> il)
	{
		if (il.size() != N)
		{
			std::string er = "Rows amount ain't equal " + std::to_string(N);
			throw std::exception(er.data());
		}
		for (size_t i = 0; i < N; ++i)
		{
			if ((il.begin() + i)->size() != M)
			{
				std::string er = "Cols num at row " + std::to_string(i) + "ain't equal to " + std::to_string(M);
				throw std::exception(er.data());
			}
			for (size_t j = 0; j < M; ++j)
			{
				m_matrix[i][j] = *((il.begin() + i)->begin() + j);
			}
		}
	}

	void printMatrix()
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

	int& elem(size_t n, size_t m)
	{
		if (n < 0 || m < 0)
		{
			return m_matrix[0][0];
		}
		return m_matrix[n][m];
	}

	int elem(size_t n, size_t m) const
	{
		if (n < 0 || m < 0)
		{
			return m_matrix[0][0];
		}
		return m_matrix[n][m];
	}

	Matrix<M, N> getTransposed()
	{
		Matrix<M, N> res;
		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < M; ++j)
			{
				res.elem(j, i) = this->elem(i, j);
			}
		}
		return res;
	}

private:
	matrix_t m_matrix;
};

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
Matrix<N, M> operator*(Matrix<N, M> a, int mult)
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
Matrix<N, M> operator*(int mult, Matrix<N, M> a)
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

template <size_t N1, size_t M2, size_t MN>
Matrix<N1, M2> operator*(const Matrix<N1, MN>& a, const Matrix<MN, M2>& b)
{
	Matrix<N1, M2> res;
	for (size_t i = 0; i < N1; ++i)
	{
		for (size_t j = 0; j < M2; ++j)
		{
			int val = 0;
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
int det(const Matrix<NM, NM>& m);

template <size_t NM>
int alg_comp(const Matrix<NM, NM>& m, size_t i_, size_t j_)
{
	if (i_ >= NM || j_ >= NM)
		throw std::exception("i or j out of matrix");

	size_t				   next_i = 0, next_j = 0;
	Matrix<NM - 1, NM - 1> res;
	for (int i = 0; i < NM - 1; ++i)
	{
		next_j = 0;
		if (next_i == i_)
		{
			next_i++;
		}

		for (int j = 0; j < NM - 1; ++j)
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
static int det(const Matrix<NM, NM>& m)
{
	int res = 0;
	for (int i = 0; i < NM; ++i)
	{
		res += alg_comp(m, 0, i);
	}
	return res;
}

template <>
int det<1>(const Matrix<1, 1>& m)
{
	return m.elem(0, 0);
}

template <>
int det<2>(const Matrix<2, 2>& m)
{
	return m.elem(0, 0) * m.elem(1, 1) - m.elem(0, 1) * m.elem(1, 0);
}

template <>
int det<3>(const Matrix<3, 3>& m)
{
	return m.elem(0, 0) * m.elem(1, 1) * m.elem(2, 2)
		+ m.elem(0, 1) * m.elem(1, 2) * m.elem(2, 0)
		+ m.elem(1, 0) * m.elem(2, 1) * m.elem(0, 2)
		- m.elem(0, 2) * m.elem(1, 1) * m.elem(2, 0)
		- m.elem(1, 0) * m.elem(0, 1) * m.elem(2, 2)
		- m.elem(0, 0) * m.elem(1, 2) * m.elem(2, 1);
}