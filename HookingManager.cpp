#include "HookingManager.h"

/*
	srcFunction = the memory address where you want to place your jmp
	dstFunction = the memory address you want to jump to (probably where you've written your code)
	additionVirtualMemory= the number of bytes used by the instruction you're overwriting
*/
bool HookingManager::InstallHook32(int additionVirtualMemory)
{
	if (additionVirtualMemory < JMP32_SIZE)
		return false;

	// From: https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualprotect#:~:text=Changes%20the%20protection%20on%20a,process%2C%20use%20the%20VirtualProtectEx%20function.
	// Changes the protection on a region of committed pages in the virtual address space of the calling process.
	// To change the access protection of any process, use the VirtualProtectEx function.
	DWORD curProtection;
	VirtualProtect(this->srcFunction, additionVirtualMemory, PAGE_EXECUTE_READWRITE, &curProtection);
	if (!curProtection)
		return false;

	// The offset between the payload function and the instruction immediately AFTER the jmp instruction
	uintptr_t relativeAddress = ((uintptr_t)this->dstFunction - (uintptr_t)this->srcFunction) - JMP32_SIZE;

	// 32 bit relative jump opcode
	*(BYTE*)this->srcFunction = JMP32_OPCODE;
	*(uintptr_t*)((uintptr_t)this->srcFunction + 1) = relativeAddress;

	// Change access protection again to the original one
	DWORD oldProtect;
	VirtualProtect(this->srcFunction, additionVirtualMemory, curProtection, &oldProtect);
	if (!oldProtect)
		return false;

	return true;
}

void* HookingManager::TrampHook32(const intptr_t additionVirtualMemory)
{
	// Make sure the length is greater than JMP32_SIZE
	if (additionVirtualMemory < JMP32_SIZE)
		return 0;

	// Create the gateway (additionVirtualMemory + JMP32_SIZE for the overwritten bytes + the jmp)
	void* gateway = VirtualAlloc(0, additionVirtualMemory + JMP32_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	//Write the additionVirtualMemory into the gateway
	memcpy(gateway, this->srcFunction, additionVirtualMemory);

	// Get the gateway to destination addy
	intptr_t  gatewayRelativeAddr = ((intptr_t)this->srcFunction - (intptr_t)gateway) - JMP32_SIZE;

	// Add the jmp opcode to the end of the gateway
	*(char*)((intptr_t)gateway + additionVirtualMemory) = JMP32_OPCODE;

	// Add the address to the jmp
	*(intptr_t*)((intptr_t)gateway + additionVirtualMemory + 1) = gatewayRelativeAddr;

	// Perform the detour
	bool isHooked = InstallHook32();
	if (!isHooked)
		cout << "Hook installation falied!\n";

	return (void*)gateway;
}

HookingManager* HookingManager::hManager = nullptr;;

/**
 * Static methods should be defined outside the class.
 */
HookingManager* HookingManager::GetInstance(void* srcFunction, void* dstFunction)
{
	/**
	 * This is a safer way to create an instance. instance = new Singleton is
	 * dangeruous in case two instance threads wants to access at the same time
	 */
	if (hManager == nullptr) {
		hManager = new HookingManager(srcFunction, dstFunction);
	}
	return hManager;
}