#include <iostream>

template <class myType, class compare = std::less<myType>>
class BST {
	
	private:
		size_t node_counter = 0;
		
		struct Node {
			myType key;
			Node *left, *right;
			Node() {}
			Node(myType key)
			:key(key)
			{
				left = right = nullptr;
			}
					
			~Node()
			{
				delete left;
				delete right;
			}
		};

		Node* root = nullptr;	//top of the tree
		
		Node* search(Node* node, myType key, compare comp = compare())
		{
			if (node == nullptr || node->key == key)
			{
				return node;
			}
			return search(comp(key, node->key)? node->left : node->right, key);
		}
	
		Node* insert(Node* node, myType key, compare comp = compare())
		{
			if (node == nullptr)
			{
				return new Node(key);
			}
			if (comp(key, node->key)) {
				node->left = insert(comp(key, node->key)? node->left : node->right, key);
			}
			else
				node->right = insert(comp(key, node->key)? node->left : node->right, key);
			return node;
		}
	
		Node* minimum(Node* node)
		{
			if (node->left == nullptr)
			{
				return node;
			}
			return minimum(node->left);
		}
		
		Node* maximum(Node* node)
		{
			if (node->right == nullptr)
			{
				return node;
			}
			return minimum(node->right);
		}
	
		Node* remove(Node* node, myType key, compare comp = compare())
		{
			if (node == nullptr)
			{
				return node;
			}
			
			if (node->key != key) {
				
				if (comp(key, node->key)) {
					node->left = remove(comp(key, node->key)? node->left : node->right, key);
				}
				
				else
					node->right = remove(comp(key, node->key)? node->left : node->right, key);
			}
			
			else {			
				if (!node->left || !node->right) // count < 2
				{
					node = remove(node->left != nullptr? node->left : node->right, key);
				}
				
				else
				{
					Node* min_from_right_subtree = minimum(node->right);
					node->key = min_from_right_subtree->key;
					node->right = remove(node->right, node->key);
				}
			}
			return node;
		}
		
	public:
		BST() {}
		BST(const BST & other) {
			*this = other;
		}
		
		class const_iterator {
			private:
				const Node *cpointer = nullptr;
				Node *root = nullptr;
			public:
				const_iterator (const Node *node, Node *top)
				: cpointer(node), root(top) {}
				
				myType operator* () {
					return *(cpointer->key);
				}
				
				bool operator== (const_iterator const &other) {
					return other.cpointer == this->cpointer;
				}
				
				bool operator!= (const_iterator const &other) {
					return other.cpointer == this->cpointer;
				}
				
				const_iterator operator++ () {
					compare comp = compare();
					Node* current = root;
					Node* possible = nullptr;
	
					while (current != nullptr)
					{
						if (comp(cpointer->key, current->key))
						{
							possible = current;
							current = current->left;
						}
						else
						{
							current = current->right;
						}
					}
					
					if (possible == nullptr)
					{
						throw std::exception();
					}
					
					this->pointer = possible;
					return *this;					
				}
				
				const_iterator operator-- () {
					compare comp = compare();
					Node* current = root;
					Node* possible = nullptr;
	
					while (current != nullptr)
					{
						if (comp(current->key, cpointer->key))
						{
							possible = current;
							current = current->right;
						}
						else
						{
							current = current->left;
						}
					}
					
					if (possible == nullptr)
					{
						throw std::exception();
					}
					
					this->pointer = possible;
					return *this;					
				}
		};
		
		class iterator {
			private:
				Node *pointer = nullptr;
				Node *root = nullptr;
			public:
				iterator() {}
				iterator (Node *node, Node *top)
				: pointer(node), root(top) {}
				
				myType operator* () {
					return pointer->key;
				}
				
				bool operator== (iterator const &other) {
					return &other.pointer == &this->pointer;
				}
				
				bool operator!= (iterator const &other) {
					return other.pointer != this->pointer;
				}

				iterator operator++ () {
					compare comp = compare();
					Node* current = root;
					Node* possible = nullptr;
	
					while (current != nullptr)
					{
						if (comp(pointer->key, current->key))
						{
							possible = current;
							current = current->left;
						}
						else
						{
							current = current->right;
						}
					}
					
					if (possible == nullptr)
					{
						throw std::exception();
					}
					
					this->pointer = possible;
					return *this;
				}
				
				iterator operator-- () {
					compare comp = compare();
					Node* current = root;
					Node* possible = nullptr;
	
					while (current != nullptr)
					{
						if (comp(current->key, pointer->key))
						{
							possible = current;
							current = current->right;
						}
						else
						{
							current = current->left;
						}
					}
					
					if (possible == nullptr)
					{
						throw std::exception();
					}
					
					this->pointer = possible;
					return *this;
				}
		};
		
		const_iterator cbegin() {
			const_iterator it(minimum(root), root);
			return it;
		}
		
		const_iterator cend() {
			const_iterator it(maximum(root), root);
			return it;
		}
		
		iterator begin() {
			iterator it(minimum(root), root);
			return it;
		}
		
		iterator end() {
			iterator it(maximum(root), root);
			return it;
		}
		
		iterator find(const myType &key)
		{
			return iterator(search(root, key), root);
		}
		
		iterator insert(const myType &key)
		{
			if ((search(root, key) != nullptr) == false) {
				root = insert(root, key);
				++node_counter;
			}
			return find(key);
		}
	
		iterator remove(const myType &key)
		{
			if ((search(root, key) != nullptr)) {
				root = remove(root, key);
				--node_counter;
			}
			return iterator(root, root);
		}
		
		bool empty () {
			return root == nullptr;
		}
		
		size_t size () {
			return node_counter;
		}
		
		template <class InputIt>	
		void assign (InputIt first, InputIt last) {
			Node *assign_tree = nullptr;
			if (*(first) > *(last))
				while (first != last) {
					assign_tree = insert(assign_tree, *(first));
					--first;
				}
			else if (*(first) < *(last))
				while (first != last) {
					assign_tree = insert(assign_tree, *(first));
					++first;
				}
			assign_tree = insert(assign_tree, *(first));
				
			this->root = assign_tree;
		}
};

int main() {
	return 0;
}
