#define NOMINMAX
#include <iostream>
#include "avlmap.h"
#include "Matrix.h"
#include "rbmap.h"
#include "Graph.h"

using namespace lin_alg;
using namespace graph;
using namespace graph::tree;

void test_avl_tree();
void test_matrix();
void test_rb_tree();
void test_graphs();

int main()
{
	return 0;
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
	ug.addEdge(4, 5);
	ug.addEdge(4, 10);
	ug.addEdge(3, 6);
	ug.addEdge(3, 8);
	ug.addEdge(3, 7);
	ug.addEdge(8, 9);

	// bfs
	auto res3 = findPath(1, 9, ug);

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

	auto e = b.getTransposed();

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