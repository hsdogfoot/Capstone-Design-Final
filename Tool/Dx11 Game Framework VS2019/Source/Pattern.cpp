#include "stdafx.h"
#include "Pattern.h"

Pattern* Pattern::Create()
{
	return new Pattern();
}

bool Pattern::Clone(Component** outInstance, std::vector<int> patternID)
{
	*outInstance = new Pattern(patternID);

	return true;
}

void Pattern::SetOwner(GameObject* owner)
{
	Assert(owner != nullptr);

	Component::SetOwner(owner);
	static_cast<Actor*>(Owner)->SetPattern(this);
}

uint8_t Pattern::Update()
{
	for (int i = 0; i < mPatternID.size(); ++i)
	{
		bool bResult = true;
		for (int j = 0; j < PatternManager::mPatterns[i].first.size(); ++j)
		{
			if (!PatternManager::mIfStatements[PatternManager::mPatterns[i].first[j]](static_cast<Actor*>(Owner), PatternManager::mParams[i].first[j]))
			{
				bResult = false;
				break;
			}
		}

		if (bResult)
		{
			//PatternManager::mBehaviours[PatternManager::mPatterns[i].second](static_cast<Actor*>(Owner), PatternManager::mParams[i].second);
		}
	}

	return 0;
}

bool Pattern::CloneForPrefab(Component** outInstance)
{
	Pattern* clone = new Pattern(*this);

	*outInstance = clone;

	return true;
}

Pattern::Pattern()
	: Component()
{
}

Pattern::Pattern(std::vector<int> patternID)
	: Component()
	, mPatternID(patternID)
{
}

Pattern::Pattern(const Pattern& other)
	: Component()
	, mPatternID(other.mPatternID)
{
}

void Pattern::destroy()
{
	Component::destroy();
}
