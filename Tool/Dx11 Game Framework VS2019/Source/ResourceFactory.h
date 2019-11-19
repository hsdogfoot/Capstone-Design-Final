#pragma once
#include "Base.h"

#include "Resources.h"

class ResourceFactory final : public Base
{
public:
	static ResourceFactory* GetInstance();
	static uint32_t DestroyInstance();

	bool Initialize();

	bool RegisterPrototype(const WCHAR* tag, Resource* prototype);
	bool RemovePrototype(const WCHAR* tag);

	bool GetClone(const WCHAR* tag, Resource** outInstance) const;

private:
	ResourceFactory();
	virtual ~ResourceFactory() override = default;

	bool registerPrototypes();

	virtual void destroy() override;

private:
	static ResourceFactory* mInstance;

	std::unordered_map<const WCHAR*, Resource*> mPrototypes;
};