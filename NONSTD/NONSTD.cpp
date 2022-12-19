// NONSTD.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include<Windows.h>
#include<stdio.h>
#include <iostream>
#include"nonstd/_Predefine.h"




int main() {
    
	nonstd::map<int,std::string> obj;
	obj.insert({ 1,"hello 1" });
	obj.insert({ 2,"hello 2" });
	obj.insert({ 3,"hello 3" });
	obj.insert({ 4,"hello 4" });

	//auto iter1 = obj[2];
	
	//std::cout << iter1->first << " " << iter1->second << std::endl;
	
	for (auto iter = obj.begin(); iter!= obj.end(); iter++) {
		std::cout << iter->first << " " << iter->second << std::endl;
	}
	
	//auto p = obj.find(2);
	//
	//std::cout << p->val.second << std::endl;
	//
	//std::cout << std::endl << std::endl;
	//obj.print();

	
	
	
	return 0;
}
																						  