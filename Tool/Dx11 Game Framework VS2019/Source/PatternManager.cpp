#include "stdafx.h"
#include "PatternManager.h"

#include <io.h>

PatternManager* PatternManager::mInstance = nullptr;

std::vector<bool (*)(Actor*, float)> PatternManager::mIfStatements;
std::vector<void (*)(Actor*, float)> PatternManager::mBehaviours;

std::vector<std::pair<std::vector<int>, int>> PatternManager::mPatterns;
std::vector<std::pair<std::vector<std::string*>, std::string*>> PatternManager::mParams;

PatternManager* PatternManager::GetInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new PatternManager();
	}

	return mInstance;
}

uint32_t PatternManager::DestroyInstance()
{
	if (mInstance == nullptr)
	{
		return 0;
	}

	uint32_t refCount = mInstance->Release();
	if (refCount == 0)
	{
		mInstance = nullptr;
	}

	return refCount;
}

bool PatternManager::Initialize()
{
	mIfStatements.push_back(CheckKeyDown);

	mBehaviours.push_back(Jump);
	mBehaviours.push_back(MoveHorizontal);
	mBehaviours.push_back(MoveVertical);

	_finddata_t fd = {};
	long handle = 0;
	int result = 0;

	handle = _findfirst("Datas\\Patterns\\*.pat", &fd);
	if (handle == -1)
	{
		return true;
	}

	FILE* stream;
	char filePath[256] = {};
	char rBuffer[256] = {};
	const char* directory = "Datas\\Patterns\\";

	int ifCount;
	char* ptr;
	char* ptr_move;
	bool bToggle = true;

	std::vector<int> ifIndicies;
	std::vector<std::string*> ifParams;
	int behaviourIndex;
	std::string* behaviourParam;

	while (result != -1)
	{
		ifIndicies.clear();
		ifParams.clear();
		behaviourIndex = -1;

		ifCount = 0;

		strcpy(filePath, directory);
		strcat(filePath, fd.name);

		stream = fopen(filePath, "r");
		if (stream == NULL)
		{
			result = _findnext(handle, &fd);

			continue;
		}

		fgets(rBuffer, 256, stream);
		ifCount = atoi(rBuffer);

		for (int i = 0; i < ifCount; ++i)
		{
			fgets(rBuffer, 256, stream);

			ptr = rBuffer;
			ptr_move = rBuffer;

			while (*ptr_move != '\0')
			{
				if (*ptr_move == ',')
				{
					*ptr_move = '\0';

					ifIndicies.push_back(atoi(ptr));

					++ptr_move;
					ptr = ptr_move;

					break;
				}

				++ptr_move;
			}

			ifParams.push_back(new std::string(ptr));
		}

		fgets(rBuffer, 256, stream);

		ptr = rBuffer;
		ptr_move = rBuffer;
		bToggle = true;

		while (*ptr_move != '\0')
		{
			if (*ptr_move == ',')
			{
				*ptr_move = '\0';

				behaviourIndex = atoi(ptr);

				++ptr_move;
				ptr = ptr_move;

				break;
			}

			++ptr_move;
		}

		behaviourParam = new std::string(ptr);

		mPatterns.push_back(std::make_pair(ifIndicies, behaviourIndex));
		mParams.push_back(std::make_pair(ifParams, behaviourParam));

		fclose(stream);

		result = _findnext(handle, &fd);
	}

	_findclose(handle);

	return true;
}

bool PatternManager::CheckKeyDown(Actor* actor, float param)
{
	unsigned char Test = DIK_U;

	switch (static_cast<int>(param))
	{
	case 0:
		Test = DIK_J;
		break;
	case 1:
		Test = DIK_L;
		break;
	case 2:
		Test = DIK_I;
		break;
	case 3:
		Test = DIK_K;
		break;
	}

	return InputManager::GetInstance()->IsKeyPressed(Test);
}

void PatternManager::Jump(Actor* actor, float param)
{

}

void PatternManager::MoveHorizontal(Actor* actor, float param)
{
	actor->GetTransform()->Position = XMVectorAdd(actor->GetTransform()->Position, XMLoadFloat2(&XMFLOAT2(param * 0.01f, 0.0f)));
}

void PatternManager::MoveVertical(Actor* actor, float param)
{
	actor->GetTransform()->Position = XMVectorAdd(actor->GetTransform()->Position, XMLoadFloat2(&XMFLOAT2(0.0f, param * 0.01f)));
}

PatternManager::PatternManager()
	: Base()
{
}

void PatternManager::destroy()
{
	mIfStatements.clear();
	mBehaviours.clear();

	mPatterns.clear();
	mParams.clear();
}
