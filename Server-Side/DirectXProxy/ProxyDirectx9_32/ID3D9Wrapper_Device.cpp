
#include "ID3D9Wrapper_Device.h"

Direct3DDevice9Wrapper::Direct3DDevice9Wrapper(IDirect3DDevice9 *pDirect3DDevice9)
{
	Direct3DDevice9 = pDirect3DDevice9;
	
	lpvMem = NULL;
	hMapObject = NULL;
	pSurfLocal=NULL;
	badMemory=false;
	lastRecordTime=0;
	
}

Direct3DDevice9Wrapper::~Direct3DDevice9Wrapper(){}

//	---------------------------------------------------------------------------------------
//		All dx9 Functions implemented as a trampoline
//	---------------------------------------------------------------------------------------

/*** IUnknown methods ***/
HRESULT Direct3DDevice9Wrapper::QueryInterface(const IID &riid, void** ppvObj)
{
	
	HRESULT hRes = Direct3DDevice9->QueryInterface(riid, ppvObj);
	if (hRes == S_OK)
		*ppvObj = this;
	else 
		*ppvObj = NULL;

	return hRes;
}

ULONG Direct3DDevice9Wrapper::AddRef()
{
	//DXLOG("D3D9Wrapper_Device: AddRef\n");
	return Direct3DDevice9->AddRef();
}

ULONG Direct3DDevice9Wrapper::Release()
{
	//==============
	this->resourceDeallocate();
	this->uninstallSharedMemory();

	//==============
	ULONG count = Direct3DDevice9->Release();
    if(count==0)
    {
        delete(this);  // destructor will be called automatically
    }
	return (count);
	//return Direct3DDevice9->Release();
}

/*** IDirect3DDevice9 methods ***/
HRESULT Direct3DDevice9Wrapper::TestCooperativeLevel()
{
	//DXLOG("D3D9Wrapper_Device: TestCooperativeLevel\n");
	return Direct3DDevice9->TestCooperativeLevel();
}

UINT Direct3DDevice9Wrapper::GetAvailableTextureMem()
{	
	//DXLOG("D3D9Wrapper_Device: GetAvailableTextureMem\n");
	return Direct3DDevice9->GetAvailableTextureMem();
}

HRESULT Direct3DDevice9Wrapper::EvictManagedResources()
{
	//DXLOG("D3D9Wrapper_Device: EvictManagedResources\n");
	return Direct3DDevice9->EvictManagedResources();
}

HRESULT Direct3DDevice9Wrapper::GetDirect3D(IDirect3D9** ppD3D9)
{
	//DXLOG("D3D9Wrapper_Device: GetDirect3D\n");
	Direct3DDevice9->GetDirect3D(ppD3D9);
	return D3D_OK;
}

HRESULT Direct3DDevice9Wrapper::GetDeviceCaps(D3DCAPS9* pCaps)
{
	//DXLOG("D3D9Wrapper_Device: GetDeviceCaps\n");
	return Direct3DDevice9->GetDeviceCaps(pCaps);
}

HRESULT Direct3DDevice9Wrapper::GetDisplayMode(UINT iSwapChain,D3DDISPLAYMODE* pMode)
{	
	//DXLOG("D3D9Wrapper_Device: GetDisplayMode\n");
	return Direct3DDevice9->GetDisplayMode(iSwapChain, pMode);
}

HRESULT Direct3DDevice9Wrapper::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters)
{
	//DXLOG("D3D9Wrapper_Device: GetCreationParameters\n");
	return Direct3DDevice9->GetCreationParameters(pParameters);
}

HRESULT Direct3DDevice9Wrapper::SetCursorProperties(UINT XHotSpot,UINT YHotSpot,IDirect3DSurface9* pCursorBitmap)
{
	//DXLOG("D3D9Wrapper_Device: SetCursorProperties\n");
	return Direct3DDevice9->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
}

void Direct3DDevice9Wrapper::SetCursorPosition(int X,int Y,DWORD Flags)
{
	//DXLOG("D3D9Wrapper_Device: SetCursorPosition\n");
	return Direct3DDevice9->SetCursorPosition(X, Y, Flags);
}

BOOL Direct3DDevice9Wrapper::ShowCursor(BOOL bShow)
{
	//DXLOG("D3D9Wrapper_Device: ShowCursor\n");
	return Direct3DDevice9->ShowCursor(bShow);
}

HRESULT Direct3DDevice9Wrapper::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain9** pSwapChain)
{
	//DXLOG("D3D9Wrapper_Device: CreateAdditionalSwapChain\n");
	return Direct3DDevice9->CreateAdditionalSwapChain(pPresentationParameters, pSwapChain);
}

HRESULT Direct3DDevice9Wrapper::GetSwapChain(UINT iSwapChain,IDirect3DSwapChain9** pSwapChain)
{
	//DXLOG("D3D9Wrapper_Device: GetSwapChain\n");
	return Direct3DDevice9->GetSwapChain(iSwapChain, pSwapChain);
}

UINT Direct3DDevice9Wrapper::GetNumberOfSwapChains()
{
	//DXLOG("D3D9Wrapper_Device: GetNumberOfSwapChains\n");
	return Direct3DDevice9->GetNumberOfSwapChains();
}

HRESULT Direct3DDevice9Wrapper::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	//DXLOG("D3D9Wrapper_Device: Reset\n");
	this->resourceDeallocate();
	return Direct3DDevice9->Reset(pPresentationParameters);
}

HRESULT Direct3DDevice9Wrapper::Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
{
	//DXLOG("Present\n");
	copyDataToMemory(Direct3DDevice9);
	return Direct3DDevice9->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT Direct3DDevice9Wrapper::GetBackBuffer(UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer)
{
	//DXLOG("D3D9Wrapper_Device: GetBackBuffer\n");
	return Direct3DDevice9->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer);
}

HRESULT Direct3DDevice9Wrapper::GetRasterStatus(UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus)
{
	//DXLOG("D3D9Wrapper_Device: GetRasterStatus\n");
	return Direct3DDevice9->GetRasterStatus(iSwapChain, pRasterStatus);
}

HRESULT Direct3DDevice9Wrapper::SetDialogBoxMode(BOOL bEnableDialogs)
{
	//DXLOG("D3D9Wrapper_Device: SetDialogBoxMode\n");
	return Direct3DDevice9->SetDialogBoxMode(bEnableDialogs);
}

void Direct3DDevice9Wrapper::SetGammaRamp(UINT iSwapChain,DWORD Flags,CONST D3DGAMMARAMP* pRamp)
{
	//DXLOG("D3D9Wrapper_Device: SetGammaRamp\n");
	return Direct3DDevice9->SetGammaRamp(iSwapChain, Flags, pRamp);
}

void Direct3DDevice9Wrapper::GetGammaRamp(UINT iSwapChain,D3DGAMMARAMP* pRamp)
{
	//DXLOG("D3D9Wrapper_Device: GetGammaRamp\n");
	return Direct3DDevice9->GetGammaRamp(iSwapChain, pRamp);
}

HRESULT Direct3DDevice9Wrapper::CreateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle)
{
	//DXLOG("D3D9Wrapper_Device: CreateTexture\n");
	return Direct3DDevice9->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
}

HRESULT Direct3DDevice9Wrapper::CreateVolumeTexture(UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle)
{
	//DXLOG("D3D9Wrapper_Device: CreateVolumeTexture\n");
	return Direct3DDevice9->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
}

HRESULT Direct3DDevice9Wrapper::CreateCubeTexture(UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle)
{
	//DXLOG("D3D9Wrapper_Device: CreateCubeTexture\n");
	return Direct3DDevice9->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
}

HRESULT Direct3DDevice9Wrapper::CreateVertexBuffer(UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle)
{
	//DXLOG("D3D9Wrapper_Device: CreateVertexBuffer\n");
	return Direct3DDevice9->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
}

HRESULT Direct3DDevice9Wrapper::CreateIndexBuffer(UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle)
{
	//DXLOG("D3D9Wrapper_Device: CreateIndexBuffer\n");
	return Direct3DDevice9->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
}

HRESULT Direct3DDevice9Wrapper::CreateRenderTarget(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{
	//DXLOG("D3D9Wrapper_Device: CreateRenderTarget\n");
	return Direct3DDevice9->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
}

HRESULT Direct3DDevice9Wrapper::CreateDepthStencilSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{
	//DXLOG("D3D9Wrapper_Device: CreateDepthStencilSurface\n");
	return Direct3DDevice9->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
}

HRESULT Direct3DDevice9Wrapper::UpdateSurface(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint)
{
	//DXLOG("D3D9Wrapper_Device: UpdateSurface\n");
	return Direct3DDevice9->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
}

HRESULT Direct3DDevice9Wrapper::UpdateTexture(IDirect3DBaseTexture9* pSourceTexture,IDirect3DBaseTexture9* pDestinationTexture)
{
	//DXLOG("D3D9Wrapper_Device: UpdateTexture\n");
	return Direct3DDevice9->UpdateTexture(pSourceTexture, pDestinationTexture);
}

HRESULT Direct3DDevice9Wrapper::GetRenderTargetData(IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface)
{
	//DXLOG("D3D9Wrapper_Device: GetRenderTargetData\n");
	return Direct3DDevice9->GetRenderTargetData(pRenderTarget, pDestSurface);
}

HRESULT Direct3DDevice9Wrapper::GetFrontBufferData(UINT iSwapChain,IDirect3DSurface9* pDestSurface)
{
	//DXLOG("D3D9Wrapper_Device: GetFrontBufferData\n");
	return Direct3DDevice9->GetFrontBufferData(iSwapChain, pDestSurface);
}

HRESULT Direct3DDevice9Wrapper::StretchRect(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter)
{
	//DXLOG("D3D9Wrapper_Device: StretchRect\n");
	return Direct3DDevice9->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
}

HRESULT Direct3DDevice9Wrapper::ColorFill(IDirect3DSurface9* pSurface,CONST RECT* pRect,D3DCOLOR color)
{
	//DXLOG("D3D9Wrapper_Device: ColorFill\n");
	return Direct3DDevice9->ColorFill(pSurface, pRect, color);
}

HRESULT Direct3DDevice9Wrapper::CreateOffscreenPlainSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{
	//DXLOG("D3D9Wrapper_Device: CreateOffscreenPlainSurface\n");
	return Direct3DDevice9->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);
}

HRESULT Direct3DDevice9Wrapper::SetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget)
{
	//DXLOG("D3D9Wrapper_Device: SetRenderTarget\n");
	return Direct3DDevice9->SetRenderTarget(RenderTargetIndex, pRenderTarget);
}

HRESULT Direct3DDevice9Wrapper::GetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget)
{
	//DXLOG("D3D9Wrapper_Device: GetRenderTarget\n");
	return Direct3DDevice9->GetRenderTarget(RenderTargetIndex, ppRenderTarget);
}

HRESULT Direct3DDevice9Wrapper::SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil)
{
	//DXLOG("D3D9Wrapper_Device: SetDepthStencilSurface\n");
	return Direct3DDevice9->SetDepthStencilSurface(pNewZStencil);
}

HRESULT Direct3DDevice9Wrapper::GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface)
{
	//DXLOG("D3D9Wrapper_Device: GetDepthStencilSurface\n");
	return Direct3DDevice9->GetDepthStencilSurface(ppZStencilSurface);
}

HRESULT Direct3DDevice9Wrapper::BeginScene()
{
	//DXLOG("BeginScene\n");
	return Direct3DDevice9->BeginScene();
}

HRESULT Direct3DDevice9Wrapper::EndScene()
{
	//DXLOG("EndScene\n");
	return Direct3DDevice9->EndScene();
}

HRESULT Direct3DDevice9Wrapper::Clear(DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil)
{
	//DXLOG("D3D9Wrapper_Device: Clear\n");
	return Direct3DDevice9->Clear(Count, pRects, Flags, Color, Z, Stencil);
}

HRESULT Direct3DDevice9Wrapper::SetTransform(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix)
{
	//DXLOG("D3D9Wrapper_Device: SetTransform\n");
	return Direct3DDevice9->SetTransform(State, pMatrix);
}

HRESULT Direct3DDevice9Wrapper::GetTransform(D3DTRANSFORMSTATETYPE State,D3DMATRIX* pMatrix)
{
	//DXLOG("D3D9Wrapper_Device: SetTransform\n");
	return Direct3DDevice9->GetTransform(State, pMatrix);
}

HRESULT Direct3DDevice9Wrapper::MultiplyTransform(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix)
{
	//DXLOG("D3D9Wrapper_Device: MultiplyTransform\n");
	return Direct3DDevice9->MultiplyTransform(State, pMatrix);
}

HRESULT Direct3DDevice9Wrapper::SetViewport(CONST D3DVIEWPORT9* pViewport)
{
	//DXLOG("D3D9Wrapper_Device: SetViewport\n");
	return Direct3DDevice9->SetViewport(pViewport);
}

HRESULT Direct3DDevice9Wrapper::GetViewport(D3DVIEWPORT9* pViewport)
{
	//DXLOG("D3D9Wrapper_Device: GetViewport\n");
	return Direct3DDevice9->GetViewport(pViewport);
}

HRESULT Direct3DDevice9Wrapper::SetMaterial(CONST D3DMATERIAL9* pMaterial)
{
	//DXLOG("D3D9Wrapper_Device: SetMaterial\n");
	return Direct3DDevice9->SetMaterial(pMaterial);
}

HRESULT Direct3DDevice9Wrapper::GetMaterial(D3DMATERIAL9* pMaterial)
{
	//DXLOG("D3D9Wrapper_Device: GetMaterial\n");
	return Direct3DDevice9->GetMaterial(pMaterial);
}

HRESULT Direct3DDevice9Wrapper::SetLight(DWORD Index,CONST D3DLIGHT9* Light)
{
	//DXLOG("D3D9Wrapper_Device: SetLight\n");
	return Direct3DDevice9->SetLight(Index, Light);
}

HRESULT Direct3DDevice9Wrapper::GetLight(DWORD Index,D3DLIGHT9* Light)
{
	//DXLOG("D3D9Wrapper_Device: GetLight\n");
	return Direct3DDevice9->GetLight(Index, Light);
}

HRESULT Direct3DDevice9Wrapper::LightEnable(DWORD Index,BOOL Enable)
{
	//DXLOG("D3D9Wrapper_Device: LightEnable\n");
	return Direct3DDevice9->LightEnable(Index, Enable);
}

HRESULT Direct3DDevice9Wrapper::GetLightEnable(DWORD Index,BOOL* pEnable)
{
	//DXLOG("D3D9Wrapper_Device: GetLightEnable\n");
	return Direct3DDevice9->GetLightEnable(Index, pEnable);
}

HRESULT Direct3DDevice9Wrapper::SetClipPlane(DWORD Index,CONST float* pPlane)
{
	//DXLOG("D3D9Wrapper_Device: SetClipPlane\n");
	return Direct3DDevice9->SetClipPlane(Index, pPlane);
}

HRESULT Direct3DDevice9Wrapper::GetClipPlane(DWORD Index,float* pPlane)
{
	//DXLOG("D3D9Wrapper_Device: GetClipPlane\n");
	return Direct3DDevice9->GetClipPlane(Index, pPlane);
}

HRESULT Direct3DDevice9Wrapper::SetRenderState(D3DRENDERSTATETYPE State,DWORD Value)
{
	//DXLOG("D3D9Wrapper_Device: SetRenderState\n");
	return Direct3DDevice9->SetRenderState(State, Value);
}

HRESULT Direct3DDevice9Wrapper::GetRenderState(D3DRENDERSTATETYPE State,DWORD* pValue)
{
	//DXLOG("D3D9Wrapper_Device: GetRenderState\n");
	return Direct3DDevice9->GetRenderState(State, pValue);
}

HRESULT Direct3DDevice9Wrapper::CreateStateBlock(D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB)
{
	//DXLOG("D3D9Wrapper_Device: CreateStateBlock\n");
	return Direct3DDevice9->CreateStateBlock(Type, ppSB);
}

HRESULT Direct3DDevice9Wrapper::BeginStateBlock()
{
	//DXLOG("D3D9Wrapper_Device: BeginStateBlock\n");
	return Direct3DDevice9->BeginStateBlock();
}

HRESULT Direct3DDevice9Wrapper::EndStateBlock(IDirect3DStateBlock9** ppSB)
{
	//DXLOG("D3D9Wrapper_Device: EndStateBlock\n");
	return Direct3DDevice9->EndStateBlock(ppSB);
}

HRESULT Direct3DDevice9Wrapper::SetClipStatus(CONST D3DCLIPSTATUS9* pClipStatus)
{
	//DXLOG("D3D9Wrapper_Device: SetClipStatus\n");
	return Direct3DDevice9->SetClipStatus(pClipStatus);
}

HRESULT Direct3DDevice9Wrapper::GetClipStatus(D3DCLIPSTATUS9* pClipStatus)
{
	//DXLOG("D3D9Wrapper_Device: GetClipStatus\n");
	return Direct3DDevice9->GetClipStatus(pClipStatus);
}

HRESULT Direct3DDevice9Wrapper::GetTexture(DWORD Stage,IDirect3DBaseTexture9** ppTexture)
{
	//DXLOG("D3D9Wrapper_Device: GetTexture\n");
	return Direct3DDevice9->GetTexture(Stage,ppTexture); 
}

HRESULT Direct3DDevice9Wrapper::SetTexture( DWORD Stage,IDirect3DBaseTexture9* pTexture)
{
	//DXLOG("D3D9Wrapper_Device: SetTexture\n");
	return Direct3DDevice9->SetTexture(Stage, pTexture);
}

HRESULT Direct3DDevice9Wrapper::GetTextureStageState( DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD* pValue)
{
	//DXLOG("D3D9Wrapper_Device: GetTextureStageState\n");
	return Direct3DDevice9->GetTextureStageState(Stage,Type,pValue);
}

HRESULT Direct3DDevice9Wrapper::SetTextureStageState( DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value)
{
	//DXLOG("D3D9Wrapper_Device: SetTextureStageState\n");
	return Direct3DDevice9->SetTextureStageState(  Stage, Type, Value); 
}

HRESULT Direct3DDevice9Wrapper::GetSamplerState( DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD* pValue)
{
	//DXLOG("D3D9Wrapper_Device: GetSamplerState\n");
	return Direct3DDevice9->GetSamplerState(  Sampler, Type, pValue);
}

HRESULT Direct3DDevice9Wrapper::SetSamplerState( DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value)
{
	//DXLOG("D3D9Wrapper_Device: SetSamplerState\n");
	return Direct3DDevice9->SetSamplerState(  Sampler, Type, Value);
}

HRESULT Direct3DDevice9Wrapper::ValidateDevice( DWORD* pNumPasses)
{
	//DXLOG("D3D9Wrapper_Device: ValidateDevice\n");
	return Direct3DDevice9->ValidateDevice( pNumPasses); 
}

HRESULT Direct3DDevice9Wrapper::SetPaletteEntries( UINT PaletteNumber,CONST PALETTEENTRY* pEntries)
{
	//DXLOG("D3D9Wrapper_Device: SetPaletteEntries\n");
	return Direct3DDevice9->SetPaletteEntries( PaletteNumber,pEntries); 
}

HRESULT Direct3DDevice9Wrapper::GetPaletteEntries( UINT PaletteNumber,PALETTEENTRY* pEntries)
{
	//DXLOG("D3D9Wrapper_Device: GetPaletteEntries\n");
	return Direct3DDevice9->GetPaletteEntries(PaletteNumber, pEntries); 
}

HRESULT Direct3DDevice9Wrapper::SetCurrentTexturePalette( UINT PaletteNumber)
{
	//DXLOG("D3D9Wrapper_Device: SetCurrentTexturePalette\n");
	return Direct3DDevice9->SetCurrentTexturePalette(PaletteNumber); 
}

HRESULT Direct3DDevice9Wrapper::GetCurrentTexturePalette( UINT *PaletteNumber)
{
	//DXLOG("D3D9Wrapper_Device: GetCurrentTexturePalette\n");
	return Direct3DDevice9->GetCurrentTexturePalette(PaletteNumber);
}

HRESULT Direct3DDevice9Wrapper::SetScissorRect( CONST RECT* pRect)
{
	//DXLOG("D3D9Wrapper_Device: SetScissorRect\n");
	return Direct3DDevice9->SetScissorRect(pRect);
}

HRESULT Direct3DDevice9Wrapper::GetScissorRect( RECT* pRect)
{
	//DXLOG("D3D9Wrapper_Device: GetScissorRect\n");
	return Direct3DDevice9->GetScissorRect(pRect); 
}

HRESULT Direct3DDevice9Wrapper::SetSoftwareVertexProcessing( BOOL bSoftware)
{
	//DXLOG("D3D9Wrapper_Device: SetSoftwareVertexProcessing\n");
	return Direct3DDevice9->SetSoftwareVertexProcessing(bSoftware); 
}

BOOL Direct3DDevice9Wrapper::GetSoftwareVertexProcessing()
{
	//DXLOG("D3D9Wrapper_Device: GetSoftwareVertexProcessing\n");
	return Direct3DDevice9->GetSoftwareVertexProcessing(); 
}

HRESULT Direct3DDevice9Wrapper::SetNPatchMode( float nSegments)
{
	//DXLOG("D3D9Wrapper_Device: SetNPatchMode\n");
	return Direct3DDevice9->SetNPatchMode(nSegments);
}

float Direct3DDevice9Wrapper::GetNPatchMode()
{
	//DXLOG("D3D9Wrapper_Device: GetNPatchMode\n");
	return Direct3DDevice9->GetNPatchMode(); 
}

HRESULT Direct3DDevice9Wrapper::DrawPrimitive( D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount)
{
	//DXLOG("DrawPrimitive\n");
	return Direct3DDevice9->DrawPrimitive(PrimitiveType,StartVertex,PrimitiveCount);
}

HRESULT Direct3DDevice9Wrapper::DrawIndexedPrimitive( D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
{
	//DXLOG("DrawIndexedPrimitive\n");
	return Direct3DDevice9->DrawIndexedPrimitive( PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount); 
}

HRESULT Direct3DDevice9Wrapper::DrawPrimitiveUP( D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
{
	//DXLOG("DrawPrimitiveUP\n");
	return Direct3DDevice9->DrawPrimitiveUP(  PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride); 
}

HRESULT Direct3DDevice9Wrapper::DrawIndexedPrimitiveUP( D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
{
	//DXLOG("DrawIndexedPrimitiveUP\n");
	return Direct3DDevice9->DrawIndexedPrimitiveUP( PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride); 
}

HRESULT Direct3DDevice9Wrapper::ProcessVertices( UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags)
{
	//DXLOG("D3D9Wrapper_Device: ProcessVertices\n");
	return Direct3DDevice9->ProcessVertices( SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags); 
}

HRESULT Direct3DDevice9Wrapper::CreateVertexDeclaration( CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl)
{
	//DXLOG("D3D9Wrapper_Device: CreateVertexDeclaration\n");
	return Direct3DDevice9->CreateVertexDeclaration( pVertexElements, ppDecl);
}

HRESULT Direct3DDevice9Wrapper::SetVertexDeclaration( IDirect3DVertexDeclaration9* pDecl)
{
	//DXLOG("D3D9Wrapper_Device: SetVertexDeclaration\n");
	return Direct3DDevice9->SetVertexDeclaration( pDecl);
}

HRESULT Direct3DDevice9Wrapper::GetVertexDeclaration( IDirect3DVertexDeclaration9** ppDecl)
{
	//DXLOG("D3D9Wrapper_Device: GetVertexDeclaration\n");
	return Direct3DDevice9->GetVertexDeclaration( ppDecl); 
}

HRESULT Direct3DDevice9Wrapper::SetFVF( DWORD FVF)
{
	//DXLOG("D3D9Wrapper_Device: SetFVF\n");
	return Direct3DDevice9->SetFVF( FVF);
}

HRESULT Direct3DDevice9Wrapper::GetFVF( DWORD* pFVF)
{
	//DXLOG("D3D9Wrapper_Device: GetFVF\n");
	return Direct3DDevice9->GetFVF(pFVF);
}

HRESULT Direct3DDevice9Wrapper::CreateVertexShader( CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader)
{
	//DXLOG("D3D9Wrapper_Device: CreateVertexShader\n");
	return Direct3DDevice9->CreateVertexShader( pFunction,ppShader);
}

HRESULT Direct3DDevice9Wrapper::SetVertexShader( IDirect3DVertexShader9* pShader)
{
	//DXLOG("D3D9Wrapper_Device: SetVertexShader\n");
	return Direct3DDevice9->SetVertexShader(pShader);
}

HRESULT Direct3DDevice9Wrapper::GetVertexShader( IDirect3DVertexShader9** ppShader)
{
	//DXLOG("D3D9Wrapper_Device: GetVertexShader\n");
	return Direct3DDevice9->GetVertexShader(ppShader);
}

HRESULT Direct3DDevice9Wrapper::SetVertexShaderConstantF( UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{
	//DXLOG("D3D9Wrapper_Device: SetVertexShaderConstantF\n");
	return Direct3DDevice9->SetVertexShaderConstantF(  StartRegister, pConstantData, Vector4fCount);
}

HRESULT Direct3DDevice9Wrapper::GetVertexShaderConstantF( UINT StartRegister,float* pConstantData,UINT Vector4fCount)
{
	//DXLOG("D3D9Wrapper_Device: GetVertexShaderConstantF\n");
	return Direct3DDevice9->GetVertexShaderConstantF(StartRegister,pConstantData, Vector4fCount);
}

HRESULT Direct3DDevice9Wrapper::SetVertexShaderConstantI( UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount)
{
	//DXLOG("D3D9Wrapper_Device: SetVertexShaderConstantI\n");
	return Direct3DDevice9->SetVertexShaderConstantI( StartRegister, pConstantData, Vector4iCount);
}

HRESULT Direct3DDevice9Wrapper::GetVertexShaderConstantI( UINT StartRegister,int* pConstantData,UINT Vector4iCount)
{
	//DXLOG("D3D9Wrapper_Device: GetVertexShaderConstantI\n");
	return Direct3DDevice9->GetVertexShaderConstantI( StartRegister, pConstantData, Vector4iCount);
}

HRESULT Direct3DDevice9Wrapper::SetVertexShaderConstantB( UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount)
{
	//DXLOG("D3D9Wrapper_Device: SetVertexShaderConstantB\n");
	return Direct3DDevice9->SetVertexShaderConstantB(  StartRegister, pConstantData,  BoolCount);
}

HRESULT Direct3DDevice9Wrapper::GetVertexShaderConstantB( UINT StartRegister,BOOL* pConstantData,UINT BoolCount)
{
	//DXLOG("D3D9Wrapper_Device: GetVertexShaderConstantB\n");
	return Direct3DDevice9->GetVertexShaderConstantB( StartRegister, pConstantData, BoolCount);
}

HRESULT Direct3DDevice9Wrapper::SetStreamSource( UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride)
{
	
	return Direct3DDevice9->SetStreamSource( StreamNumber, pStreamData, OffsetInBytes, Stride);
}

HRESULT Direct3DDevice9Wrapper::GetStreamSource( UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* pOffsetInBytes,UINT* pStride)
{
	//DXLOG("D3D9Wrapper_Device: GetStreamSource\n");
	return Direct3DDevice9->GetStreamSource(  StreamNumber, ppStreamData, pOffsetInBytes, pStride);
}

HRESULT Direct3DDevice9Wrapper::SetStreamSourceFreq( UINT StreamNumber,UINT Setting)
{
	//DXLOG("D3D9Wrapper_Device: SetStreamSourceFreq\n");
	return Direct3DDevice9->SetStreamSourceFreq(  StreamNumber, Setting);
}

HRESULT Direct3DDevice9Wrapper::GetStreamSourceFreq( UINT StreamNumber,UINT* pSetting)
{
	//DXLOG("D3D9Wrapper_Device: GetStreamSourceFreq\n");
	return Direct3DDevice9->GetStreamSourceFreq(  StreamNumber, pSetting);
}

HRESULT Direct3DDevice9Wrapper::SetIndices( IDirect3DIndexBuffer9* pIndexData)
{
	//DXLOG("D3D9Wrapper_Device: SetIndices\n");
	return Direct3DDevice9->SetIndices( pIndexData);
}

HRESULT Direct3DDevice9Wrapper::GetIndices( IDirect3DIndexBuffer9** ppIndexData)
{
	//DXLOG("D3D9Wrapper_Device: GetIndices\n");
	return Direct3DDevice9->GetIndices( ppIndexData);
}

HRESULT Direct3DDevice9Wrapper::CreatePixelShader( CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader)
{
	//DXLOG("D3D9Wrapper_Device: CreatePixelShader\n");
	return Direct3DDevice9->CreatePixelShader( pFunction,ppShader);
}

HRESULT Direct3DDevice9Wrapper::SetPixelShader( IDirect3DPixelShader9* pShader)
{
	//DXLOG("D3D9Wrapper_Device: SetPixelShader\n");
	return Direct3DDevice9->SetPixelShader(  pShader);
}

HRESULT Direct3DDevice9Wrapper::GetPixelShader( IDirect3DPixelShader9** ppShader)
{
	//DXLOG("D3D9Wrapper_Device: GetPixelShader\n");
	return Direct3DDevice9->GetPixelShader(  ppShader);
}

HRESULT Direct3DDevice9Wrapper::SetPixelShaderConstantF( UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{
	//DXLOG("D3D9Wrapper_Device: SetPixelShaderConstantF\n");
	return Direct3DDevice9->SetPixelShaderConstantF(  StartRegister, pConstantData, Vector4fCount); 
}

HRESULT Direct3DDevice9Wrapper::GetPixelShaderConstantF( UINT StartRegister,float* pConstantData,UINT Vector4fCount)
{
	//DXLOG("D3D9Wrapper_Device: GetPixelShaderConstantF\n");
	return Direct3DDevice9->GetPixelShaderConstantF(  StartRegister,pConstantData, Vector4fCount);
}

HRESULT Direct3DDevice9Wrapper::SetPixelShaderConstantI( UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount)
{
	//DXLOG("D3D9Wrapper_Device: SetPixelShaderConstantI\n");
	return Direct3DDevice9->SetPixelShaderConstantI( StartRegister, pConstantData, Vector4iCount);
}

HRESULT Direct3DDevice9Wrapper::GetPixelShaderConstantI( UINT StartRegister,int* pConstantData,UINT Vector4iCount)
{
	//DXLOG("D3D9Wrapper_Device: GetPixelShaderConstantI\n");
	return Direct3DDevice9->GetPixelShaderConstantI(  StartRegister, pConstantData, Vector4iCount);
}

HRESULT Direct3DDevice9Wrapper::SetPixelShaderConstantB( UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount)
{
	//DXLOG("D3D9Wrapper_Device: SetPixelShaderConstantB\n");
	return Direct3DDevice9->SetPixelShaderConstantB(  StartRegister, pConstantData,  BoolCount);
}

HRESULT Direct3DDevice9Wrapper::GetPixelShaderConstantB( UINT StartRegister,BOOL* pConstantData,UINT BoolCount)
{
	//DXLOG("D3D9Wrapper_Device: GetPixelShaderConstantB\n");
	return Direct3DDevice9->GetPixelShaderConstantB(  StartRegister, pConstantData, BoolCount); 
}

HRESULT Direct3DDevice9Wrapper::DrawRectPatch( UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo)
{
	//DXLOG("D3D9Wrapper_Device: DrawRectPatch\n");
	return Direct3DDevice9->DrawRectPatch(  Handle,pNumSegs,pRectPatchInfo);
}

HRESULT Direct3DDevice9Wrapper::DrawTriPatch( UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo)
{
	//DXLOG("D3D9Wrapper_Device: DrawTriPatch\n");
	return Direct3DDevice9->DrawTriPatch(Handle,pNumSegs,pTriPatchInfo);
}

HRESULT Direct3DDevice9Wrapper::DeletePatch( UINT Handle)
{
	//DXLOG("D3D9Wrapper_Device: DeletePatch\n");
	return Direct3DDevice9->DeletePatch(  Handle);
}

HRESULT Direct3DDevice9Wrapper::CreateQuery( D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery)
{
	//DXLOG("D3D9Wrapper_Device: CreateQuery\n");
	return Direct3DDevice9->CreateQuery( Type,ppQuery);
}

bool Direct3DDevice9Wrapper::setupSharedMemory()
{
		hMapObject = CreateFileMapping( 
                INVALID_HANDLE_VALUE,   // use paging file
                NULL,                   // default security attributes
                PAGE_READWRITE,         // read/write access
                0,                      // size: high 32-bits
                SHAREDMEMSIZE,              // size: low 32-bits
                TEXT("ded9dllmemfilemap")); // name of map object
        if (hMapObject == NULL) 
            return FALSE; 
		bool fInit = (GetLastError() != ERROR_ALREADY_EXISTS); 
		lpvMem = (BYTE*)MapViewOfFile( 
                hMapObject,     // object to map view of
                FILE_MAP_WRITE, // read/write access
                0,              // high offset:  map from
                0,              // low offset:   beginning
                0);             // default: map entire file
		if (lpvMem == NULL) 
           return FALSE; 
		if (fInit) 
           memset(lpvMem, '\0', SHAREDMEMSIZE);
		
		return TRUE;
}
void Direct3DDevice9Wrapper::uninstallSharedMemory()
{
	if(lpvMem!=NULL)
	UnmapViewOfFile(lpvMem);
	lpvMem=NULL;
	if(hMapObject!=NULL)
	CloseHandle(hMapObject);
	hMapObject=NULL;
	badMemory=false;
	
}
HRESULT	Direct3DDevice9Wrapper::copyDataToMemory(IDirect3DDevice9* device)
{
	performanceDebugClock=clock();
	//===============Memory Check=======================
	HRESULT hRes = S_FALSE;
	if(badMemory)
	{
		DXLOG("Sorry !Memory!");
	
		return S_FALSE;
	}
	if(lpvMem==NULL)//Memory INIT;
	{
		badMemory=!setupSharedMemory();
		if(badMemory)
		{
			DXLOG("Sorry !Memory!");
			return S_FALSE;
		}
	}
	if(!isMemoryWritable())
	{
		DXLOG("Memory not writable");
		return S_FALSE;
	}
	//================FPS CHECK========================
	if(lastRecordTime==0)
	{
		lastRecordTime=clock();
	}
	else
	{
		long inteval=clock()-lastRecordTime;
		if(inteval<1000/MAXFPS)
		{
			//DXLOG("FPS QUICK");
			return S_FALSE;
		}
		else
		{
			//DXLOG("FPS OK");
			lastRecordTime=clock();
		}
	}
	//=================================================
	IDirect3DSurface9 *pBackBuffer;
	
	if (FAILED(device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer))) 
	{
		DXLOG("PBACKBUFEER");
		return hRes;
	}
	D3DSURFACE_DESC surfaceDesc;
	pBackBuffer->GetDesc(&surfaceDesc);
	if(pSurfLocal==NULL)
	{
		DXLOG("SURFLOCAL CREATING");
		
		hRes = device->CreateOffscreenPlainSurface(surfaceDesc.Width,// RWIDTH,
												surfaceDesc.Height,//RHEIGHT,
												surfaceDesc.Format,
												//D3DFMT_X8R8G8B8, //Seems to work best. Using the target surface's doesn't always work
												D3DPOOL_SYSTEMMEM,
												&pSurfLocal,
												NULL);
		if (FAILED(hRes))
		{
			DXLOG("SURFLOCAL CREATING FAILED");
			SafeRelease(pBackBuffer);
			return hRes;
		}
		
	}

	device->GetRenderTargetData(pBackBuffer, pSurfLocal);
	
	D3DLOCKED_RECT lockedRect;
	if(FAILED(pSurfLocal->LockRect(&lockedRect, NULL, D3DLOCK_READONLY)))
	{
			DXLOG("Lock FAILED");
			SafeRelease(pBackBuffer);
			return hRes;
	}
	 
	//================BPP ADJUSTMENT=====================
	UINT bpp=4;
	UINT bppformat=-1;
	switch(surfaceDesc.Format)
	{

		case D3DFMT_R5G6B5:
			bpp = 2;
			bppformat=2;
			break;
		case D3DFMT_X1R5G5B5:
			bpp = 2;
			bppformat=3;
			break;
		case D3DFMT_A8R8G8B8:
		case D3DFMT_X8R8G8B8:
			bpp = 4;
			bppformat=1;
			break;
		default:
			bpp = 4;
			bppformat=1;
			break;//Most cards are gonna support A8R8G8B8/X8R8G8B8 anyway
	}
	UINT copySize=surfaceDesc.Width*surfaceDesc.Height*bpp;
	UINT surfaceHeight=surfaceDesc.Height;
	UINT surfaceWidth=surfaceDesc.Width;
	//======================Memory Copy=============================
	memcpy(lpvMem+(SHAREDMEMSIZE-RESERVEDMEMORY)/8,(void *)&copySize,sizeof(UINT));
	memcpy(lpvMem+(SHAREDMEMSIZE-RESERVEDMEMORY)/8+sizeof(UINT),(void *)&surfaceHeight,sizeof(UINT));
	memcpy(lpvMem+(SHAREDMEMSIZE-RESERVEDMEMORY)/8+sizeof(UINT)*2,(void *)&surfaceWidth,sizeof(UINT));
	memcpy(lpvMem+(SHAREDMEMSIZE-RESERVEDMEMORY)/8+sizeof(UINT)*3,(void *)&bppformat,sizeof(UINT));
	memcpy(lpvMem, lockedRect.pBits,copySize);
	setMemoryReadable();
	//===================================================
	pSurfLocal->UnlockRect();
	SafeRelease(pBackBuffer);
	LOGFILE(clock()-performanceDebugClock);
	return hRes;
}

void Direct3DDevice9Wrapper::resourceDeallocate()
{
	
	if(pSurfLocal!=NULL)
	SafeRelease(pSurfLocal);
	pSurfLocal=NULL;
	
}
bool Direct3DDevice9Wrapper::isMemoryWritable()
{
	return lpvMem[SHAREDMEMSIZE/8-1]==0;
}
void Direct3DDevice9Wrapper::setMemoryReadable()
{
	lpvMem[SHAREDMEMSIZE/8-1]=1;
}