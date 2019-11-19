#pragma once
#include "Component.h"

class Pattern final : public Component
{
public:
	static Pattern* Create();
	bool Clone(Component** outInstance, std::vector<int> patternID);

	virtual void SetOwner(GameObject* owner) override;

	virtual uint8_t Update() override;

	virtual bool CloneForPrefab(Component** outInstance) override;

private:
	Pattern();
	explicit Pattern(std::vector<int> patternID);
	explicit Pattern(const Pattern& other);
	virtual ~Pattern() override = default;

	virtual void destroy() override;

private:
	std::vector<int> mPatternID;
};