// proxydll.cpp
#include "proxydll.h"

// global variables

Direct3D9Wrapper*       gl_pmyIDirect3D9;
HINSTANCE           gl_hOriginalDll;
HINSTANCE           gl_hThisInstance;


BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	
   
    switch (ul_reason_for_call)
	{
	    case DLL_PROCESS_ATTACH:
			InitInstance(hModule);
			break;
	    case DLL_PROCESS_DETACH:
			ExitInstance(); 
			break;
		default:
			break;
	}
    return TRUE;
}

// Exported function (faking d3d9.dll's one-and-only export)
IDirect3D9* WINAPI Direct3DCreate9(UINT SDKVersion)
{
	if (!gl_hOriginalDll) LoadOriginalDll(); // looking for the "right d3d9.dll"
	
	// Hooking IDirect3D Object from Original Library
	typedef IDirect3D9 *(WINAPI* D3D9_Type)(UINT SDKVersion);
	D3D9_Type D3DCreate9_fn = (D3D9_Type) GetProcAddress( gl_hOriginalDll, "Direct3DCreate9");
    
    // Debug
	if (!D3DCreate9_fn) 
    {
        
        ::ExitProcess(0); // exit the hard way
    }
	
	// Request pointer from Original Dll. 
	IDirect3D9 *pIDirect3D9_orig = D3DCreate9_fn(SDKVersion);
	
	// Create my IDirect3D8 object and store pointer to original object there.
	// note: the object will delete itself once Ref count is zero (similar to COM objects)
	gl_pmyIDirect3D9 = new Direct3D9Wrapper(pIDirect3D9_orig);
	
	// Return pointer to hooking Object instead of "real one"
	return (gl_pmyIDirect3D9);
}

int WINAPI D3DPERF_EndEvent()
{
    if (!gl_hOriginalDll) LoadOriginalDll(); // looking for the "right d3d9.dll"
    
    typedef int (WINAPI* D3DPERF_EE)(void);
    D3DPERF_EE D3DPERF_EndEvent_fn = (D3DPERF_EE) GetProcAddress( gl_hOriginalDll, "D3DPERF_EndEvent");

    return D3DPERF_EndEvent_fn();
}

 int WINAPI D3DPERF_BeginEvent(D3DCOLOR col, LPCWSTR wszName)
{
    if (!gl_hOriginalDll) LoadOriginalDll(); // looking for the "right d3d9.dll"
    
    typedef int (WINAPI* D3DPERF_BE)(D3DCOLOR,LPCWSTR);
    D3DPERF_BE D3DPERF_BeginEvent_fn = (D3DPERF_BE) GetProcAddress( gl_hOriginalDll, "D3DPERF_BeginEvent");

    return D3DPERF_BeginEvent_fn(col, wszName);
}

void WINAPI D3DPERF_SetMarker(D3DCOLOR col, LPCWSTR wszName)
{
    if (!gl_hOriginalDll) LoadOriginalDll(); // looking for the "right d3d9.dll"
    
    typedef void (WINAPI* D3DPERF_SM)(D3DCOLOR,LPCWSTR);
    D3DPERF_SM D3DPERF_SetMarker_fn = (D3DPERF_SM) GetProcAddress( gl_hOriginalDll, "D3DPERF_SetMarker");

    D3DPERF_SetMarker_fn(col, wszName);
}


void InitInstance(HANDLE hModule) 
{
	
	
	// Initialisation
	gl_hOriginalDll        = NULL;
	gl_hThisInstance       = NULL;
	gl_pmyIDirect3D9       = NULL;
	
	
	// Storing Instance handle into global var
	gl_hThisInstance = (HINSTANCE)  hModule;
}

void LoadOriginalDll(void)
{
    char buffer[MAX_PATH];
    
    // Getting path to system dir and to d3d8.dll
	::GetSystemDirectory(buffer,MAX_PATH);

	// Append dll name
	strcat_s(buffer,"\\d3d9.dll");
	
	// try to load the system's d3d9.dll, if pointer empty
	if (!gl_hOriginalDll) gl_hOriginalDll = ::LoadLibrary(buffer);

	// Debug
	if (!gl_hOriginalDll)
	{
		
		::ExitProcess(0); // exit the hard way
	}
}

void ExitInstance() 
{    
    
	
	// Release the system's d3d9.dll
	if (gl_hOriginalDll)
	{
		::FreeLibrary(gl_hOriginalDll);
	    gl_hOriginalDll = NULL;  
	}
}

