// proxydll.h
#pragma once
#define WIN32_LEAN_AND_MEAN	
#include <windows.h>
#include <d3d9.h>
#include "ID3D9Wrapper.h"
#include "ID3D9Wrapper_Device.h"
	

// Exported function
IDirect3D9* WINAPI Direct3DCreate9 (UINT SDKVersion);

// regular functions
void InitInstance(HANDLE hModule);
void ExitInstance(void);
void LoadOriginalDll(void);