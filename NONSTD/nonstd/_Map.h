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

			std::cout << "节点: " << p->val.first << " ";
			if (p->left)
			{
				std::cout << "left:" << p->left->val.first << "->color:" << p->left->color << " ";
			}
			if (p->right)
			{
				std::cout << "right:" << p->right->val.first << "->color:" << p->right->color << " ";
			}
			std::cout << std::endl << std::endl;

			print(p->left);
			print(p->right);
		}
		RBNode<T>* find(const Tx& v1) {
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
			return node;
		}
		bool insert(const T& v1) {
			RBNode<T>* newNode = new RBNode<T>(v1);
			RBNode<T>* parent = find(v1.first);
			if (parent == nullptr)
			{//红黑树为空，当前插入的节点为根节点。插入后将根颜色变为黑
				root = newNode;
				root->color = _rb_black_node;
				return true;
			}
			if (parent->val == v1)//v1已经存在红黑树中。不再插入
				return false;

			if (v1.first < parent->val.first)
			{
				parent->left = newNode;
			}else{
				parent->right = newNode;
			}
			newNode->parent = parent;
			InsertReBalance(newNode);
			return true;
		}
		void	DeleteValue(const T& v1) {
			RBNode<T>* nextNode = nullptr;
			RBNode<T>* p = find(v1);
			if (p == nullptr)
			{
				std::cout << "删除的值不存在" << std::endl;
				return;
			}
			if (p->left && p->right)
			{
				RBNode<T>* tempNode = p->right;
				while (tempNode)
				{//中序序列的后继节点
					nextNode = tempNode;
					tempNode = tempNode->left;
				}
				p->val = nextNode->val;
				p = nextNode;
			}
			if (p->left)
			{
				//直接用后继节点的值替换
				RBNode<T>* temp = p->left;
				p->val = temp->val;
				p->left = nullptr;
				delete temp;
			}
			else if (p->right)
			{
				//直接用后继节点的值替换
				RBNode<T>* temp = p->right;
				p->val = temp->val;
				p->right = nullptr;
				delete temp;
			}
			else
			{
				//左右子树都不存在，需要进入删除调整算法
				DeleteReblance(p);
				if (p == root)
				{
					root = nullptr;
				}
				else if (p == p->parent->left)
				{//父节点的指针域需要修改
					p->parent->left = nullptr;
				}
				else if (p == p->parent->right)
				{//父节点的指针域需要修改
					p->parent->right = nullptr;
				}
				delete p;
			}
		}
		void	DeleteReblance(RBNode<T>* node) {
			RBNode<T>* parent = nullptr;
			RBNode<T>* other = nullptr;
			while (node->color == _rb_black_node && node->parent)
			{
				parent = node->parent;
				if (node == parent->left)
				{
					other = parent->right;
					if (other->color == _rb_red_node)
					{//情形1兄弟节点为红
						parent->color = _rb_red_node;
						other->color = _rb_black_node;
						_rbtree_rotate_left(parent);
						other = parent->right;
					}
					if ((other->left == nullptr || other->left->color == _rb_black_node)
						&& (other->right == nullptr || other->left->color == _rb_black_node))
					{//情形2兄弟为黑，且兄弟的两个孩子也为黑
						other->color = _rb_red_node;
						node = parent;
						continue;
					}
					if (other->right == nullptr || other->right->color == _rb_black_node)
					{//情形3兄弟节点的右孩子为黑，左为红
						other->left->color = _rb_black_node;//此时左孩子一定存在且颜色为红，如果不满足就不会进入这个条件
						other->color = _rb_red_node;
						_rbtree_rotate_right(other);
						other = parent->right;
					}
					//情形4兄弟节点的右孩子为红
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
					{//情形1兄弟节点为红
						parent->color = _rb_red_node;
						other->color = _rb_black_node;
						_rbtree_rotate_right(parent);
						other = parent->left;
					}
					if ((other->left == nullptr || other->left->color == _rb_black_node)
						&& (other->right == nullptr || other->left->color == _rb_black_node))
					{//情形2兄弟为黑，且兄弟的两个孩子也为黑
						other->color = _rb_red_node;
						node = parent;
						continue;
					}
					if (other->left == nullptr || other->left->color == _rb_black_node)
					{//情形3兄弟节点的右孩子为黑，左为红
						other->right->color = _rb_black_node;//此时左孩子一定存在且颜色为红，如果不满足就不会进入这个条件
						other->color = _rb_red_node;
						_rbtree_rotate_left(other);
						other = parent->left;
					}
					//情形4兄弟节点的右孩子为红
					other->left->color = _rb_black_node;
					other->color = parent->color;
					parent->color = _rb_black_node;
					_rbtree_rotate_right(parent);
					break;
				}
			}
			node->color = _rb_black_node;
		}
		void	Destroy(RBNode<T>* p) {
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
		void	InsertReBalance(RBNode<T>* node) {
			RBNode<T>* parent = node->parent;
			RBNode<T>* grandParent = nullptr;
			while (parent && parent->color == _rb_red_node)
			{
				grandParent = parent->parent;
				if (parent == grandParent->left)
				{//父节点为祖父节点的左儿子
					RBNode<T>* uncle = grandParent->right;
					if (uncle && uncle->color == _rb_red_node)
					{//情形1 父节点与叔节点都为红
						//解决方法父与叔变黑，祖父变黑。祖父变为新的当前节点重新进入算法
						parent->color = _rb_black_node;
						uncle->color = _rb_black_node;
						grandParent->color = _rb_red_node;
						node = grandParent;
						parent = grandParent->parent;
					}
					else
					{
						if (node == parent->right)
						{//情形2，叔为黑，当前节点为其父节点的右子节点
							//解决方法：以父节点为根进行左旋
							//操作后将转换为情形3
							node = _rbtree_rotate_left(parent);
							parent = node->parent;
							grandParent = parent->parent;
						}
						//情形3父为红，当前节点为父节点的左子节点
						//解决方法：父节点变黑，祖父节点变红，以
						//祖父节点为根节点进行右旋
						parent->color = _rb_black_node;
						grandParent->color = _rb_red_node;
						_rbtree_rotate_right(grandParent);
					}
				}
				else
				{//父节点为祖父节点的右子节点，情况与上面相同
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
			//x的右节点为y的左节点
			x->right = y->left;
			if (y->left)//如果y的左节点存在，其父节点为y
				y->left->parent = x;
			if (root == x)
			{//x为root，旋转后y为新的root根节点
				root = y;
			}
			else
			{
				if (x == x->parent->left)
				{//如果x为其父节点的左子节点。
					//x的父节点的新左子节点为y
					x->parent->left = y;
				}
				else
				{
					x->parent->right = y;
				}
				//y的父节点为x的父节点
				y->parent = x->parent;
			}
			//y的左子节点为x
			y->left = x;
			//x的父节点为y
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
	private:
		RBNode<T>* root;
	};
	template<class Tx, class Ty>
	using map = RBTree<nonstd::pair<Tx, Ty>>;
}
