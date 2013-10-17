template<class COMObject>
void SafeRelease(COMObject*& pRes)
{
    IUnknown *unknown = pRes;
    if (unknown)
    {
        unknown->Release();
    }
    pRes = NULL;
}