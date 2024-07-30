#include "avlmap.h"
#include "Matrix.h"

int main()
{
	// авл дерево
	avlmap<int, std::string> am;
	am.insert(3, "test1");
	am.insert(1, "test2");
	am.insert(2, "test3");
	am.insert(4, "test3");
	am.printTree();
	am.remove(2);
	am.printTree();
	am.remove(4);
	am.printTree();

	std::cout << am.find(3);


	// матрицы
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

	std::cout << det(y);
}
