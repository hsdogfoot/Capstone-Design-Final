#pragma once

struct AdapterData
{
public:
	explicit AdapterData(IDXGIAdapter* adapter);
	~AdapterData() = default;

public:
	IDXGIAdapter* Adapter;
	DXGI_ADAPTER_DESC Description;
};

class AdapterReader final abstract
{
public:
	static std::vector<AdapterData> GetAdapters();

private:
	static std::vector<AdapterData> mAdapters;
};