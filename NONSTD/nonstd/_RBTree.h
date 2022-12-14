#pragma once
namespace nonstd {
    template<class T, class Allocator = allcator<T>>
    class RBTreeNode {
    public:
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;
        using iterator = T*;
        using const_iterator = const T*;
        using difference_type = nonstd::ptrdiff_t;
        using size_type = nonstd::size_t;
        using _Self = RBTreeNode<T, Allocator>;
        RBTreeNode() : m_data(nullptr), m_left(nullptr), m_right(nullptr), m_parent(nullptr), m_isRed(false) {}
        RBTreeNode(const value_type& value) : m_data(Allocator::allocate(1)), m_left(nullptr), m_right(nullptr), m_parent(nullptr), m_isRed(false) {
            Allocator::construct(m_data, value);
        }
        RBTreeNode(value_type&& value) : m_data(Allocator::allocate(1)), m_left(nullptr), m_right(nullptr), m_parent(nullptr), m_isRed(false) {
            Allocator::construct(m_data, nonstd::forward<value_type>(value));
        }
        ~RBTreeNode() {
            Allocator::destroy(m_data);
            Allocator::deallocate(m_data);
        }

        reference operator*() {
            return *m_data;
        }
        const_reference operator*() const {
            return *m_data;
        }
        //指针运算
		iterator operator->() {
			return m_data;
		}
        const RBTreeNode* operator->() const {
            return m_data;
        }
        //next
		_Self* next() {
			if (m_right) {
				_Self* node = m_right;
				while (node->m_left) {
					node = node->m_left;
				}
				return node;
			}
			else {
				_Self* node = this;
				while (node->m_parent && node->m_parent->m_right == node) {
					node = node->m_parent;
				}
				return node->m_parent;
			}
		}
        //clear
		void clear() {
			if (m_left) {
				m_left->clear();
				//用分配器 delete m_left;
                Allocator::destroy((T*)m_left);
                Allocator::deallocate((T*)m_left);
				m_left = nullptr;
			}
			if (m_right) {
				m_right->clear();
				//用分配器 delete m_right;
                Allocator::destroy((T*)m_right);
                Allocator::deallocate((T*)m_right);
				m_right = nullptr;
			}
		}
        //insert
        _Self* insert(const value_type& value) {
            if (value < *m_data) {
                if (m_left) {
                    return m_left->insert(value);
                }
                else {
                    m_left =(_Self*)Allocator::allocate(1);
                    Allocator::construct((T*)m_left, value);
                    m_left->m_parent = this;
                    return m_left;
                }
            }
            else {
                if (m_right) {
                    return m_right->insert(value);
                }
                else {
                    m_right = (_Self*)Allocator::allocate(1);
                    Allocator::construct((T*)m_right, value);
                    m_right->m_parent = this;
                    return m_right;
                }
            }
        }
        //leftMost
		_Self* leftMost() {
			_Self* node = this;
			while (node->m_left) {
				node = node->m_left;
			}
			return node;
		}
		//rightMost
		_Self* rightMost() {
			_Self* node = this;
			while (node->m_right) {
				node = node->m_right;
			}
			return node;
		}

        iterator begin() { return m_data; }
        const_iterator begin() const { return m_data; }
        const_iterator cbegin() const { return m_data; }
        iterator end() { return m_data + 1; }
        const_iterator end() const { return m_data + 1; }
        const_iterator cend() const { return m_data + 1; }

        RBTreeNode* left() { return m_left; }
        RBTreeNode* right() { return m_right; }
        RBTreeNode* parent() { return m_parent; }
        bool isRed() { return m_isRed; }

        void setLeft(RBTreeNode* left) { m_left = left; }
        void setRight(RBTreeNode* right) { m_right = right; }
        void setParent(RBTreeNode* parent) { m_parent = parent; }
        void setRed(bool isRed) { m_isRed = isRed; }

    private:
        value_type* m_data;
        RBTreeNode* m_left;
        RBTreeNode* m_right;
        RBTreeNode* m_parent;
		bool m_isRed;
    };
    
    template<class KeyType,class ValueType, class _PairType = pair<KeyType, ValueType>, class Compare = nonstd::less<KeyType>>
    class RBTree {
        using _Allcator = allcator<_PairType>;
		using _Node = RBTreeNode<_PairType>;
        using _NodePtr=_Node*;
        using _NodeRef=_Node&;
        using _NodeConstRef=const _Node&;
        using _NodeConstPtr=const _Node*;
        using _Self=RBTree<KeyType, ValueType, _PairType, Compare>;
        class _Iterator {
        public:
            using value_type = _Node;
            using difference_type = nonstd::ptrdiff_t;
            using pointer = _Node*;
            using reference = _Node&;
            using const_reference = const _Node&;
            using const_pointer = const _Node*;
            using iterator = _Node*;
            using const_iterator = const _Node*;
            _Iterator() : m_node(nullptr) {}
            _Iterator(_NodePtr node) : m_node(node) {}
            _Iterator(const _Iterator& other) : m_node(other.m_node) {}
            _Iterator(_Iterator&& other) : m_node(other.m_node) {
                other.m_node = nullptr;
            }
            _Iterator& operator=(const _Iterator& other) {
                m_node = other.m_node;
                return *this;
            }
            _Iterator& operator=(_Iterator&& other) {
                m_node = other.m_node;
                other.m_node = nullptr;
                return *this;
            }
            ~_Iterator() {}
           //解引用
			reference operator*() {
				return *m_node;
			}
            const_reference operator*() const {
                return *m_node;
            }
            value_type operator->() {
                return *m_node;
            }
            const pointer operator->() const {
                return m_node;
            }
            _Iterator& operator++() {
                m_node = m_node->next();
                return *this;
            }
            _Iterator operator++(int) {
                _Iterator tmp = *this;
                m_node = m_node->next();
                return tmp;
            }
            _Iterator& operator--() {
                m_node = m_node->prev();
                return *this;
            }
            _Iterator operator--(int) {
                _Iterator tmp = *this;
                m_node = m_node->prev();
                return tmp;
            }
            bool operator==(const _Iterator& other) const {
                return m_node == other.m_node;
            }
            bool operator!=(const _Iterator& other) const {
                return m_node != other.m_node;
            }
        private:
            _NodePtr m_node;
        };
    public:
        using key_type = KeyType;
        using mapped_type = ValueType;
        using value_type = _PairType;
        using key_compare = Compare;
        using value_compare = Compare;
        using allocator_type = _Allcator;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using iterator = _Iterator;
        using const_iterator = const _Iterator;
        using difference_type = nonstd::ptrdiff_t;
        using size_type = nonstd::size_t;
        RBTree() : m_root(nullptr), m_size(0) {}
        RBTree(const RBTree& other) : m_root(nullptr), m_size(0) {
            nonstd::copy(other);
        }
        RBTree(RBTree&& other) : m_root(nullptr), m_size(0) {
            nonstd::move(other);
        }
        RBTree& operator=(const RBTree& other) {
            nonstd::copy(other);
            return *this;
        }
        RBTree& operator=(RBTree&& other) {
            nonstd::move(other);
            return *this;
        }
        ~RBTree() {
            clear();
        }
        iterator begin() {
            return iterator(m_root->leftMost());
        }
        const_iterator begin() const {
            return const_iterator(m_root->leftMost());
        }
        const_iterator cbegin() const {
            return const_iterator(m_root->leftMost());
        }
        iterator end() {
            return iterator(m_root);
        }
        const_iterator end() const {
            return const_iterator(m_root);
        }
        const_iterator cend() const {
            return const_iterator(m_root);
        }
        bool empty() const {
            return m_size == 0;
        }
        size_type size() const {
            return m_size;
        }
        size_type max_size() const {
            return _Allcator::max_size();
        }
        void clear() {
            if (m_root) {
                m_root->clear();
                m_root = nullptr;
                m_size = 0;
            }
        }
        iterator find(const key_type& key) {
            if (m_root) {
                _NodePtr node = m_root->find(key);
                if (node) {
                    return iterator(node);
                }
            }
            return end();
        }
        const_iterator find(const key_type& key) const {
            if (m_root) {
                _NodePtr node = m_root->find(key);
                if (node) {
                    return const_iterator(node);
                }
            }
            return cend();
        }
        size_type count(const key_type& key) const {
            if (m_root) {
                return m_root->count(key);
            }
            return 0;
        }
        iterator lower_bound(const key_type& key) {
            if (m_root) {
                _NodePtr node = m_root->lower_bound(key);
                if (node) {
                    return iterator(node);
                }
            }
            return end();
        }
        const_iterator lower_bound(const key_type& key) const {
            if (m_root) {
                _NodePtr node = m_root->lower_bound(key);
                if (node) {
                    return const_iterator(node);
                }
            }
            return cend();
        }
        iterator upper_bound(const key_type& key) {
            if (m_root) {
                _NodePtr node = m_root->upper_bound(key);
                if (node) {
                    return iterator(node);
                }
            }
            return end();
        }
        const_iterator upper_bound(const key_type& key) const {
            if (m_root) {
                _NodePtr node = m_root->upper_bound(key);
                if (node) {
                    return const_iterator(node);
                }
            }
            return cend();
        }
        nonstd::pair<iterator, iterator> equal_range(const key_type& key) {
            return nonstd::make_pair(lower_bound(key), upper_bound(key));
        }
        nonstd::pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
            return nonstd::make_pair(lower_bound(key), upper_bound(key));
        }
        //重载[]操作符
        mapped_type& operator[](const key_type& key) {
            if (m_root) {
                _NodePtr node = m_root->find(key);
                if (node) {
                    return node->value.second;
                }
            }
            return insert(nonstd::make_pair(key, mapped_type())).first->second;
        }
        nonstd::pair<iterator, bool> insert(const value_type& value) {
            if (m_root) {
                _NodePtr node = m_root->insert(value);
                if (node) {
                    return nonstd::make_pair(iterator(node), true);
                }
            } else {
                m_root = new _Node(value);
                m_size = 1;
                return nonstd::make_pair(iterator(m_root), true);
            }
            return nonstd::make_pair(end(), false);
        }
        nonstd::pair<iterator, bool> insert(value_type&& value) {
            if (m_root) {
                _NodePtr node = m_root->insert(nonstd::move(value));
                if (node) {
                    return nonstd::make_pair(iterator(node), true);
                }
            } else {
                m_root = new _Node(nonstd::move(value));
                m_size = 1;
                return nonstd::make_pair(iterator(m_root), true);
            }
            return nonstd::make_pair(end(), false);
        }
        iterator insert(const_iterator hint, const value_type& value) {
            return insert(value).first;
        }
        iterator insert(const_iterator hint, value_type&& value) {
            return insert(nonstd::move(value)).first;
        }
        template <class InputIterator>
        void insert(InputIterator first, InputIterator last) {
            for (; first != last; ++first) {
                insert(*first);
            }
        }
        void insert(nonstd::initializer_list<value_type> ilist) {
            insert(ilist.begin(), ilist.end());
        }
        template <class... Args>
        nonstd::pair<iterator, bool> emplace(Args&&... args) {
            return insert(value_type(nonstd::forward<Args>(args)...));
        }
        template <class... Args>
        iterator emplace_hint(const_iterator hint, Args&&... args) {
            return insert(hint, value_type(nonstd::forward<Args>(args)...));
        }
        iterator erase(const_iterator position) {
            if (m_root) {
                _NodePtr node = m_root->erase(position.m_node);
                if (node) {
                    return iterator(node);
                }
            }
            return end();
        }
        iterator erase(const_iterator first, const_iterator last) {
            if (m_root) {
                _NodePtr node = m_root->erase(first.m_node, last.m_node);
                if (node) {
                    return iterator(node);
                }
            }
            return end();
        }
        size_type erase(const key_type& key) {
            if (m_root) {
                return m_root->erase(key);
            }
            return 0;
        }
        void swap(_Self& other) {
            nonstd::swap(m_root, other.m_root);
            nonstd::swap(m_size, other.m_size);
        }
        void swap(_Self&& other) {
            nonstd::swap(m_root, other.m_root);
            nonstd::swap(m_size, other.m_size);
        }
        void merge(_Self& other) {
            if (m_root) {
                m_root->merge(other.m_root);
            } else {
                m_root = other.m_root;
            }
            m_size += other.m_size;
            other.m_root = nullptr;
            other.m_size = 0;
        }
        void merge(_Self&& other) {
            if (m_root) {
                m_root->merge(other.m_root);
            } else {
                m_root = other.m_root;
            }
            m_size += other.m_size;
            other.m_root = nullptr;
            other.m_size = 0;
        }
    private:
        _NodePtr m_root;
        size_type m_size;
    };
}