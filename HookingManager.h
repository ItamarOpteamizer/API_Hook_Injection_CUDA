#pragma once
#include <string>
#include <iostream>
#include <Windows.h>
using namespace std;

#define JMP32_SIZE 5
#define JMP32_OPCODE 0xE9

class HookingManager
{
private:
	static HookingManager* hManager;
	void* srcFunction;
	void* dstFunction;

	HookingManager(void* srcFunction, void* dstFunction) {
		this->srcFunction = srcFunction;
		this->dstFunction = dstFunction;
	}

public:
	HookingManager(HookingManager& other) = delete;

	void operator=(const HookingManager&) = delete;

	static HookingManager* GetInstance(void* srcFunction, void* dstFunction);

	void* getSrcFunction() {
		return this->srcFunction;
	}

	void setSrcFunction(void* srcFunction) {
		this->srcFunction = srcFunction;
	}

	void* getDstFunction() {
		return this->srcFunction;
	}

	void setDstFunction(void* dstFunction) {
		this->dstFunction = dstFunction;
	}

	bool InstallHook32(int additionVirtualMemory = JMP32_SIZE);
	void* TrampHook32(const intptr_t additionVirtualMemory = JMP32_SIZE);
};

