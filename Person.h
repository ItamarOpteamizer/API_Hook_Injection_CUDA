#pragma once
#include <string>
#include <iostream>
using namespace std;

class Person
{
public:
	int age;
	string name;

	Person(int age, string name) {
		this->age = age;
		this->name = name;
	}

	static void stopDogFromRunning();
};

