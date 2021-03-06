// lr1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#include <windows.h>
#include <process.h>
#include <iostream>
#include <conio.h>
#include <queue>
#include <time.h>
#include <list>

using namespace std;

#define SIZE 7

CRITICAL_SECTION critsect;

struct Point {
	int x, y;
	Point(int x, int y) {
		this->x = x;
		this->y = y;
	}
	Point(){}
};
            //критическая секция

queue <Point*> qq;
HANDLE SetQ, PrintQ, SortQ;       //Потоки для работы с массивом

void Set(void *) {
	EnterCriticalSection(&critsect);
	cout << "begin set \n";
	srand(time(NULL));
	for (int i = 0; i < SIZE; i++) 
		qq.push(new Point(rand() % 10 + 1, rand() % 10 + 1));
	cout << "end of set \n";
	LeaveCriticalSection(&critsect);
	_endthread();
}
void Print(void *) {
	EnterCriticalSection(&critsect);
	cout << "begin print \n";
	while (!qq.empty()) {
		cout << "[" << qq.front()->x << " , " << qq.front()->y << "]";
		qq.pop();
	}
	cout << "end of print \n";
	LeaveCriticalSection(&critsect);
	_endthread();
}
void Sort(void *) {
	EnterCriticalSection(&critsect);
	cout << "begin sort \n";
	list <Point*> List;
	while (!qq.empty()) {
		List.push_front(qq.front());
		qq.pop();
	}
	List.sort();
	for (auto it = List.begin(); it != List.end(); it++)
		qq.push(*it);
	cout << "end of sort \n";
	LeaveCriticalSection(&critsect);
	_endthread();
}


int main()
{
	InitializeCriticalSection(&critsect);
	SetQ = (HANDLE)_beginthread(Set, 1024, NULL);
	Sleep(1000);
	SortQ = (HANDLE)_beginthread(Sort, 1024, NULL);
	Sleep(1000);
	PrintQ = (HANDLE)_beginthread(Print, 1024, NULL);
	Sleep(1000);
	DeleteCriticalSection(&critsect);
	
	system("pause");
	return 0;
}

