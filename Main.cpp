#include <string>
#include <iostream>
#include <Windows.h>
using namespace std;

#include "Dog.h"
#include "Person.h"

void* goToParkFunction = &Dog::goToPark;
void* stopDogFromRunningFunction = &Person::stopDogFromRunning;

void InstallHook(void* func2hook, void* payloadFunction)
{
	// From: https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualprotect#:~:text=Changes%20the%20protection%20on%20a,process%2C%20use%20the%20VirtualProtectEx%20function.
	// Changes the protection on a region of committed pages in the virtual address space of the calling process.
	// To change the access protection of any process, use the VirtualProtectEx function.
	DWORD oldProtect;
	VirtualProtect(goToParkFunction, 1024, PAGE_EXECUTE_READWRITE, &oldProtect);

	// 32 bit relative jump opcode is E9, takes 1 32 bit operand for jump offset
	uint8_t jmpInstruction[5] = { 0xE9, 0x0, 0x0, 0x0, 0x0 };

	// To fill out the last 4 bytes of jmpInstruction, we need the offset between 
	// The payload function and the instruction immediately AFTER the jmp instruction
	const uint32_t relAddr = (uint32_t)payloadFunction - ((uint32_t)func2hook + sizeof(jmpInstruction));
	memcpy(jmpInstruction + 1, &relAddr, 4);

	//install the hook
	memcpy(func2hook, jmpInstruction, sizeof(jmpInstruction));
}

void main() {
	InstallHook(goToParkFunction, stopDogFromRunningFunction);

	Dog *Rocky = new Dog(1, "Rocky");
	Person *Paul = new Person(18, "Paul");

	// Rocky try to run to the park
	// Adam should try to stop him.
	// The function goToPark already hooked by the Adam's function stopDogFromRunning
	Rocky->goToPark();
}