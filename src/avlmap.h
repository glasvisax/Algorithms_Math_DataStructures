#pragma once

#include <deque>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <stack>
namespace graph::tree
{
	template <typename K, typename V>
	class avlmap
	{
		struct Node
		{
			Node(K k, const V& v)
			{
				key = k;
				value = v;
			}

			K	   key;
			V	   value;
			Node*  right = nullptr;
			Node*  left = nullptr;
			int	   height = -1;
			size_t arr_index = -1;
		}* root = nullptr;

		std::deque<Node>   nodes_store;
		std::stack<size_t> free_indexes;

	public:
		// Prints the tree structure to the console
		void printTree() const
		{
			printTreeRec(root, 0);
			std::cout << "\n\n";
		}

		// Adding a Node with a Key and a Value
		void insert(K key, const V& value);

		// Finding a value by key (constant)
		V find(K key) const;

		// Removing a value by key
		void remove(K key);

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

	private:
		Node* create(K key, const V& value);

		Node* getMaxFrom(Node* root);

		Node* getMinFrom(Node* root);

		void rightRotate(Node* node);

		void leftRotate(Node* node);

		int tieNode(Node* root, K key, const V& value);

		void balance(Node* root);

		void updateHeight(Node* node);

		int balanceFactor(Node* node);

		void swapKV(Node* n1, Node* n2);

		Node* findRec(K key, Node* root) const;

		void removeRec(K key, Node*& root);

		void eraseNode(Node* node);

		void printTreeRec(Node* node, int indent) const;
	};

	template <typename K, typename V>
	V& avlmap<K, V>::find(K key)
	{
		if (root == nullptr)
			throw std::exception("root is nullptr");
		Node* n = findRec(key, root);
		if (n->key != key)
			throw std::exception("element does not exist");
		return n->value;
	}

	template <typename K, typename V>
	V avlmap<K, V>::find(K key) const
	{
		if (root == nullptr)
			throw std::exception("root is nullptr");
		Node* n = findRec(key, root);
		if (n->key != key)
			throw std::exception("element does not exist");
		return n->value;
	}
	template <typename K, typename V>
	void avlmap<K, V>::remove(K key)
	{
		if (root == nullptr)
			throw std::exception("root = nullptr");
		removeRec(key, root);
	}

	template <typename K, typename V>
	void avlmap<K, V>::insert(K key, const V& value)
	{
		if (root == nullptr)
		{
			root = create(key, value);
			return;
		}
		tieNode(root, key, value);
		balance(root);
	}

	template <typename K, typename V>
	void avlmap<K, V>::eraseNode(Node* node)
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
	void avlmap<K, V>::printTreeRec(Node* node, int indent) const
	{
		if (node != nullptr)
		{
			if (node->right)
			{
				printTreeRec(node->right, indent + 4);
			}
			if (indent)
			{
				std::cout << std::setw(indent) << ' ';
			}
			if (node->right)
				std::cout << " /\n"
						  << std::setw(indent) << ' ';
			std::cout << node->key << "\n ";
			if (node->left)
			{
				std::cout << std::setw(indent) << ' ' << " \\\n";
				printTreeRec(node->left, indent + 4);
			}
		}
	}

	template <typename K, typename V>
	typename avlmap<K, V>::Node* avlmap<K, V>::create(K key, const V& value)
	{
		Node*  current;
		size_t current_index;
		if (free_indexes.size() == 0)
		{
			nodes_store.emplace_back(key, value);
			current = &nodes_store.back();
			current_index = nodes_store.size() - 1;
		}
		else
		{
			auto free_addr = &*(nodes_store.begin() + free_indexes.top());
			current = new (free_addr) Node(key, value);
			current_index = free_indexes.top();
			free_indexes.pop();
		}
		current->arr_index = current_index;
		current->height = 0;
		return current;
	}

	template <typename K, typename V>
	typename avlmap<K, V>::Node* avlmap<K, V>::getMaxFrom(Node* root)
	{
		if (root == nullptr)
			return nullptr;
		return root->right == nullptr ? root : getMaxFrom(root->right);
	}

	template <typename K, typename V>
	typename avlmap<K, V>::Node* avlmap<K, V>::getMinFrom(Node* root)
	{
		if (root == nullptr)
			return nullptr;
		return root->left == nullptr ? root : getMaxFrom(root->left);
	}

	template <typename K, typename V>
	void avlmap<K, V>::rightRotate(Node* node)
	{
		swapKV(node, node->left);
		Node* n = node->right;
		node->right = node->left;
		node->left = node->left->left;
		node->right->left = node->right->right;
		node->right->right = n;
		updateHeight(node->right);
		updateHeight(node);
	}

	template <typename K, typename V>
	void avlmap<K, V>::leftRotate(Node* node)
	{
		swapKV(node, node->right);
		Node* n = node->left;
		node->left = node->right;
		node->right = node->right->right;
		node->left->right = node->left->left;
		node->left->left = n;
		updateHeight(node->left);
		updateHeight(node);
	}

	template <typename K, typename V>
	int avlmap<K, V>::tieNode(Node* root, K key, const V& value)
	{
		if (root->key == key)
		{
			return -1;
		}
		Node*& current = key > root->key ? root->right : root->left;
		if (current == nullptr)
		{
			current = create(key, value);
			root->height = std::max(1, root->height);
			return 1;
		}
		else
		{
			int new_height = tieNode(current, key, value);
			if (new_height == -1)
			{
				return -1;
			}
			++new_height;
			root->height = std::max(new_height, root->height);

			balance(root);

			return root->height;
		}
	}

	template <typename K, typename V>
	void avlmap<K, V>::balance(Node* root)
	{
		if (root == nullptr)
			return;
		int bf = balanceFactor(root);
		if (bf == 2)
		{
			if (balanceFactor(root->right) == -1)
			{
				rightRotate(root->right);
			}
			leftRotate(root);
		}
		else if (bf == -2)
		{
			if (balanceFactor(root->left) == 1)
			{
				leftRotate(root->left);
			}
			rightRotate(root);
		}
	}

	template <typename K, typename V>
	void avlmap<K, V>::updateHeight(Node* node)
	{
		int lh = node->left == nullptr ? -1 : node->left->height;
		int rh = node->right == nullptr ? -1 : node->right->height;
		node->height = std::max(lh, rh) + 1;
	}

	template <typename K, typename V>
	int avlmap<K, V>::balanceFactor(Node* node)
	{
		int left_height = node->left == nullptr ? -1 : node->left->height;
		int right_height = node->right == nullptr ? -1 : node->right->height;
		return right_height - left_height;
	}

	template <typename K, typename V>
	void avlmap<K, V>::swapKV(Node* n1, Node* n2)
	{
		K k = n1->key;
		n1->key = n2->key;
		n2->key = k;
		V v = std::move(n1->value);
		n1->value = std::move(n2->value);
		n2->value = std::move(v);
	}

	template <typename K, typename V>
	typename avlmap<K, V>::Node* avlmap<K, V>::findRec(K key, Node* root) const
	{
		if (root == nullptr)
			return nullptr;
		if (key == root->key)
			return root;
		if (key > root->key)
		{
			return root->right == nullptr ? root : findRec(key, root->right);
		}
		else
		{
			return root->left == nullptr ? root : findRec(key, root->left);
		}
	}

	template <typename K, typename V>
	void avlmap<K, V>::removeRec(K key, Node*& root)
	{
		if (root == nullptr)
		{
			return;
		}

		if (key == root->key)
		{
			Node* current;
			if (root->right == nullptr)
			{
				if (root->left == nullptr)
				{
					eraseNode(root);
					root = nullptr;
					return;
				}
				current = root->left;
			}
			else if (root->left == nullptr)
			{
				current = root->right;
			}
			else
			{
				Node* lmax = getMaxFrom(root->left);
				swapKV(lmax, root);
				removeRec(key, root->left);
				return;
			}

			Node* prev = root;
			root = current;
			eraseNode(prev);
			return;
		}

		Node*& current = key > root->key ? root->right : root->left;
		if (current == nullptr)
		{
			return;
		}
		removeRec(key, current);
		updateHeight(root);
		balance(root);
		return;
	}
} // namespace graph::tree