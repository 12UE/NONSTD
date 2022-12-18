// NONSTD.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include<Windows.h>
#include<stdio.h>
#include <iostream>
#include"nonstd/_Predefine.h"

//重载pair的等号运算符
template<class T1, class T2>
bool operator==(const nonstd::pair<T1, T2>& lhs, const nonstd::pair<T1, T2>& rhs) {
	return lhs.first == rhs.first;
}

template<class T1, class T2>
bool operator<(const nonstd::pair<T1, T2>&lhs, const nonstd::pair<T1, T2>&rhs) {
	return lhs.first <= rhs.first;
}


int main() {
    

	
    nonstd::RBTree<nonstd::pair<int, int>> rbt;
    
    rbt.Insert({ 1, 1 });
    
	rbt.Insert({ 2, 2 });
    
	rbt.Insert({ 3, 3 });
    
	rbt.Insert({ 4, 4 });
    
    //查找
	auto* node = rbt[{1,1}];
    if (node != nullptr) {
		printf("查找到了节点：%d\n", node->nData.second);
	}
	else {
		printf("没有找到节点\n");
    }
       
    
    
    return 0;
}
																						  