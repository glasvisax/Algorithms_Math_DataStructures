﻿#define NOMINMAX
#include <iostream>
#include <cassert>
#include "avlmap.h"
#include "Matrix.h"
#include "rbmap.h"
#include "Graph.h"
#include "Vector.h"
#include "SLE_algorithms.h"

using namespace lin_alg;
using namespace graph;
using namespace graph::tree;
using namespace structs;

void test_avl_tree();
void test_matrix();
void test_rb_tree();
void test_graphs();
void test_vector();
void test_SLE_Algs();

int main()
{
	test_SLE_Algs();

}
void test_vector()
{

	Vector<int> vec;
	assert(vec.getSize() == 0);
	assert(vec.getCapacity() == 0);
	vec.pushBack(1);
	vec.pushBack(2);
	vec.pushBack(3);
	vec.pushBack(4);
	vec.pushBack(5);

	assert(vec.getSize() == 5);
	assert(vec.getCapacity() >= 5);

	assert(vec[0] == 1);
	assert(vec[1] == 2);
	assert(vec[2] == 3);
	assert(vec[3] == 4);
	assert(vec[4] == 5);

	try
	{
		vec.at(4) = 10;
		assert(vec.at(4) == 10);
	}
	catch (...)
	{
		assert(false); // не должно сюда попасть
	}

	try
	{
		vec.at(5);
		assert(false);
	}
	catch (...)
	{
	}

	Vector<int> vecCopy(vec);
	assert(vecCopy.getSize() == vec.getSize());
	for (size_t i = 0; i < vec.getSize(); ++i)
	{
		assert(vecCopy[i] == vec[i]);
	}

	Vector<int> vecMove(std::move(vecCopy));
	assert(vecMove.getSize() == 5);
	assert(vecCopy.getSize() == 0);

	Vector<int> vecAssign = vecMove;
	assert(vecAssign.getSize() == vecMove.getSize());
	for (size_t i = 0; i < vecMove.getSize(); ++i)
	{
		assert(vecAssign[i] == vecMove[i]);
	}

	Vector<int> vecMoveAssign;
	vecMoveAssign = std::move(vecAssign);
	assert(vecMoveAssign.getSize() == 5);
	assert(vecAssign.getSize() == 0);

	Vector<int>::Iterator it = vecMoveAssign.begin();
	assert(*it == 1);
	++it;
	assert(*it == 2);
	it++;
	assert(*it == 3);

	const Vector<int>&		   constVec = vecMoveAssign;
	Vector<int>::ConstIterator cit = constVec.cbegin();
	assert(*cit == 1);
	++cit;
	assert(*cit == 2);
	cit++;
	assert(*cit == 3);

	vecMoveAssign.reserve(20);
	assert(vecMoveAssign.getCapacity() == 20);
	assert(vecMoveAssign.getSize() == 5);

	std::cout << "All tests passed!" << std::endl;
}

void test_SLE_Algs()
{
	Matrix<3, 3> A = {
		{ 4.0f, 3.0f, 0.0f },
		{ 10.0f, 7.51f, 8.0f },
		{ 2.0f, -1.0f, -1.0f }
	};

	Matrix<1, 3> B = { { 3.0f, -0.49f, 0.0f } };

	Matrix_method(A, B).print();
	Kramer_method(A, B).print();
	Matrix_method(A, B).print();
}

void test_graphs()
{
	// weighted graph
	WGraph wg1;
	wg1.addEdge(0, 1, 4);
	wg1.addEdge(0, 2, 1);
	wg1.addEdge(1, 3, 1);
	wg1.addEdge(2, 1, 2);
	wg1.addEdge(2, 3, 5);

	// dijkstra
	auto res1 = findPath(0, 3, wg1);

	for (auto i : res1.first)
	{
		std::cout << i << " ";
	}
	std::cout << "\n";

	WGraph wg2;
	wg2.addEdge(0, 1, 1);
	wg2.addEdge(0, 2, 4);
	wg2.addEdge(1, 2, -2);
	wg2.addEdge(1, 3, 3);
	wg2.addEdge(2, 3, 2);

	// bellman-ford
	auto res2 = findPath(0, 3, wg2);

	for (auto i : res2.first)
	{
		std::cout << i << " ";
	}
	std::cout << "\n";

	// unweighted graph
	UGraph<int> ug;
	ug.addEdge(1, 2);
	ug.addEdge(1, 3);
	ug.addEdge(2, 4);
	ug.addEdge(4, 7);
	ug.addEdge(4, 8);
	ug.addEdge(3, 6);
	ug.addEdge(3, 5);

	// bfs
	auto res3 = findPath(1, 6, ug);

	for (auto i : res3.first)
	{
		std::cout << i << " ";
	}
	std::cout << "\n";

	// dfs
	auto res4 = findPath(1, 9, ug, false);

	for (auto i : res4.first)
	{
		std::cout << i << " ";
	}
	std::cout << "\n";
}

void test_avl_tree()
{
	avlmap<int, std::string> am;
	am.insert(5, "c");
	am.insert(2, "c");
	am.insert(8, "c");
	am.insert(4, "c");
	am.insert(3, "c");
	am.insert(6, "c");
	am.insert(10, "c");
	am.insert(9, "c");
	am.insert(1, "c");
	am.remove(6);
	am.remove(9);
	am.remove(5);
	am.printTree();
	std::cout << am.find(3) << "\n\n";
}

void test_matrix()
{
	Matrix<3, 2> a = { { 1, 2 },
		{ 2, 1 },
		{ 3, 4 } };

	Matrix<2, 3> b = { { 1, 2, 3 },
		{ 2, 1, 4 } };

	auto d = a * b;

	auto e = getTransposed(b);

	Matrix<4, 4> y = {
		{ 1, 0, 3, 2 },
		{ 2, 0, -1, 0 },
		{ 3, 2, 0, 3 },
		{ 0, 1, 2, 3 }
	};

	std::cout << det(y) << "\n\n";
}

void test_rb_tree()
{
	rbmap<int, std::string> rbm;
	rbm.insert(5, "c");
	rbm.insert(2, "c");
	rbm.insert(8, "c");
	rbm.insert(4, "c");
	rbm.insert(3, "c");
	rbm.insert(6, "c");
	rbm.insert(10, "c");
	rbm.insert(9, "c");
	rbm.insert(1, "c");
	rbm.remove(6);
	rbm.remove(9);
	rbm.printTree();
	std::cout << rbm.find(3) << "\n\n";
}