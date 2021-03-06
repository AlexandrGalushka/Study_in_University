// oslr2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <thread>
#include <Windows.h>
#include <process.h>
#include <vector>
#include <string>
#include <ctime>

#define SIZE 5

using namespace std;


HANDLE Semaphore;

class Fork {
public: 
	bool isUsing = false;
};

Fork * forks;

class Philosopher {
	bool isHunger;
	string philosopherName;
	int number;
	int time_ = 0;
	public: 
		Philosopher(string name, int number) {
		this->philosopherName = name;
		this->number = number;
		Sleep(253);
		srand(time(NULL));
		time_ = rand() % 4000 + 1000;
	}
private: void GetFork()
	{
	
		time_ = rand() % 8000 + 2000;

		cout << "Философ " << philosopherName << " ждет вилку" << "\     ("<< time_ << "мс)\n";

		int first = number;
		int second = (number + 1) % 4;

		WaitForSingleObject(Semaphore, INFINITE);
		if (forks[first].isUsing || forks[second].isUsing) {
			ReleaseSemaphore(Semaphore, 1, NULL);
			return;
		}
		Sleep(rand() % 3000 + 1000);
		forks[first].isUsing = true;
		forks[second].isUsing = true;

		
		cout << "Философ " << philosopherName << " обедает" << "     ("<< time_ << "мс)\n";
		cout << "Вилки " << (first + 1) << " и " << (second + 1) << " заняты " << "     ("<< time_ <<"мс)\n";
		ReleaseSemaphore(Semaphore, 1, NULL);
		Sleep(rand() % 3000 + 1000);
		forks[first].isUsing = false;
		forks[second].isUsing = false;
		cout << "Вилки " << (first + 1) << " и " << (second + 1) << " свободны " << "     (" << time_ << "мс)\n";
	}

public: void Start()
	{
		while (true)
		{
			Sleep(time_);
			ChangeStatus();
			if (isHunger)
				GetFork();
		}
	}

private:	void ChangeStatus()
	{
		isHunger = !isHunger;
		if (!isHunger)
			cout << "Философ " << philosopherName << " размышляет." << "     ("<< time_ <<"мс)\n";
	}
};
Philosopher *p1, *p2, *p3, *p4, *p5;
void test1(void *) {
	p1->Start();
}
void test2(void *) {
	p2->Start();
}
void test3(void *) {
	p3->Start();
}
void test4(void *) {
	p4->Start();
}
void test5(void *) {
	p5->Start();
}



vector <HANDLE> threads; vector <Philosopher> philosophers;

int main()
{
	
	setlocale(LC_ALL, "Russian");
	Philosopher Sokrat(string("Sokrat"), 0);
	Philosopher Platon(string("Platon"), 1);
	Philosopher Kant(string("Kant"), 2);
	Philosopher Marks(string("Marks"), 3);
	Philosopher Archimed(string("Archimed"), 4);
	p1 = &Sokrat;
	p2 = &Platon;
	p3 = &Kant;
	p4 = &Marks;
	p5 = &Archimed;

	forks = new Fork[5];

	Semaphore = CreateSemaphore(NULL, 1, 5, NULL);
	threads.push_back((HANDLE)_beginthread(test1, 1024, NULL));
	threads.push_back((HANDLE)_beginthread(test2, 1024, NULL));
	threads.push_back((HANDLE)_beginthread(test3, 1024, NULL));
	threads.push_back((HANDLE)_beginthread(test4, 1024, NULL));
	threads.push_back((HANDLE)_beginthread(test5, 1024, NULL));

	Sleep(1000);
	//CloseHandle(Semaphore);
	system("pause");
}

