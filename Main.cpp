#include <string>
#include <iostream>
#include <Windows.h>
using namespace std;

#include "Dog.h"
#include "Person.h"
#include "HookingManager.h"

void* goToParkFunction = &Dog::goToPark;
void* stopDogFromRunningFunction = &Person::stopDogFromRunning;

void main() {
	HookingManager* hManager = HookingManager::GetInstance(goToParkFunction, stopDogFromRunningFunction);

	bool fullHooking = false;

	if (fullHooking) {
		// Install hook with 5 extra bytes (Jump + 32 bit adress)
		bool isHooked = hManager->InstallHook32();
		if (!isHooked)
			cout << "Hook installation falied!\n";
	}
	else {
		void* gateway = hManager->TrampHook32();
		if(!gateway)
			cout << "Hook installation falied!\n";
	}
	// Dog and Person instances
	Dog *Rocky = new Dog(1, "Rocky");
	Person *Paul = new Person(18, "Adam");

	// Rocky try to run to the park
	// Adam should try to stop him.
	// The function goToPark already hooked by the Adam's function stopDogFromRunning
	Rocky->goToPark();
}