// proxydll.cpp
#include "stdafx.h"
#include "Log.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <mhook.h>
#include "d3d8.h"
HANDLE	HookHandle		= NULL;

#ifdef __cplusplus
extern "C"
{
#endif
	//--------------------------------------------------------------------------------------
	//	DirectX detoured function
	//--------------------------------------------------------------------------------------
	IDirect3D8* (WINAPI *Real_Direct3DCreate8)(UINT SDKVersion)= Direct3DCreate8;

	//--------------------------------------------------------------------------------------
	//	Various windows API detoured functions
	//--------------------------------------------------------------------------------------
		HWND	(WINAPI *Real_CreateWindowExA)		(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)=CreateWindowExA;
		HWND	(WINAPI *Real_CreateWindowExW)		(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)= CreateWindowExW;
		
#ifdef __cplusplus
}
#endif


IDirect3D8* WINAPI Mine_Direct3DCreate8(UINT SDKVersion)
	{
	LOGFILE("Direct3DCreate8 called.\n");
	IDirect3D8* Direct3D = Real_Direct3DCreate8(SDKVersion);
	IDirect3D8* Mine_Direct3D = new myIDirect3D8(Direct3D);
	return Mine_Direct3D;
	}

//------------------------------------------------------------------------------------------
//	Function name:	HookAPI
//	Description:	Hooking the real functions and our intercepted ones.
//------------------------------------------------------------------------------------------
void HookAPI()
{
	
	if (!Mhook_SetHook((PVOID*)&Real_Direct3DCreate8, Mine_Direct3DCreate8))
	{
		exit(-1);
	}
}

//------------------------------------------------------------------------------------------
//	Function name:	UnhookAPI
//	Description:	Unhooking the real functions and our intercepted ones, upon exit.
//------------------------------------------------------------------------------------------
void UnhookAPI()
{
	Mhook_Unhook((PVOID*)&Real_Direct3DCreate8);
}

//------------------------------------------------------------------------------------------
//	Function name:	DllMain
//	Description:	The dll's main entry point
//	Parameters:		Parameters are used to determine the creation purpose.
//	Returns:		TRUE.
//------------------------------------------------------------------------------------------
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);
		HookHandle = hModule;
		HookAPI();
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		UnhookAPI();
	}
	return TRUE;
}