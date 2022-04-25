// HookingManager.cpp - Contains HookingManager class implementation of InstallHooking and UninstallHooking
#include "HookingManager.h"

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
bool HookingManager::InstallHook() {
	// From: https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualprotect#:~:text=Changes%20the%20protection%20on%20a,process%2C%20use%20the%20VirtualProtectEx%20function.
	// Changes the protection on a region of committed pages in the virtual address space of the calling process
	// To change the access protection of any process, use the VirtualProtectEx function
	//
	/*	oldProtect - A pointer to a variable that receives the previous access protection value of the first page
		in the specified region of pages.
		*** If this parameter is NULL or does not point to a valid variable, the function fails.*/
	DWORD oldProtect;
	bool isConnect = VirtualProtect(this->func2hook, 1024, PAGE_EXECUTE_READWRITE, &oldProtect);
	checkError(isConnect);
	if (oldProtect == NULL)
		return false;

	// 32 bit relative jump opcode is E9, takes 1 32 bit operand for jump offset
	uint8_t jmpInstruction[5] = { 0xE9, 0x0, 0x0, 0x0, 0x0 };

	// To fill out the last 4 bytes of jmpInstruction, we need the offset between 
	// The payload function and the instruction immediately AFTER the jmp instruction
	const uint32_t relAddr = (uint32_t)this->payloadFunction - ((uint32_t)this->func2hook + sizeof(jmpInstruction));
	memcpy(jmpInstruction + 1, &relAddr, 4);

	// Save the function before installing the hook
	memcpy(this->func2hookSaved, this->func2hook, sizeof(void*));

	// Install the hook
	memcpy(this->func2hook, jmpInstruction, sizeof(jmpInstruction));

	return true;
}

bool HookingManager::UninstallHook() {
	// Checking if there is any saved function
	if (!this->func2hookSaved)
		return false;

	// Uninstall the hook
	memcpy(this->func2hook, this->func2hookSaved, sizeof(void*));
}