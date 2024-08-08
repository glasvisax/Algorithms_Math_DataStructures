#include <iostream>
#include "avlmap.h"
#include "Matrix.h"
#include "rbmap.h"
#include <Windows.h>
#include <map>

int main()
{
	// avl tree map
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

	// matrix
	Matrix<3, 2> a = {	{ 1, 2 },
						{ 2, 1 },
						{ 3, 4 } };

	Matrix<2, 3> b = {	{ 1, 2, 3 },
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

	// red black tree map
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
