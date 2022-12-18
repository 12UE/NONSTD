// NONSTD.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include<Windows.h>
#include<stdio.h>
#include <iostream>
#include"nonstd/_Predefine.h"




int main() {
    
	nonstd::map<int,int> obj;
	obj.insert({ 1,1 });
	obj.insert({ 2,2 });
	obj.insert({ 4,3 });
	obj.insert({ 4,4 });
	
	auto p = obj.find(2);
	
	std::cout << p->val.first << std::endl;
	
	std::cout << std::endl << std::endl;
	obj.print();
	
	
	return 0;
}
																						  