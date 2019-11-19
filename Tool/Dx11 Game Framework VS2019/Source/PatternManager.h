#pragma once
#include "Base.h"

class Actor;

class PatternManager final : public Base
{
public:
	static PatternManager* GetInstance();
	static uint32_t DestroyInstance();

	static bool Initialize();

	static bool CheckKeyDown(Actor* actor, float param);

	static void Jump(Actor* actor, float param);
	static void MoveHorizontal(Actor* actor, float param);
	static void MoveVertical(Actor* actor, float param);

private:
	PatternManager();
	virtual ~PatternManager() override = default;

	virtual void destroy() override;

public:
	static std::vector<bool (*)(Actor*, float)> mIfStatements;
	static std::vector<void (*)(Actor*, float)> mBehaviours;

	static std::vector<std::pair<std::vector<int>, int>> mPatterns;
	static std::vector<std::pair<std::vector<std::string*>, std::string*>> mParams;

private:
	static PatternManager* mInstance;
};