// proxydll.h
#pragma once
#include "stdafx.h"
// Exported function
IDirect3D8* WINAPI Direct3DCreate8 (UINT SDKVersion);

// regular functions
void InitInstance(HANDLE hModule);
void ExitInstance(void);
void LoadOriginalDll(void);