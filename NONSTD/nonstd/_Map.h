#pragma once
namespace nonstd {
    template<class Tx, class Ty>
    class RBTree<nonstd::pair<Tx, Ty>> {
    public:
        using node_type = nonstd::pair<Tx, Ty>;
        using key_type = Tx;
        using value_type = Ty;
        RBTree() {
            nil = new RBTreeNode<node_type>();
            nil->left = nullptr;
            nil->right = nullptr;
            nil->parent = nullptr;
            nil->nData = node_type();
            nil->color = NodeColor::BLACK;
            root = nil;
        }
        ~RBTree() {
            DeleteTree(root);
            delete nil;
            root = nullptr;
            nil = nullptr;
        }
        bool Insert(const node_type& nData) {
            auto pNewNode = CreateNode(nData);
            auto pPNewNode = nil;
            auto pTemp = root;
            while (pTemp != nil) {
                pPNewNode = pTemp;
                if (nData.first < pTemp->nData.first)
                    pTemp = pTemp->left;
                else
                    pTemp = pTemp->right;
            }

            pNewNode->parent = pPNewNode;
            if (pPNewNode == nil)
                root = pNewNode;
            else if (nData.first < pPNewNode->nData.first)
                pPNewNode->left = pNewNode;
            else
                pPNewNode->right = pNewNode;

            InsertFixup(pNewNode);
            return true;
        }
        bool Delete(RBTreeNode<node_type>* pNode) {
            if (pNode == nil) return;
            DeleteTree(pNode->left);
            DeleteTree(pNode->right);
            delete pNode;
            pNode = nullptr;
        }
        RBTreeNode<node_type>* Find(const key_type& nData) {
            auto pCurrent = root;
            while (pCurrent != nullptr) {
                if (nData == pCurrent->nData.first) {
                    return pCurrent;
                }
                else if (nData < pCurrent->nData.first) {
                    pCurrent = pCurrent->left;
                }else {
                    pCurrent = pCurrent->right;
                }
            }
            return nullptr;
        }
        RBTreeNode<node_type>* operator[](const node_type& nData) {
            return Find(nData);
        }
    private:
        void RotateLeft(RBTreeNode<node_type>* pNode) {
            auto pRNode = pNode->right;
            pNode->right = pRNode->left;
            if (pRNode->left != nil) {
                pRNode->left->parent = pNode;
                pRNode->parent = pNode->parent;
            }
            if (pNode->parent == nil) {
                root = pRNode;
            }
            else if (pNode->parent->left == pNode) {
                pNode->parent->left = pRNode;
            }
            else {
                pNode->parent->right = pRNode;
            }
            pRNode->left = pNode;
            pNode->parent = pRNode;
        }
        void RotateRight(RBTreeNode<node_type>* pNode) {
            auto pLNode = pNode->left;
            pNode->left = pLNode->right;
            if (pLNode->right != nil) {
                pLNode->right->parent = pNode;
                pLNode->parent = pNode->parent;
            }
            if (pNode->parent == nil) {
                root = pLNode;
            }
            else if (pNode->parent->left == pNode) {
                pNode->parent->left = pLNode;
            }
            else {
                pNode->parent->right = pLNode;
            }
            pLNode->right = pNode;
            pNode->parent = pLNode;
        }
        void InsertFixup(RBTreeNode<node_type>* pNode) {
            while (pNode->parent->color == NodeColor::RED) {
                if (pNode->parent == pNode->parent->parent->left) {
                    auto pUNode = pNode->parent->parent->right;
                    if (pUNode->color == NodeColor::RED) {
                        pNode->parent->color = NodeColor::BLACK;
                        pUNode->color = NodeColor::BLACK;
                        pNode->parent->parent->color = NodeColor::RED;
                        pNode = pNode->parent->parent;
                    }
                    else if (pNode == pNode->parent->right) {
                        pNode = pNode->parent;
                        RotateLeft(pNode);
                    }
                    else {
                        pNode->parent->color = NodeColor::BLACK;
                        pNode->parent->parent->color = NodeColor::RED;
                        RotateRight(pNode->parent->parent);
                    }
                }
                else {
                    auto pUNode = pNode->parent->parent->left;
                    if (pUNode->color == NodeColor::RED) {
                        pNode->parent->color = NodeColor::BLACK;
                        pUNode->color = NodeColor::BLACK;
                        pNode->parent->parent->color = NodeColor::RED;
                        pNode = pNode->parent->parent;
                    }
                    else if (pNode == pNode->parent->left) {
                        pNode = pNode->parent;
                        RotateRight(pNode);
                    }
                    else {
                        pNode->parent->color = NodeColor::BLACK;
                        pNode->parent->parent->color = NodeColor::RED;
                        RotateLeft(pNode->parent->parent);
                    }
                }
            }
            root->color = NodeColor::BLACK;
        }
        void DeleteFixup(RBTreeNode<node_type>* pNode) {
            while (pNode != root && pNode->color == NodeColor::BLACK) {
                if (pNode == pNode->parent->left) {
                    auto pBNode = pNode->parent->right;
                    if (pBNode->color == NodeColor::RED) {
                        pBNode->color = NodeColor::BLACK;
                        pNode->parent->color = NodeColor::RED;
                        RotateLeft(pNode->parent);
                        pBNode = pNode->parent->right;
                    }
                    if (pBNode->left->color == NodeColor::BLACK && pBNode->right->color == NodeColor::BLACK) {
                        pBNode->color = NodeColor::RED;
                        pNode = pNode->parent;
                    }
                    else if (pBNode->right->color == NodeColor::BLACK) {
                        pBNode->left->color = NodeColor::BLACK;
                        pBNode->color = NodeColor::RED;

                        RotateRight(pBNode);
                        pBNode = pNode->parent->right;
                    }
                    else {
                        pBNode->color = pNode->parent->color;
                        pNode->parent->color = NodeColor::BLACK;
                        pBNode->right->color = NodeColor::BLACK;

                        RotateLeft(pNode->parent);
                        pNode = root;
                    }
                }
                else {
                    auto pBNode = pNode->parent->left;
                    if (pBNode->color == NodeColor::RED) {
                        pBNode->color = NodeColor::BLACK;
                        pNode->parent->color = NodeColor::RED;
                        RotateLeft(pNode->parent);
                        pBNode = pNode->parent->left;
                    }
                    if (pBNode->left->color == NodeColor::BLACK && pBNode->right->color == NodeColor::BLACK) {
                        pBNode->color = NodeColor::RED;
                        pNode = pNode->parent;
                    }
                    else if (pBNode->left->color == NodeColor::BLACK) {
                        pBNode->right->color = NodeColor::BLACK;
                        pBNode->color = NodeColor::RED;

                        RotateRight(pBNode);
                        pBNode = pNode->parent->left;
                    }
                    else {
                        pBNode->color = pNode->parent->color;
                        pNode->parent->color = NodeColor::BLACK;
                        pBNode->left->color = NodeColor::BLACK;

                        RotateLeft(pNode->parent);
                        pNode = root;
                    }
                }
            }
            pNode->color = NodeColor::BLACK;
        }
        RBTreeNode<node_type>* CreateNode(node_type nData) {
            auto pTempNode = new RBTreeNode<node_type>();
            pTempNode->left = nil;
            pTempNode->right = nil;
            pTempNode->parent = nil;
            pTempNode->nData = nData;
            pTempNode->color = NodeColor::RED;
            return pTempNode;
        }
        RBTreeNode<node_type>* DeleteNode(RBTreeNode<node_type>* pNode) {
            RBTreeNode<node_type>* pDeleteNode = nil;//删除节点
            RBTreeNode<node_type>* pCDeleteNode = nil;//删除节点的子节点
            if (pNode->left == nil || pNode->right == nil)
                pDeleteNode = pNode;
            else
                pDeleteNode = Successor(pNode);

            if (pDeleteNode->left != nil)
                pCDeleteNode = pDeleteNode->left;
            else
                pCDeleteNode = pDeleteNode->right;

            if (pDeleteNode->parent == nil)
                root = pCDeleteNode;
            else if (pDeleteNode == pDeleteNode->parent->left)
                pDeleteNode->parent->left = pCDeleteNode;
            else
                pDeleteNode->parent->right = pCDeleteNode;

            if (pDeleteNode != pNode) pNode->nData = pDeleteNode->nData;
            pCDeleteNode->parent = pDeleteNode->parent;
            if (pDeleteNode->color == NodeColor::BLACK) DeleteFixup(pCDeleteNode);
            return pDeleteNode;
        }
        RBTreeNode<node_type>* FindNode(const node_type& nData) {
            auto pTemp = root;
            while (pTemp != nil) {
				if (nData.first < pTemp->nData.first)
                    pTemp = pTemp->left;
                else if (nData.first > pTemp->nData.first)
                    pTemp = pTemp->right;
                else
                    return pTemp;
            }
            return nil;
        }
        RBTreeNode<node_type>* Maximum(RBTreeNode<node_type>* pNode) {
            while (pNode->right != nil)
                pNode = pNode->right;
            return pNode;
        }
        RBTreeNode<node_type>* Minimum(RBTreeNode<node_type>* pNode) {
            while (pNode->left != nil)
                pNode = pNode->left;
            return pNode;
        }
        RBTreeNode<node_type>* Successor(RBTreeNode<node_type>* pNode) {
            if (pNode->right != nil) return Minimum(pNode->right);
            auto pPNode = pNode->parent;
            while (pPNode != nil && pNode == pPNode->right) {
                pNode = pPNode;
                pPNode = pNode->parent;
            }
            return pPNode;
        }
        void DeleteTree(RBTreeNode<node_type>* pNode) {
            if (pNode == nil) return;
            DeleteTree(pNode->left);
            DeleteTree(pNode->right);
            delete pNode;
            pNode = nullptr;
        }
    private:
        RBTreeNode<node_type>* root;
        RBTreeNode<node_type>* nil;//nil 是一个空节点，它的左右孩子和父亲都是nil，它的颜色是黑色
        int    node_count;
    };
    template<class KTx,class VTy>
	using map = RBTree<nonstd::pair<KTx, VTy>>;
}