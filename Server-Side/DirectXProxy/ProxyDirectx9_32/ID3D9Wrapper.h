#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9tex.h>
#include <windows.h>
#include "ID3D9Wrapper_Device.h"
class Direct3D9Wrapper : public IDirect3D9
{
public:

	Direct3D9Wrapper( LPDIRECT3D9 pDirect3D );
	virtual ~Direct3D9Wrapper();

	//	----------------------------------------------------------------------
	//	----------------------------------------------------------------------
	//
	//		Got from d3d9.h - detoured functions signature
	//
	//	----------------------------------------------------------------------
	//	----------------------------------------------------------------------

	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj);
	STDMETHOD_(ULONG,AddRef)(THIS);
	STDMETHOD_(ULONG,Release)(THIS);

	/*** IDirect3D9 methods ***/
	STDMETHOD(RegisterSoftwareDevice)(THIS_ void* pInitializeFunction);
	STDMETHOD_(UINT, GetAdapterCount)(THIS);
	STDMETHOD(GetAdapterIdentifier)(THIS_ UINT Adapter,DWORD Flags,D3DADAPTER_IDENTIFIER9* pIdentifier);
	STDMETHOD_(UINT, GetAdapterModeCount)(THIS_ UINT Adapter,D3DFORMAT Format);
	STDMETHOD(EnumAdapterModes)(THIS_ UINT Adapter,D3DFORMAT Format,UINT Mode,D3DDISPLAYMODE* pMode);
	STDMETHOD(GetAdapterDisplayMode)(THIS_ UINT Adapter,D3DDISPLAYMODE* pMode);
	STDMETHOD(CheckDeviceType)(THIS_ UINT Adapter,D3DDEVTYPE DevType,D3DFORMAT AdapterFormat,D3DFORMAT BackBufferFormat,BOOL bWindowed);
	STDMETHOD(CheckDeviceFormat)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,DWORD Usage,D3DRESOURCETYPE RType,D3DFORMAT CheckFormat);
	STDMETHOD(CheckDeviceMultiSampleType)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SurfaceFormat,BOOL Windowed,D3DMULTISAMPLE_TYPE MultiSampleType,DWORD* pQualityLevels);
	STDMETHOD(CheckDepthStencilMatch)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,D3DFORMAT RenderTargetFormat,D3DFORMAT DepthStencilFormat);
	STDMETHOD(CheckDeviceFormatConversion)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SourceFormat,D3DFORMAT TargetFormat);
	STDMETHOD(GetDeviceCaps)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DCAPS9* pCaps);
	STDMETHOD_(HMONITOR, GetAdapterMonitor)(THIS_ UINT Adapter);

	//------------------------------------------------------------------------------------------
	//	Function name:	CreateDevice
	//	Description:	Here we create our own device - Direct3DDevice9WrapperExtended - which
	//					will channel all the graphic command to use for our advantage.
	//------------------------------------------------------------------------------------------
	STDMETHOD(CreateDevice)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice9** ppReturnedDeviceInterface);

	
	//	----------------------------------------------------------------------
	//	----------------------------------------------------------------------
	//
	//									Members
	//
	//	----------------------------------------------------------------------
	//	----------------------------------------------------------------------
	IDirect3D9* Direct3D9;	//	The real IDirect3D9 object
};