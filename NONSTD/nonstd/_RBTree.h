#pragma once
namespace nonstd {
    enum class NodeColor :int {RED=0,BLACK=1,};
    template<class T>
    class RBTreeNode final{
    public:
        RBTreeNode<T>* left;
        RBTreeNode<T>* right;
        RBTreeNode<T>* parent;
        T nData;
        NodeColor color;
    };
    template<class T>
    class RBTree{
    public:
        RBTree();
        ~RBTree();
        bool Insert(const T& nData);
        bool Delete(const T& nData);
        RBTreeNode<T>* Find(const T& nData);
        RBTreeNode<T>* operator[](const T& nData);
    private:
        void RotateLeft(RBTreeNode<T>* pNode);
        void RotateRight(RBTreeNode<T>* pNode);
        void InsertFixup(RBTreeNode<T>* pNode);
        void DeleteFixup(RBTreeNode<T>* pNode);
        RBTreeNode<T>* CreateNode(T nData);
        RBTreeNode<T>* DeleteNode(RBTreeNode<T>* pNode);
        RBTreeNode<T>* FindNode(const T& nData);
        RBTreeNode<T>* Maximum(RBTreeNode<T>* pNode);
        RBTreeNode<T>* Minimum(RBTreeNode<T>* pNode);
        RBTreeNode<T>* Successor(RBTreeNode<T>* pNode);
        void DeleteTree(RBTreeNode<T>* pNode);
    private:
        RBTreeNode<T>* root;
	    RBTreeNode<T>* nil;//nil 是一个空节点，它的左右孩子和父亲都是nil，它的颜色是黑色
        int    node_count;
    };
    template<class T>
    RBTree<T>::RBTree(){
        nil = new RBTreeNode<T>();
        nil->left = nullptr;
        nil->right = nullptr;
        nil->parent = nullptr;
        nil->nData = T();
        nil->color = NodeColor::BLACK;
        root = nil;
    }
    template<class T>
    RBTree<T>::~RBTree(){
        DeleteTree(root);
        delete nil;
        root = nullptr;
        nil = nullptr;
    }
    template<class T>
    RBTreeNode<T>* RBTree<T>::CreateNode(T nData){
        auto pTempNode = new RBTreeNode<T>();
        pTempNode->left = nil;
        pTempNode->right = nil;
        pTempNode->parent = nil;
        pTempNode->nData = nData;
        pTempNode->color = NodeColor::RED;
        return pTempNode;
    }
    template<class T>
    void RBTree<T>::DeleteTree(RBTreeNode<T>* pNode){
        if (pNode == nil) return;
        DeleteTree(pNode->left);
        DeleteTree(pNode->right);
        delete pNode;
        pNode = nullptr;
    }

    //左旋转
    template<class T>
    void RBTree<T>::RotateLeft(RBTreeNode<T>* pNode){
        auto pRNode = pNode->right;
        pNode->right = pRNode->left;
        if (pRNode->left != nil){
            pRNode->left->parent = pNode;
            pRNode->parent = pNode->parent;
        }
        if (pNode->parent == nil){
            root = pRNode;
        }else if (pNode->parent->left == pNode){
            pNode->parent->left = pRNode;
        }else{
            pNode->parent->right = pRNode;
        }
        pRNode->left = pNode;
        pNode->parent = pRNode;
    }

    //右旋转
    template<class T>
    void RBTree<T>::RotateRight(RBTreeNode<T>* pNode){
        auto pLNode = pNode->left;
        pNode->left = pLNode->right;
        if (pLNode->right != nil){
            pLNode->right->parent = pNode;
            pLNode->parent = pNode->parent;
        }
        if (pNode->parent == nil){
            root = pLNode;
        }else if (pNode->parent->left == pNode){
            pNode->parent->left = pLNode;
        }else{
            pNode->parent->right = pLNode;
        }
        pLNode->right = pNode;
        pNode->parent = pLNode;
    }
    template<class T>
    RBTreeNode<T>* RBTree<T>::Maximum(RBTreeNode<T>* pNode){
        while (pNode->right != nil)
            pNode = pNode->right;
        return pNode;
    }
    template<class T>
    RBTreeNode<T>* RBTree<T>::Minimum(RBTreeNode<T>* pNode){
        while (pNode->left != nil)
            pNode = pNode->left;
        return pNode;
    }
    template<class T>
    RBTreeNode<T>* RBTree<T>::Successor(RBTreeNode<T>* pNode){
        if (pNode->right != nil) return Minimum(pNode->right);
        auto pPNode = pNode->parent;
        while (pPNode != nil && pNode == pPNode->right){
            pNode = pPNode;
            pPNode = pNode->parent;
        }
        return pPNode;
    }
    template<class T>
    bool RBTree<T>::Insert(const T& nData){
        auto pNewNode = CreateNode(nData);
        auto pPNewNode = nil;
        auto pTemp = root;
        while (pTemp != nil){
            pPNewNode = pTemp;
            if (nData < pTemp->nData)
                pTemp = pTemp->left;
            else
                pTemp = pTemp->right;
        }

        pNewNode->parent = pPNewNode;
        if (pPNewNode == nil)
            root = pNewNode;
        else if (nData < pPNewNode->nData)
            pPNewNode->left = pNewNode;
        else
            pPNewNode->right = pNewNode;

        InsertFixup(pNewNode);
        return true;
    }
    template<class T>
    void RBTree<T>::InsertFixup(RBTreeNode<T>* pNode){
        while (pNode->parent->color == NodeColor::RED){
            if (pNode->parent == pNode->parent->parent->left){
                auto pUNode = pNode->parent->parent->right;
                if (pUNode->color == NodeColor::RED){
                    pNode->parent->color = NodeColor::BLACK;
                    pUNode->color = NodeColor::BLACK;
                    pNode->parent->parent->color = NodeColor::RED;
                    pNode = pNode->parent->parent;
                }else if (pNode == pNode->parent->right){
                    pNode = pNode->parent;
                    RotateLeft(pNode);
                }else{
                    pNode->parent->color = NodeColor::BLACK;
                    pNode->parent->parent->color = NodeColor::RED;
                    RotateRight(pNode->parent->parent);
                }
            }else{
                auto pUNode = pNode->parent->parent->left;
                if (pUNode->color == NodeColor::RED){
                    pNode->parent->color = NodeColor::BLACK;
                    pUNode->color = NodeColor::BLACK;
                    pNode->parent->parent->color = NodeColor::RED;
                    pNode = pNode->parent->parent;
                }else if (pNode == pNode->parent->left){
                    pNode = pNode->parent;
                    RotateRight(pNode);
                }else{
                    pNode->parent->color = NodeColor::BLACK;
                    pNode->parent->parent->color = NodeColor::RED;
                    RotateLeft(pNode->parent->parent);
                }
            }
        }
        root->color = NodeColor::BLACK;
    }
    template<class T>
    bool RBTree<T>::Delete(const T& nData){
        auto pDeleteNode = FindNode(nData);
        if (pDeleteNode == nil){
            return false;
        }
        DeleteNode(pDeleteNode);
        return true;
    }
    template<class T>
    inline RBTreeNode<T>* RBTree<T>::Find(const T& nData){
        auto pCurrent = root;
        while (pCurrent != nullptr){
            if (nData == pCurrent->nData){
                return pCurrent;
            }else if (nData < pCurrent->nData){
                pCurrent = pCurrent->left;
            }else{
                pCurrent = pCurrent->right;
            }
        }
        return nullptr;
    }

    template<class T>
    inline RBTreeNode<T>* RBTree<T>::operator[](const T& nData){return Find(nData);}

    template<class T>
    RBTreeNode<T>* RBTree<T>::FindNode(const T& nData){
        auto pTemp = root;
        while (pTemp != nil){
            if (nData < pTemp->nData)
                pTemp = pTemp->left;
            else if (nData > pTemp->nData)
                pTemp = pTemp->right;
            else
                return pTemp;
        }
        return nil;
    }
    template<class T>
    RBTreeNode<T>* RBTree<T>::DeleteNode(RBTreeNode<T>* pNode){
        auto pDeleteNode = nil;//删除节点
        auto pCDeleteNode = nil;//删除节点的子节点
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
    template<class T>
    void RBTree<T>::DeleteFixup(RBTreeNode<T>* pNode){
        while (pNode != root && pNode->color == NodeColor::BLACK){
            if (pNode == pNode->parent->left){
                auto pBNode = pNode->parent->right;
                if (pBNode->color == NodeColor::RED){
                    pBNode->color = NodeColor::BLACK;
                    pNode->parent->color = NodeColor::RED;
                    RotateLeft(pNode->parent);
                    pBNode = pNode->parent->right;
                }
                if (pBNode->left->color == NodeColor::BLACK && pBNode->right->color == NodeColor::BLACK){
                    pBNode->color = NodeColor::RED;
                    pNode = pNode->parent;
                }else if (pBNode->right->color == NodeColor::BLACK){
                    pBNode->left->color = NodeColor::BLACK;
                    pBNode->color = NodeColor::RED;

                    RotateRight(pBNode);
                    pBNode = pNode->parent->right;
                }else{
                    pBNode->color = pNode->parent->color;
                    pNode->parent->color = NodeColor::BLACK;
                    pBNode->right->color = NodeColor::BLACK;

                    RotateLeft(pNode->parent);
                    pNode = root;
                }
            }else{
                auto pBNode = pNode->parent->left;
                if(pBNode->color == NodeColor::RED){
                    pBNode->color = NodeColor::BLACK;
                    pNode->parent->color = NodeColor::RED;
                    RotateLeft(pNode->parent);
                    pBNode = pNode->parent->left;
                }
                if(pBNode->left->color == NodeColor::BLACK && pBNode->right->color == NodeColor::BLACK){
                    pBNode->color = NodeColor::RED;
                    pNode = pNode->parent;
                }else if (pBNode->left->color == NodeColor::BLACK){
                    pBNode->right->color = NodeColor::BLACK;
                    pBNode->color = NodeColor::RED;

                    RotateRight(pBNode);
                    pBNode = pNode->parent->left;
                }else{
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
    
}
