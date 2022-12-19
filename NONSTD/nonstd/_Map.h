#pragma once
namespace nonstd {
	template<class Tx, class Ty>
	class RBTree<nonstd::pair<Tx,Ty>>
	{
	public:
		using T = nonstd::pair<Tx, Ty>;
		RBTree() :root(nullptr) {}
		~RBTree()
		{
			if (root)
			{
				Destroy(root);
			}
		}
		void print() {
			print(root);
		}
		void print(RBNode<T>* p) {
			if (!p) return;
			print(p->left);
			print(p->right);
		}
		class iterator{
		public:
			using value_type = nonstd::pair<Tx, Ty>;
			using pointer = nonstd::pair<Tx, Ty>*;
			using reference = nonstd::pair<Tx, Ty>&;
			using iterator_category = nonstd::forward_iterator_tag;
			using difference_type = nonstd::ptrdiff_t;
			using nodeptr = RBNode<value_type>*;
			iterator(nodeptr node = nullptr) : current(node) {}
			reference operator*() const { return current->val; }
			pointer operator->() const { return &current->val; }
			operator nodeptr() const { return current; }
			iterator operator++() { return Next(current);}
			iterator operator++(int) { return ++(*this); }
			iterator& operator--() {return Prev(current);}
			iterator operator--(int) {return --(*this);}
			bool operator==(const iterator& other) const { return current == other.current; }
			bool operator!=(const iterator& other) const { return current != other.current; }
		private:
			nodeptr current;
			decltype(auto) Next(nodeptr& node) {
				if (node == nullptr) return *this;
				if (node->right) {
					node = node->right;
					while (node->left) node = node->left;
				}else {
					auto parent = node->parent;
					while (parent && node == parent->right) {
						node = parent;
						parent = parent->parent;
					}
					node = parent;
				}
				return node;
			}
			decltype(auto) Prev(nodeptr& node) {
				if (node == nullptr) return *this;
				if (node->left) {
					node = node->left;
					while (node->right) node = node->right;
				}
				else {
					auto parent = node->parent;
					while (parent && node == parent->left) {
						node = parent;
						parent = parent->parent;
					}
					node = parent;
				}
				return node;
			}
		};
		iterator find(const Tx& v1) {
			RBNode<T>* p = root;
			RBNode<T>* node = nullptr;
			while (p)
			{
				if (p->val.first == v1)
				{
					node = p;
					break;
				}
				else if (p->val.first < v1)
				{
					node = p;
					p = p->right;
				}
				else
				{
					node = p;
					p = p->left;
				}
			}
			return iterator(node);
		}
		iterator operator[](const Tx& v1) {return find(v1);}
		iterator insert(const T& v1) {
			RBNode<T>* newNode = new RBNode<T>(v1);
			RBNode<T>* parent = find(v1.first);
			if (parent == nullptr)
			{//�����Ϊ�գ���ǰ����Ľڵ�Ϊ���ڵ㡣����󽫸���ɫ��Ϊ��
				root = newNode;
				root->color = _rb_black_node;
				return newNode;
			}
			if (parent->val == v1)//v1�Ѿ����ں�����С����ٲ���
				return newNode;

			if (v1.first < parent->val.first)
			{
				parent->left = newNode;
			}else{
				parent->right = newNode;
			}
			newNode->parent = parent;
			InsertReBalance(newNode);
			//����endNode
			
			return newNode;
		}
		iterator erase(iterator _p) {
			RBNode<T>* nextNode = nullptr;
			RBNode<T>* p = _p;
			if (!p){
				return iterator(p);
			}
			if (p->left && p->right)
			{
				RBNode<T>* tempNode = p->right;
				while (tempNode)
				{//�������еĺ�̽ڵ�
					nextNode = tempNode;
					tempNode = tempNode->left;
				}
				p->val = nextNode->val;
				p = nextNode;
			}
			if (p->left)
			{
				//ֱ���ú�̽ڵ��ֵ�滻
				RBNode<T>* temp = p->left;
				p->val = temp->val;
				p->left = nullptr;
				delete temp;
				return iterator(temp);
			}
			else if (p->right)
			{
				//ֱ���ú�̽ڵ��ֵ�滻
				RBNode<T>* temp = p->right;
				p->val = temp->val;
				p->right = nullptr;
				delete temp;
				return iterator(temp);
			}
			else
			{
				//���������������ڣ���Ҫ����ɾ�������㷨
				DeleteReblance(p);
				if (p == root)
				{
					root = nullptr;
				}
				else if (p == p->parent->left)
				{//���ڵ��ָ������Ҫ�޸�
					p->parent->left = nullptr;
				}
				else if (p == p->parent->right)
				{//���ڵ��ָ������Ҫ�޸�
					p->parent->right = nullptr;
				}
				delete p;
				return iterator(p);
			}
		}
		void DeleteReblance(RBNode<T>* node) {
			RBNode<T>* parent = nullptr;
			RBNode<T>* other = nullptr;
			while (node->color == _rb_black_node && node->parent)
			{
				parent = node->parent;
				if (node == parent->left)
				{
					other = parent->right;
					if (other->color == _rb_red_node)
					{//����1�ֵܽڵ�Ϊ��
						parent->color = _rb_red_node;
						other->color = _rb_black_node;
						_rbtree_rotate_left(parent);
						other = parent->right;
					}
					if ((other->left == nullptr || other->left->color == _rb_black_node)
						&& (other->right == nullptr || other->left->color == _rb_black_node))
					{//����2�ֵ�Ϊ�ڣ����ֵܵ���������ҲΪ��
						other->color = _rb_red_node;
						node = parent;
						continue;
					}
					if (other->right == nullptr || other->right->color == _rb_black_node)
					{//����3�ֵܽڵ���Һ���Ϊ�ڣ���Ϊ��
						other->left->color = _rb_black_node;//��ʱ����һ����������ɫΪ�죬���������Ͳ�������������
						other->color = _rb_red_node;
						_rbtree_rotate_right(other);
						other = parent->right;
					}
					//����4�ֵܽڵ���Һ���Ϊ��
					other->right->color = _rb_black_node;
					other->color = parent->color;
					parent->color = _rb_black_node;
					_rbtree_rotate_left(parent);
					break;
				}
				else
				{
					other = parent->left;
					if (other->color == _rb_red_node)
					{//����1�ֵܽڵ�Ϊ��
						parent->color = _rb_red_node;
						other->color = _rb_black_node;
						_rbtree_rotate_right(parent);
						other = parent->left;
					}
					if ((other->left == nullptr || other->left->color == _rb_black_node)
						&& (other->right == nullptr || other->left->color == _rb_black_node))
					{//����2�ֵ�Ϊ�ڣ����ֵܵ���������ҲΪ��
						other->color = _rb_red_node;
						node = parent;
						continue;
					}
					if (other->left == nullptr || other->left->color == _rb_black_node)
					{//����3�ֵܽڵ���Һ���Ϊ�ڣ���Ϊ��
						other->right->color = _rb_black_node;//��ʱ����һ����������ɫΪ�죬���������Ͳ�������������
						other->color = _rb_red_node;
						_rbtree_rotate_left(other);
						other = parent->left;
					}
					//����4�ֵܽڵ���Һ���Ϊ��
					other->left->color = _rb_black_node;
					other->color = parent->color;
					parent->color = _rb_black_node;
					_rbtree_rotate_right(parent);
					break;
				}
			}
			node->color = _rb_black_node;
		}
		void Destroy(RBNode<T>* p) {
			if (p->left)
			{
				Destroy(p->left);
			}
			if (p->right)
			{
				Destroy(p->right);
			}
			delete p;
		}
		void InsertReBalance(RBNode<T>* node) {
			RBNode<T>* parent = node->parent;
			RBNode<T>* grandParent = nullptr;
			while (parent && parent->color == _rb_red_node)
			{
				grandParent = parent->parent;
				if (parent == grandParent->left)
				{//���ڵ�Ϊ�游�ڵ�������
					RBNode<T>* uncle = grandParent->right;
					if (uncle && uncle->color == _rb_red_node)
					{//����1 ���ڵ�����ڵ㶼Ϊ��
						//��������������ڣ��游��ڡ��游��Ϊ�µĵ�ǰ�ڵ����½����㷨
						parent->color = _rb_black_node;
						uncle->color = _rb_black_node;
						grandParent->color = _rb_red_node;
						node = grandParent;
						parent = grandParent->parent;
					}
					else
					{
						if (node == parent->right)
						{//����2����Ϊ�ڣ���ǰ�ڵ�Ϊ�丸�ڵ�����ӽڵ�
							//����������Ը��ڵ�Ϊ����������
							//������ת��Ϊ����3
							node = _rbtree_rotate_left(parent);
							parent = node->parent;
							grandParent = parent->parent;
						}
						//����3��Ϊ�죬��ǰ�ڵ�Ϊ���ڵ�����ӽڵ�
						//������������ڵ��ڣ��游�ڵ��죬��
						//�游�ڵ�Ϊ���ڵ��������
						parent->color = _rb_black_node;
						grandParent->color = _rb_red_node;
						_rbtree_rotate_right(grandParent);
					}
				}
				else
				{//���ڵ�Ϊ�游�ڵ�����ӽڵ㣬�����������ͬ
					RBNode<T>* uncle = grandParent->left;
					if (uncle && uncle->color == _rb_red_node)
					{
						uncle->color = _rb_black_node;
						parent->color = _rb_black_node;
						grandParent->color = _rb_red_node;
						node = grandParent;
						parent = node->parent;
					}
					else
					{
						if (node == parent->left)
						{
							node = _rbtree_rotate_right(parent);
							parent = node->parent;
							grandParent = parent->parent;
						}
						parent->color = _rb_black_node;
						grandParent->color = _rb_red_node;
						_rbtree_rotate_left(grandParent);
					}
				}
			}
			root->color = _rb_black_node;
		}
		RBNode<T>* _rbtree_rotate_left(RBNode<T>* x) {
			RBNode<T>* y = x->right;
			if (y == nullptr)
			{
				return x;
			}
			//x���ҽڵ�Ϊy����ڵ�
			x->right = y->left;
			if (y->left)//���y����ڵ���ڣ��丸�ڵ�Ϊy
				y->left->parent = x;
			if (root == x)
			{//xΪroot����ת��yΪ�µ�root���ڵ�
				root = y;
			}
			else
			{
				if (x == x->parent->left)
				{//���xΪ�丸�ڵ�����ӽڵ㡣
					//x�ĸ��ڵ�������ӽڵ�Ϊy
					x->parent->left = y;
				}
				else
				{
					x->parent->right = y;
				}
				//y�ĸ��ڵ�Ϊx�ĸ��ڵ�
				y->parent = x->parent;
			}
			//y�����ӽڵ�Ϊx
			y->left = x;
			//x�ĸ��ڵ�Ϊy
			x->parent = y;
			return x;
		}
		RBNode<T>* _rbtree_rotate_right(RBNode<T>* x) {
			RBNode<T>* y = x->left;
			if (y == nullptr)
			{
				return x;
			}
			x->left = y->right;
			if (y->right)
				y->right->parent = x;
			if (root == x)
			{
				root = y;
			}
			else
			{
				if (x == x->parent->left)
				{
					x->parent->left = y;
				}
				else
				{
					x->parent->right = y;
				}
				y->parent = x->parent;
			}
			y->right = x;
			x->parent = y;
			return x;
		}
		iterator begin() { return iterator(Maxmiun<false>(root)); }
		iterator end(){ return iterator(Maxmiun<true>(root));}
	private:
		RBNode<T>* root;
		template<bool right> RBNode<T>* Maxmiun(RBNode<T>* _node) {
			auto node = _node;
			if constexpr (right) {
				while (node&&node->right) node = node->right;
			}else {
				while (node&&node->left) node = node->left;
			}
			return node;
		}
	};
	template<class Tx, class Ty>
	using map = RBTree<nonstd::pair<Tx, Ty>>;
}
