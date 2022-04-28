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

	// Install hook with 5 extra bytes (Jump + 32 bit adress)
	bool isHooked = hManager->InstallHook32();
	if (!isHooked)
		cout << "Hook installation falied!\n";

	// Dog and Person instances
	Dog *Rocky = new Dog(1, "Rocky");
	Person *Paul = new Person(18, "Adam");

	// Uninstall the hook by restore the overwritten old memory
	hManager->UninstallHook32();

	// Rocky try to run to the park
	// Adam should try to stop him.
	// The function goToPark already hooked by the Adam's function stopDogFromRunning
	Rocky->goToPark();
}