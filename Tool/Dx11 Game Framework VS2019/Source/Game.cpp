#include "stdafx.h"
#include "Game.h"

#include "GraphicDevice.h"
#include "RenderWindow.h"

bool Game::Create(HINSTANCE hInstance, HWND parentHandle, std::string windowTitle, std::string windowClassName, int windowWidth, int windowHeight, Game** outIstance)
{
	Assert(hInstance != nullptr);
	Assert(parentHandle != nullptr);

	Game* instance = new Game();
	if (!instance->initialize(hInstance, parentHandle, windowTitle, windowClassName, windowWidth, windowHeight))
	{
		instance->Release();

		return false;
	}

	*outIstance = instance;

	return true;
}

void Game::ResetWindowSize()
{
	RECT windowRect;
	GetClientRect(mRenderWindow->WindowHandle, &windowRect);

	int width = windowRect.right - windowRect.left;
	int height = windowRect.bottom - windowRect.top;
	if (width == mRenderWindow->Width && height == mRenderWindow->Height)
	{
		return;
	}

	mRenderWindow->Width = width;
	mRenderWindow->Height = height;

	InputManager::GetInstance()->ResetWindowSize(width, height);
	PickingManager::GetInstance()->ResetWindowSize(width, height);
}

void Game::Update()
{
	InputManager::GetInstance()->Update();
	ActorManager::GetInstance()->Update();
	CameraManager::GetInstance()->Update();

	float fTime = 0.1f;

	if (InputManager::GetInstance()->IsKeyPressed(DIK_Q))
	{
		CameraManager::GetInstance()->MainCamera->ZoomIn(fTime);
	}
	else if (InputManager::GetInstance()->IsKeyPressed(DIK_E))
	{
		CameraManager::GetInstance()->MainCamera->ZoomOut(fTime);
	}

	if (InputManager::GetInstance()->IsKeyPressed(DIK_W))
	{
		CameraManager::GetInstance()->MainCamera->AdjustLookAtPos(XMFLOAT2(0.0f, 0.1f));
	}
	else if (InputManager::GetInstance()->IsKeyPressed(DIK_S))
	{
		CameraManager::GetInstance()->MainCamera->AdjustLookAtPos(XMFLOAT2(0.0f, -0.1f));
	}

	if (InputManager::GetInstance()->IsKeyPressed(DIK_A))
	{
		CameraManager::GetInstance()->MainCamera->AdjustLookAtPos(XMFLOAT2(-0.1f, 0.0f));
	}
	else if (InputManager::GetInstance()->IsKeyPressed(DIK_D))
	{
		CameraManager::GetInstance()->MainCamera->AdjustLookAtPos(XMFLOAT2(0.1f, 0.0f));
	}

	if (InputManager::GetInstance()->IsKeyDown(DIK_LBRACKET))
	{
		RenderManager::GetInstance()->WidenGrid();
	}

	if (InputManager::GetInstance()->IsKeyDown(DIK_RBRACKET))
	{
		RenderManager::GetInstance()->NarrowGrid();
	}

	if (InputManager::GetInstance()->IsKeyDown(DIK_BACKSLASH))
	{
		RenderManager::GetInstance()->ToggleGrid();
	}

	if (IsEditMode)
	{
		if (InputManager::GetInstance()->IsMouseUp(InputManager::eMouseKeyType::Left))
		{
			if (InputManager::GetInstance()->GetMouseX() >= 0 && InputManager::GetInstance()->GetMouseX() <= InputManager::GetInstance()->mScreenWidth)
			{
				if (InputManager::GetInstance()->GetMouseY() >= 0 && InputManager::GetInstance()->GetMouseY() <= InputManager::GetInstance()->mScreenHeight)
				{
					if (PrefabManager::GetInstance()->SelectedPrefab)
					{
						Actor* clone = nullptr;
						if (PrefabManager::GetInstance()->SelectedPrefab->Clone(&clone))
						{
							clone->GetTransform()->Position = XMLoadFloat2(&InputManager::GetInstance()->GetMouseAtWorld());
							ActorManager::GetInstance()->RegisterActor(clone);
						}
					}
				}
			}
		}

		if (InputManager::GetInstance()->IsKeyDown(DIK_MINUS))
		{
			IsEditMode = false;
			PrefabManager::GetInstance()->SelectedPrefab->Release();
			PrefabManager::GetInstance()->SelectedPrefab = nullptr;
		}
	}
	else
	{

	}

	if (PickingManager::GetInstance()->SelectedActor)
	{
		if (InputManager::GetInstance()->IsKeyDown(DIK_DELETE))
		{
			PickingManager::GetInstance()->SelectedActor->IsInvalid = true;
		}
	}

	//if (PickingManager::GetInstance()->SelectedActor)
	//{
	//	Actor* old = PickingManager::GetInstance()->SelectedActor;

	//	if (InputManager::GetInstance()->IsMouseDown(InputManager::eMouseKeyType::Left))
	//	{
	//		PickingManager::GetInstance()->Pick();
	//	}

	//	if (old == PickingManager::GetInstance()->SelectedActor)
	//	{
	//		if (InputManager::GetInstance()->IsMousePressed(InputManager::eMouseKeyType::Left))
	//		{
	//			old->GetTransform()->Position = XMLoadFloat2(&InputManager::GetInstance()->GetMouseAtWorld());
	//		}
	//	}
	//}

	if (InputManager::GetInstance()->IsMouseUp(InputManager::eMouseKeyType::Left))
	{
		PickingManager::GetInstance()->Pick();
	}
}

void Game::Render()
{
	mGraphicDevice->Render();
}

void Game::SaveData()
{
	const char* targetName = "Datas\\Scenes\\Save.dat";
	FILE* stream = fopen(targetName, "w");

	char buffer[256] = {};

	int totalActorCount = ActorManager::GetInstance()->GetActorCount();
	sprintf(buffer, "%d\n", totalActorCount);

	fwrite(buffer, sizeof(char), strlen(buffer), stream);

	Actor* actor;
	Transform* transform;
	Renderer* renderer;

	XMFLOAT2 scale;
	XMFLOAT2 position;

	for (int i = 0; i < totalActorCount; ++i)
	{
		actor = ActorManager::GetInstance()->GetActor(i);
		transform = actor->GetTransform();
		renderer = actor->GetRenderer();

		XMStoreFloat2(&scale, transform->Scale);
		XMStoreFloat2(&position, transform->Position);

		sprintf(buffer, "%ws\n", actor->PrefabName);
		fwrite(buffer, sizeof(char), strlen(buffer), stream);

		sprintf(buffer, "%f,%f\n", position.x, position.y);
		fwrite(buffer, sizeof(char), strlen(buffer), stream);

		//std::wstring pathWide = renderer->GetTexturePath();
		//std::string path = std::string(pathWide.begin(), pathWide.end());

		//strcpy(buffer, path.c_str());

		//fwrite(buffer, sizeof(char), strlen(buffer), stream);
		//sprintf(buffer, "\n");
		//fwrite(buffer, sizeof(char), strlen(buffer), stream);
	}

	fclose(stream);
}

void Game::LoadData()
{
	//FILE* stream = fopen("Save Data\\Save.dat", "r");
	//if (stream == nullptr)
	//{
	//	return;
	//}

	//char rBuffer[256];

	//fgets(rBuffer, 256, stream);

	//int actorCount = atoi(rBuffer);

	//for (int i = 0; i < actorCount; ++i)
	//{
	//	Actor* actor = nullptr;

	//	actor = Actor::Create();

	//}
}

void Game::TestingCode()
{
	PrefabManager::GetInstance()->SetupPrefab(L"sibal");
	IsEditMode = true;
}

void Game::RegisterPrefab(const WCHAR* prefabPath, const WCHAR* prefabName)
{
	if (PrefabManager::GetInstance()->CheckPrefab(prefabName))
	{
		PrefabManager::GetInstance()->SetupPrefab(prefabName);
		IsEditMode = true;
		return;
	}

	std::wstring ws = std::wstring(prefabPath);
	std::string s = std::string(ws.begin(), ws.end());

	FILE* stream = fopen(s.c_str(), "r");
	if (stream == nullptr)
	{
		return;
	}

	char rBuffer[256];

	fgets(rBuffer, 256, stream);

	char textureTag[256];

	strcpy(textureTag, rBuffer);
	char* texturePtr = textureTag;
	while (*texturePtr != '\0')
	{
		if (*texturePtr == '\n')
		{
			*texturePtr = '\0';
			break;
		}

		++texturePtr;
	}

	fgets(rBuffer, 256, stream);

	char* ptr = rBuffer;
	char* ptr_left = rBuffer;

	float scaleX = 1.0f;
	float scaleY = 1.0f;

	while (*ptr != '\0')
	{
		if (*ptr == ',')
		{
			*ptr = '\0';
			scaleX = atof(ptr_left);

			++ptr;
			ptr_left = ptr;
			continue;
		}

		++ptr;
	}

	scaleY = atof(ptr_left);

	fgets(rBuffer, 256, stream);

	PrefabManager::GetInstance()->RegisterPrefab(prefabName, XMFLOAT2(scaleX, scaleY), XMConvertToRadians(atof(rBuffer)), StringConverter::ConvertStringToWide(std::string(textureTag)).c_str());

	fclose(stream);

	PrefabManager::GetInstance()->SetupPrefab(prefabName);
	IsEditMode = true;
}

Game::Game()
	: Application()
{
}

bool Game::initialize(HINSTANCE hInstance, HWND parentHandle, std::string windowTitle, std::string windowClassName, int windowWidth, int windowHeight)
{
	if (!RenderWindow::Create(hInstance, parentHandle, windowTitle, windowClassName, windowWidth, windowHeight, this, &mRenderWindow))
	{
		ErrorLog("RenderWindow create failed.");

		return false;
	}

	if (!GraphicDevice::Create(mRenderWindow->WindowHandle, windowWidth, windowHeight, &mGraphicDevice))
	{
		ErrorLog("GraphicDevice create failed.");

		return false;
	}

	if (!InputManager::GetInstance()->Initialize(mRenderWindow->HInstance, mRenderWindow->WindowHandle, mRenderWindow->ParentHandle, mRenderWindow->Width, mRenderWindow->Height))
	{
		ErrorLog("InputManager initialize failed.");

		return false;
	}

	if (!RenderManager::GetInstance()->Initialize(mGraphicDevice->Device, mGraphicDevice->DeviceContext, 100, 0, true))
	{
		ErrorLog("RenderManager initialize failed.");

		return false;
	}

	PickingManager::GetInstance()->Initialize(mRenderWindow->Width, mRenderWindow->Height);

	PrefabManager::GetInstance()->Initialize(mGraphicDevice->Device, mGraphicDevice->DeviceContext);

	if (!loadComponents())
	{
		return false;
	}

	if (!loadResources())
	{
		return false;
	}

	//if (!loadActors())
	//{
	//	return false;
	//}

	if (!loadPrefabs())
	{
		return false;
	}

	if (!loadCameras())
	{
		return false;
	}

	Actor* actor = nullptr;

	//actor = Actor::Create();
	//if (!actor->EquipComponent<Transform>(L"Transform", XMFLOAT2(2.0f, 2.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)))
	//{
	//	return false;
	//}

	//if (!actor->EquipComponent<Renderer>(L"Renderer", mGraphicDevice->Device, mGraphicDevice->DeviceContext, L"VS_Default", L"PS_Default", L"Resources\\Textures\\Test02.png"))
	//{
	//	return false;
	//}

	//if (!actor->EquipComponent<ColorEffect>(L"ColorEffect", mGraphicDevice->Device, mGraphicDevice->DeviceContext))
	//{
	//	return false;
	//}

	//std::vector<int> patternIDs;
	//patternIDs.push_back(0);
	//if (!actor->EquipComponent<Pattern>(L"Pattern", patternIDs))
	//{
	//	return false;
	//}
	//ActorManager::GetInstance()->RegisterActor(actor);

	//AnimationData data;
	//PrefabManager::GetInstance()->RegisterPrefab(L"Angmond", XMFLOAT2(2.0f, 3.0f), L"Texture", data);
	//PrefabManager::GetInstance()->SetupPrefab(L"Angmond");

	//PatternManager::GetInstance()->Initialize();

	//LoadData();

	return true;
}

bool Game::loadComponents() const
{
	if (!ComponentFactory::GetInstance()->Initialize())
	{
		ErrorLog("ComponentFactory initialize failed.");

		return false;
	}

	return true;
}

bool Game::loadResources() const
{
	ResourceFactory* factory = ResourceFactory::GetInstance();
	Resource* prototype = nullptr;

	if (!RectBuffer::Create(mGraphicDevice->Device, mGraphicDevice->DeviceContext, &prototype))
	{
		return false;
	}
	if (!factory->RegisterPrototype(L"RectBuffer", prototype))
	{
		return false;
	}

	_finddata_t fd = {};
	long lHandle = 0;
	int iResult = 0;

	lHandle = _findfirst("Resources\\Textures\\*.*", &fd);

	if (lHandle == -1)
	{
		return true;
	}

	while (iResult != -1)
	{
		if (strlen(fd.name) < 3)
		{
			iResult = _findnext(lHandle, &fd);
			continue;
		}

		char texturePath[256] = "Resources\\Textures\\";
		strcat(texturePath, fd.name);

		WCHAR* filePath = new WCHAR[256];
		wcscpy(filePath, StringConverter::ConvertStringToWide(std::string(texturePath)).c_str());


		if (!Texture::Create(mGraphicDevice->Device, mGraphicDevice->DeviceContext, filePath, &prototype))
		{
			return false;
		}
		if (!factory->RegisterPrototype(filePath, prototype))
		{
			return false;
		}

		iResult = _findnext(lHandle, &fd);
	}

	_findclose(lHandle);

	if (!VertexShader::Create(mGraphicDevice->Device, mGraphicDevice->DeviceContext, L"Resources\\Shaders\\VS_Default.cso", VertexShader::LAYOUT_DESC_XYZ_TEXTURE, 2, &prototype))
	{
		return false;
	}
	if (!factory->RegisterPrototype(L"VS_Default", prototype))
	{
		return false;
	}

	if (!PixelShader::Create(mGraphicDevice->Device, mGraphicDevice->DeviceContext, L"Resources\\Shaders\\PS_Default.cso", &prototype))
	{
		return false;
	}
	if (!factory->RegisterPrototype(L"PS_Default", prototype))
	{
		return false;
	}

	return true;
}

bool Game::loadActors() const
{
	FILE* stream = fopen("Datas\\Scenes\\Save.dat", "r");
	if (stream == nullptr)
	{
		return false;
	}

	char rBuffer[256];

	fgets(rBuffer, 256, stream);
	int actorCount = atoi(rBuffer);

	for (int i = 0; i < actorCount; ++i)
	{
		fgets(rBuffer, 256, stream);
		WCHAR* prefabName = new WCHAR[256];
		wcscpy(prefabName, StringConverter::ConvertStringToWide(std::string(rBuffer)).c_str());
		WCHAR* wptr = prefabName;
		while (*wptr != '\0')
		{
			if (*wptr == '\n')
			{
				*wptr = '\0';
				break;
			}

			++wptr;
		}
		Actor* actor = Actor::Create(prefabName);

		char prefabPath[256] = "Datas\\Prefabs\\";
		char extension[] = ".pfb";
		char* ptr_left = rBuffer;
		char* ptr_right = rBuffer;
		while (*ptr_left != '\0')
		{
			if (*ptr_left == '\n')
			{
				*ptr_left = '\0';
				break;
			}

			++ptr_left;
		}

		strcat(prefabPath, rBuffer);
		strcat(prefabPath, extension);

		FILE* stream_prefab = fopen(prefabPath, "r");
		if (stream_prefab == nullptr)
		{
			return false;
		}

		fgets(rBuffer, 256, stream_prefab);
		char texturePath[256];
		strcpy(texturePath, rBuffer);

		WCHAR* texturePath_Wide = new WCHAR[256];
		wcscpy(texturePath_Wide, StringConverter::ConvertStringToWide(std::string(texturePath)).c_str());
		wptr = texturePath_Wide;
		while (*wptr != '\0')
		{
			if (*wptr == '\n')
			{
				*wptr = '\0';
				break;
			}

			++wptr;
		}

		fgets(rBuffer, 256, stream_prefab);
		ptr_left = rBuffer;
		ptr_right = rBuffer;

		float scaleX, scaleY;

		while (*ptr_right != '\0')
		{
			if (*ptr_right == ',')
			{
				*ptr_right = '\0';

				scaleX = atof(ptr_left);

				++ptr_right;
				ptr_left = ptr_right;
				break;
			}

			++ptr_right;
		}

		scaleY = atof(ptr_right);

		fgets(rBuffer, 256, stream_prefab);

		float rotation = atof(rBuffer);

		fgets(rBuffer, 256, stream_prefab);

		int patternCount = atoi(rBuffer);

		fclose(stream_prefab);

		//

		fgets(rBuffer, 256, stream);
		ptr_left = rBuffer;
		ptr_right = rBuffer;

		float posX, posY;

		while (*ptr_right != '\0')
		{
			if (*ptr_right == ',')
			{
				*ptr_right = '\0';

				posX = atof(ptr_left);

				++ptr_right;
				ptr_left = ptr_right;
				break;
			}

			++ptr_right;
		}

		posY = atof(ptr_right);

		if (!actor->EquipComponent<Transform>(L"Transform", XMFLOAT2(scaleX, scaleY), XMFLOAT2(XMConvertToRadians(rotation), 0.0f), XMFLOAT2(posX, posY)))
		{
			return false;
		}

		if (!actor->EquipComponent<Renderer>(L"Renderer", mGraphicDevice->Device, mGraphicDevice->DeviceContext, L"VS_Default", L"PS_Default", texturePath_Wide))
		{
			return false;
		}

		if (!actor->EquipComponent<ColorEffect>(L"ColorEffect", mGraphicDevice->Device, mGraphicDevice->DeviceContext))
		{
			return false;
		}

		ActorManager::GetInstance()->RegisterActor(actor);

	}

	fclose(stream);


	return true;
}

bool Game::loadPrefabs() const
{
	return true;
}

bool Game::loadCameras() const
{
	Camera* camera = nullptr;

	camera = Camera::Create();
	if (!camera->EquipComponent<CamComponent>(L"CamComponent", XMFLOAT2(0.0f, 0.0f), 16.0f, 12.0f, 0.1f, 1000.0f, 1.0f))
	{
		return false;
	}
	CameraManager::GetInstance()->RegisterCamera(L"Main Cam", camera);
	CameraManager::GetInstance()->SetMainCamera(L"Main Cam");

	return true;
}

void Game::destroy()
{
	InputManager::GetInstance()->DestroyInstance();
	PickingManager::GetInstance()->DestroyInstance();
	PrefabManager::GetInstance()->DestroyInstance();
	RenderManager::GetInstance()->DestroyInstance();
	ResourceFactory::GetInstance()->DestroyInstance();
	ComponentFactory::GetInstance()->DestroyInstance();
	CameraManager::GetInstance()->DestroyInstance();
	ActorManager::GetInstance()->DestroyInstance();
	PatternManager::GetInstance()->DestroyInstance();

	Application::destroy();
}
