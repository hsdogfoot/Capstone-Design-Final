#pragma once
#include "Base.h"

class Component;

class ComponentFactory final : public Base
{
public:
	static ComponentFactory* GetInstance();
	static uint32_t DestroyInstance();

	bool Initialize();

	bool RegisterPrototype(const WCHAR* tag, Component* prototype);
	void RemovePrototype(const WCHAR* tag);

	template <typename T, typename... Args>
	bool GetClone(const WCHAR* tag, Component** outInstance, Args... arguments) const;

private:
	ComponentFactory();
	virtual ~ComponentFactory() override = default;

	bool registerDefaultPrototypes();

	virtual void destroy() override;

private:
	static ComponentFactory* mInstance;

	std::unordered_map<const WCHAR*, Component*> mPrototypes;
};

template<typename T, typename ...Args>
bool ComponentFactory::GetClone(const WCHAR* tag, Component** outInstance, Args... arguments) const
{
	auto iter = mPrototypes.find(tag);
	if (iter == mPrototypes.end())
	{
		ErrorLog("Component prototype find failed.");

		return false;
	}

	if (!static_cast<T*>(iter->second)->Clone(outInstance, arguments...))
	{
		return false;
	}

	return true;
}
