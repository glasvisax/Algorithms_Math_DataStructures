#pragma once

#include <deque>
#include <iostream>
#include <memory>
#include <utility>
#include <type_traits>

template <typename K, typename V>
class rbmap
{
	using Color = bool;
	static const Color BLACK = 0;
	static const Color RED = 1;

	struct BaseNode
	{
		BaseNode() {}
		BaseNode(Color color)
			: color(color) {}
		Color  color = BLACK;
		size_t arr_index = 0;
	};

	struct Node : public BaseNode
	{
		Node(K key, const V& val)
			: key(key)
			, value(val)
			, BaseNode(RED) {}

		K key;
		V value;

		Node* left = nullptr;
		Node* right = nullptr;
		Node* parent = nullptr;
	} *root = nullptr, *null = nullptr;

	static inline BaseNode NULL_NODE;

	std::deque<Node>   nodes_store;
	std::stack<size_t> free_indexes;

public:
	// Default ctor
	rbmap();

	// Prints the tree structure to the console
	void printTree() const;

	// Adding a Node with a Key and a Value
	void insert(K key, const V& val);

	// Finding a value by key (constant)
	V find(K key) const;

	// Finding a value by key
	V& find(K key);

	// Returns the maximum element of the tree (constant)
	V& getMax() { return getMaxFrom(root)->value; }

	// Returns the maximum element of the tree 
	V getMax() const { return getMaxFrom(root)->value; }
	
	// Returns the minimum element of the tree 
	V& getMin() { return getMinFrom(root)->value; }

	// Returns the minimum element of the tree (constant)
	V getMin() const { return getMinFrom(root)->value; }

	// Removing a value by key
	void remove(K key)
	{
		removeHelper(key, root);
	}

private:

	void eraseNode(Node* node);

	Node* getMaxFrom(Node* from);

	Node* getMinFrom(Node* from);

	void printTreeRec(Node* node, int indent) const;

	void removeHelper(K key, Node* tree);

	void removeBalance(Node* parent, bool deleted_left);

	inline bool isNull(Node* node) const
	{
		return node == null;
	}

	Node* unsafeFind(K key, Node* from);

	void insertBalance(Node* node);

	bool isLeft(Node* node)
	{
		return node->parent->left == node;
	}

	bool isRight(Node* node)
	{
		return node->parent->right == node;
	}

	void swapKVC(Node* n1, Node* n2);

	void swapKV(Node* n1, Node* n2);

	void rightRotate(Node* node);

	void leftRotate(Node* node);
};

template <typename K, typename V>
rbmap<K, V>::rbmap()
{
	null = static_cast<Node*>(&NULL_NODE);
	root = null;
}

template <typename K, typename V>
void rbmap<K, V>::printTree() const
{
	printTreeRec(root, 0);
	std::cout << "\n\n";
}

template <typename K, typename V>
void rbmap<K, V>::printTreeRec(Node* node, int indent) const
{
	if (!isNull(node))
	{
		if (!isNull(node->right))
		{
			printTreeRec(node->right, indent + 4);
		}
		if (indent)
		{
			std::cout << std::setw(indent) << ' ';
		}
		if (!isNull(node->right))
			std::cout << " /\n"
					  << std::setw(indent) << ' ';
		std::cout << node->key << ((node->color) ? "r" : "b")
				  << "\n ";
		if (!isNull(node->left))
		{
			std::cout << std::setw(indent) << ' ' << " \\\n";
			printTreeRec(node->left, indent + 4);
		}
	}
}

template <typename K, typename V>
void rbmap<K, V>::insert(K key, const V& val)
{
	Node* parent = unsafeFind(key, root);
	if (parent->key == key)
		return;

	Node*  current;
	size_t current_index;
	if (free_indexes.size() == 0)
	{
		nodes_store.emplace_back(key, val);
		current = &nodes_store.back();
		current_index = nodes_store.size() - 1;
	}
	else
	{
		auto free_addr = &*(nodes_store.begin() + free_indexes.top());
		current = new (free_addr) Node(key, val);
		current_index = free_indexes.top();
		free_indexes.pop();
	}

	if (isNull(root))
	{
		root = current;
		root->right = null;
		root->left = null;
		root->parent = null;
		root->color = BLACK;
		root->arr_index = current_index;
		return;
	}
	Node* child;
	if (key > parent->key)
	{
		parent->right = current;
		child = parent->right;
	}
	else
	{
		parent->left = current;
		child = parent->left;
	}

	child->left = null;
	child->right = null;
	child->parent = parent;
	child->arr_index = nodes_store.size() - 1;
	insertBalance(child);
}

template <typename K, typename V>
V rbmap<K, V>::find(K key) const
{
	if (isNull(root))
		throw std::exception("root does not exist");
	Node* current = unsafeFind(key, root);
	if (current->key == key)
	{
		return current->value;
	}
	throw std::exception("element does not exist");
}

template <typename K, typename V>
V& rbmap<K, V>::find(K key)
{
	if (isNull(root))
		throw std::exception("root does not exist");
	Node* current = unsafeFind(key, root);
	if (current->key == key)
	{
		return current->value;
	}
	throw std::exception("element does not exist");
}

template <typename K, typename V>
void rbmap<K, V>::eraseNode(Node* node)
{
	size_t curr_arr_index = node->arr_index;
	if (curr_arr_index != nodes_store.size() - 1)
	{
		nodes_store[curr_arr_index].~Node();
		free_indexes.push(curr_arr_index);
	}
	else
	{
		nodes_store.pop_back();
	}
}

template <typename K, typename V>
typename rbmap<K, V>::Node* rbmap<K, V>::getMaxFrom(Node* from)
{
	Node* current = from;
	while (true)
	{
		if (isNull(current->right))
			return current;
		current = current->right;
	}
}

template <typename K, typename V>
typename rbmap<K, V>::Node* rbmap<K, V>::getMinFrom(Node* from)
{
	Node* current = from;
	while (true)
	{
		if (isNull(current->left))
			return current;
		current = current->left;
	}
}

template <typename K, typename V>
void rbmap<K, V>::removeHelper(K key, Node* tree)
{
	Node* node = unsafeFind(key, tree);
	if (node->key != key)
	{
		return;
	}
	Node* current;
	if (isNull(node->right))
	{
		if (isNull(node->left))
		{
			if (node == root)
			{
				root = null;
			}
			else
			{
				if (isRight(node))
				{
					node->parent->right = null;
					if (node->color == BLACK)
					{
						removeBalance(node->parent, false); ///
					}
				}
				else
				{
					node->parent->left = null;
					if (node->color == BLACK)
					{
						removeBalance(node->parent, true);
					}
				}
			}
			eraseNode(node);
			return;
		}
		else
		{
			current = node->left;
		}
	}
	else if (isNull(node->left))
	{
		current = node->right;
	}
	else
	{
		Node* rmin = getMinFrom(node->right);
		swapKV(rmin, node);
		removeHelper(key, rmin);
		return;
	}
	if (node == root)
	{
		root = current;
		root->parent = null;
	}
	else if (isRight(node))
	{
		node->parent->right = current;
		current->parent = node->parent;
		if (node->color == BLACK)
		{
			if (current->color == RED)
			{
				current->color = BLACK;
			}
			else
			{
				removeBalance(node->parent, false);
			}
		}
	}
	else
	{
		node->parent->left = current;
		current->parent = node->parent;
		if (node->color == BLACK)
		{
			if (current->color == RED)
			{
				current->color = BLACK;
			}
			else
			{
				removeBalance(node->parent, false);
			}
		}
	}
	eraseNode(node);
}

template <typename K, typename V>
void rbmap<K, V>::removeBalance(Node* parent, bool deleted_left)
{
	if (parent == nullptr || isNull(parent))
	{
		return;
	}
	while (true)
	{
		if (deleted_left)
		{
			Node* br = parent->right;
			if (br->color == RED)
			{
				parent->color = RED;
				br->color = BLACK;
				leftRotate(parent);
			}
			else
			{
				if (br->right->color == RED)
				{
					br->color = parent->color;
					parent->color = BLACK;
					br->right->color = BLACK;
					leftRotate(parent);
					return;
				}
				else if (br->left->color == RED)
				{
					br->color = RED;
					br->left->color = BLACK;
					rightRotate(br);
				}
				else
				{
					br->color = RED;
					if (parent->color == BLACK)
					{
						deleted_left = isLeft(parent);
						parent = parent->parent;
					}
					else
					{
						parent->color = BLACK;
						return;
					}
				}
			}
		}
		else
		{
			Node* br = parent->left;
			if (br->color == RED)
			{
				parent->color = RED;
				br->color = BLACK;
				rightRotate(parent);
			}
			else
			{
				if (br->left->color == RED)
				{
					br->color = parent->color;
					parent->color = BLACK;
					br->left->color = BLACK;
					rightRotate(parent);
					return;
				}
				else if (br->right->color == RED)
				{
					br->color = RED;
					br->left->color = BLACK;
					leftRotate(br);
				}
				else
				{
					br->color = RED;
					if (parent->color == BLACK)
					{
						deleted_left = isLeft(parent);
						parent = parent->parent;
					}
					else
					{
						parent->color = BLACK;
						return;
					}
				}
			}
		}
	}
}

template <typename K, typename V>
typename rbmap<K, V>::Node* rbmap<K, V>::unsafeFind(K key, Node* from)
{
	Node* child = from;
	Node* parent = from;

	while (!isNull(child))
	{
		if (key == child->key)
		{
			break;
		}
		else if (key > child->key)
		{
			parent = child;
			child = child->right;
		}
		else if (key < child->key)
		{
			parent = child;
			child = child->left;
		}
	}

	return isNull(child) ? parent : child;
}

template <typename K, typename V>
void rbmap<K, V>::insertBalance(Node* node)
{
	if (isNull(node) || node == root)
	{
		return;
	}
	Node* parent = node->parent;
	if (parent->color == RED)
	{
		Node* grandpa = node->parent->parent;

		if (isLeft(parent))
		{
			Node* uncle = grandpa->right;
			if (uncle->color == RED)
			{
				grandpa->color = RED;
				parent->color = BLACK;
				uncle->color = BLACK;
				insertBalance(grandpa);
			}
			else
			{
				if (isRight(node))
				{
					leftRotate(parent);
				}
				rightRotate(grandpa);
				parent->color = RED;
				grandpa->color = BLACK;
				node->color = RED;
			}
		}
		else
		{
			Node* uncle = grandpa->left;
			if (uncle->color == RED)
			{
				grandpa->color = RED;
				parent->color = BLACK;
				uncle->color = BLACK;
				insertBalance(grandpa);
			}
			else
			{
				if (isLeft(node))
				{
					rightRotate(parent);
				}
				leftRotate(grandpa);
				parent->color = RED;
				grandpa->color = BLACK;
				node->color = RED;
			}
		}
		root->color = BLACK;
	}
}

template <typename K, typename V>
void rbmap<K, V>::swapKVC(Node* n1, Node* n2)
{
	swapKV(n1, n2);
	bool n1c = n1->color;
	n1->color = n2->color;
	n2->color = n1c;
}

template <typename K, typename V>
void rbmap<K, V>::swapKV(Node* n1, Node* n2)
{
	K k = n1->key;
	n1->key = n2->key;
	n2->key = k;

	if (std::is_move_assignable_v<V> && std::is_move_constructible_v<V>)
	{
		V v = std::move(n1->value);
		n1->value = std::move(n2->value);
		n2->value = std::move(v);
	}
	else
	{
		V v = n1->value;
		n1->value = n2->value;
		n2->value = v;
	}
}

template <typename K, typename V>
void rbmap<K, V>::rightRotate(Node* node)
{
	swapKVC(node, node->left);
	Node* n = node->right;
	node->right = node->left;
	node->left = node->left->left;
	node->right->left = node->right->right;
	node->right->right = n;
	node->parent = node->right->parent->parent;
	node->right->parent = node;
	if (!isNull(node->left))
	{
		node->left->parent = node;
	}
}

template <typename K, typename V>
void rbmap<K, V>::leftRotate(Node* node)
{
	swapKVC(node, node->right);
	Node* n = node->left;
	node->left = node->right;
	node->right = node->right->right;
	node->left->right = node->left->left;
	node->left->left = n;
	node->parent = node->left->parent->parent;
	node->left->parent = node;
	if (!isNull(node->right))
	{
		node->right->parent = node;
	}
}
