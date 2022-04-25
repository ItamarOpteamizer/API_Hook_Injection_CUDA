// HookingManager.h - Contains HookingManager class
#pragma once

#include <string>
#include <iostream>
#include <Windows.h>
using namespace std;

class HookingManager
{
private:
	// The function we want to hook
	void* func2hook;
	// The function we replace with the hooking
	void* payloadFunction;
	// Previues version of func2hook
	void* func2hookSaved = nullptr;

	void checkError(bool answer) {
		if (!answer) {
			cout << "Hooking failed!";
		}
	}

public:
	HookingManager() {
		this->func2hook = nullptr;
		this->payloadFunction = nullptr;
	}

	HookingManager(void* func2hook, void* payloadFunction) {
		this->func2hook = func2hook;
		this->payloadFunction = payloadFunction;
	}

	void setfunc2hook(void* func2hook) {
		this->func2hook = func2hook;
	}

	void* getfunc2hook() {
		return this->func2hook;
	}

	void setpayloadFunction(void* payloadFunction) {
		this->payloadFunction = payloadFunction;
	}

	void* getpayloadFunction() {
		return this->payloadFunction;
	}

	/*
		Sequence:
			- Change func2hook protection
			- Create jump instruction (Jump command + hooking function)
			- Copy the jump instruction to fun2hook

		Input:
			- fun2hook:			A pointer to function
			- payloadFunction:	A pointer to function

		Ouput:
			- If success, return true
			- Otherwise, return false
	*/
	// Replace the call to the function fun2hook with the function payloadFunction
	bool __declspec(dllexport) InstallHook();

	bool __declspec(dllexport) UninstallHook();
};
