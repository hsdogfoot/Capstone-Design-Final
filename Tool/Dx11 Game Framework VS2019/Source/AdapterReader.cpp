#include "stdafx.h"
#include "AdapterReader.h"

std::vector<AdapterData> AdapterReader::mAdapters;

AdapterData::AdapterData(IDXGIAdapter* adapter)
	: Adapter(adapter)
{
	HRESULT hResult = Adapter->GetDesc(&Description);
	if (FAILED(hResult))
	{
		HRLog(hResult);
	}
}

std::vector<AdapterData> AdapterReader::GetAdapters()
{
	if (!mAdapters.empty())
	{
		return mAdapters;
	}

	IDXGIFactory* factory = nullptr;
	HRESULT hResult = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory));
	if (FAILED(hResult))
	{
		HRLog(hResult);
	}

	IDXGIAdapter* adapter;
	UINT index = 0;
	while (SUCCEEDED(factory->EnumAdapters(index, &adapter)))
	{
		mAdapters.push_back(AdapterData(adapter));
		++index;
	}

	return mAdapters;
}
