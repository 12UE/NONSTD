// NONSTD.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include<Windows.h>
#include<stdio.h>
#include <iostream>
#include"nonstd/_Predefine.h"
#include<initializer_list>
#include"rbtree.h"




int main() {
    
    RBTree rbt;
    
	rbt.Insert(66);

	rbt.Insert(58);
    
	rbt.Insert(47);

	rbt.Insert(35);

    //查找
	RBTreeNode* node = rbt.Find(75);
    if (node != nullptr) {
		printf("查找到了节点：%d\n", node->nData);
	}
	else {
		printf("没有找到节点\n");
    }
       
    
    
    return 0;
}
																						  