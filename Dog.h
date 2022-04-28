#pragma once
#include <string>
#include <iostream>
using namespace std;

class Dog
{
public:
	int age;
	string name;

	Dog(int age, string name) {
		this->age = age;
		this->name = name;
	}

	static void goToPark();
};