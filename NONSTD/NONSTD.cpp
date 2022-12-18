// NONSTD.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include<Windows.h>
#include<stdio.h>
#include <iostream>
#include"nonstd/_Predefine.h"



int main() {
    
	nonstd::map<nonstd::string, int> map1;
	map1.Insert({ "1",66 });
	map1.Insert({ "2",77 });
	map1.Insert({ "3",88 });
	map1.Insert({ "4",99 });
	map1.Insert({ "5",100 });

	//查找3
	auto it = map1.Find("3");
	if (it != nullptr) {
		printf("找到了3,值为:%d", it->nData.second);
	}else {
		printf("没找到!");
	}
	return 0;
}
																						  