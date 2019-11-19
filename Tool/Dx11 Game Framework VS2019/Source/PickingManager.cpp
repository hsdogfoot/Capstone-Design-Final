#include "stdafx.h"
#include "PickingManager.h"

#include "Actor.h"
#include "CameraManager.h"
#include "RenderManager.h"

PickingManager* PickingManager::mInstance = nullptr;

PickingManager* PickingManager::GetInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new PickingManager();
	}

	return mInstance;
}

uint32_t PickingManager::DestroyInstance()
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

void PickingManager::Initialize(int screenWidth, int screenHeight)
{
	mScreenWidth = static_cast<float>(screenWidth);
	mScreenHeight = static_cast<float>(screenHeight);
}

void PickingManager::ResetWindowSize(int screenWidth, int screenHeight)
{
	mScreenWidth = static_cast<float>(screenWidth);
	mScreenHeight = static_cast<float>(screenHeight);
}

void PickingManager::Pick()
{
	float mouseX = static_cast<float>(InputManager::GetInstance()->GetMouseX());
	float mouseY = static_cast<float>(InputManager::GetInstance()->GetMouseY());

	XMFLOAT4X4 projection;
	XMStoreFloat4x4(&projection, CameraManager::GetInstance()->MainCamera->GetProjection());

	XMFLOAT3 mouse;
	mouse.x = 2.0f * mouseX / mScreenWidth - 1.0f;
	mouse.y = 2.0f * mouseY / mScreenHeight - 1.0f;
	mouse.y *= -1.0f;
	mouse.z = 1.f;

	mouse.x /= projection._11;
	mouse.y /= projection._22;

	XMVECTOR point = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	XMVECTOR dir = XMVectorSet(mouse.x, mouse.y, 1.f, 0.f) - point;
	dir = XMVector3Normalize(dir);


	XMMATRIX view = CameraManager::GetInstance()->MainCamera->GetView();
	view = XMMatrixInverse(nullptr, view);

	point = XMVector3TransformCoord(point, view);
	dir = XMVector3TransformNormal(dir, view);

	int a = 0;

	//OutputDebugString(std::to_wstring(mouseX).c_str());
	//OutputDebugString(L" / ");
	//OutputDebugString(std::to_wstring(mouseY).c_str());
	//OutputDebugString(L"\n");



	//mouseX = 2.0f * mouseX / mScreenWidth - 1.0f;
	//mouseY = 2.0f * mouseY / mScreenHeight - 1.0f;
	//mouseY *= -1.0f;

	//XMVECTOR testDir;
	//testDir = XMVectorSet(mouseX, mouseY, 1.f, 0.f);

	//XMVECTOR a = XMVectorSet(mouseX, mouseY, 1.0f, 1.0f);
	//XMVECTOR b = XMVectorSet(mouseX, mouseY, 1.0f, 1.0f);

	//XMFLOAT4X4 proj;
	//XMStoreFloat4x4(&proj, CameraManager::GetInstance()->MainCamera->GetProjection());


	//XMVECTOR point = XMVectorSet(mouseX / proj._11, mouseY / proj._22, 0.0f, 1.0f);
	//XMMATRIX projectionInverse = XMMatrixInverse(nullptr, CameraManager::GetInstance()->MainCamera->GetProjection());
	//XMMATRIX viewInverse = XMMatrixInverse(nullptr, CameraManager::GetInstance()->MainCamera->GetView());
	////point = XMVector3TransformCoord(point, projectionInverse);
	//point = XMVector3TransformCoord(point, viewInverse);

	//XMFLOAT3X3 viewInv;
	//XMStoreFloat3x3(&viewInv, viewInverse);

	////testDir = XMVector3TransformNormal(testDir, projectionInverse);
	///*testDir.x = mouseX * viewInv._11 + mouseY * viewInv._21 + viewInv._31;
	//testDir.y = mouseX * viewInv._12 + mouseY * viewInv._22 + viewInv._32;
	//testDir.z = mouseX * viewInv._13 + mouseY * viewInv._23 + viewInv._33;*/
	//
	//XMVECTOR dir;
	//dir = XMVector3TransformNormal(testDir, viewInverse);
	////dir = XMVectorSet(testDir.x, testDir.y, testDir.z, 0.f);
	////dir = XMVector3Normalize(dir);
	//XMFLOAT4X4 view;
	//XMStoreFloat4x4(&view, CameraManager::GetInstance()->MainCamera->GetView());
	//XMVECTOR direction = XMVectorSet(view._31, view._32, view._33, 1.0f);
	//direction = XMVector3Normalize(direction);

	//dir = dir - point;

	XMVECTOR localVertices[] =
	{
	   XMVectorSet(-0.5f, -0.5f, 0.0f, 0.0f),
	   XMVectorSet(-0.5f, +0.5f, 0.0f, 0.0f),
	   XMVectorSet(+0.5f, +0.5f, 0.0f, 0.0f),
	   XMVectorSet(+0.5f, -0.5f, 0.0f, 0.0f),
	};

	XMVECTOR worldVertices[] =
	{
	   XMVectorZero(),
	   XMVectorZero(),
	   XMVectorZero(),
	   XMVectorZero(),
	};

	XMMATRIX world;
	XMVECTOR planeNormal;
	XMFLOAT4 planeVector;
	float dot;
	float u;
	XMVECTOR meetPoint;
	for (auto actor : RenderManager::GetInstance()->Actors)
	{
		world = actor->GetTransform()->World;

		for (int i = 0; i < ARRAYSIZE(worldVertices); ++i)
		{
			worldVertices[i] = XMVector3TransformCoord(localVertices[i], world);
		}

		planeNormal = XMVector3Cross(worldVertices[1] - worldVertices[0], worldVertices[2] - worldVertices[0]);
		planeNormal = XMVector3Normalize(planeNormal);

		planeVector.x = XMVectorGetX(planeNormal);
		planeVector.y = XMVectorGetY(planeNormal);
		planeVector.z = XMVectorGetZ(planeNormal);

		planeVector.w = planeVector.x * XMVectorGetX(worldVertices[3]) + planeVector.y * XMVectorGetY(worldVertices[3]) + planeVector.z * XMVectorGetZ(worldVertices[3]);
		planeVector.w *= -1.0f;

		//dot = XMVectorGetX(XMVector3Dot(planeNormal, a));
		//if (dot > 0)
		//{
		//   continue;
		//}

		// u = N * (P3 - P1) / N * (P2 - P1) P1, P2 : 직선 위의 점 P3 : 평면 위의 점 N : 노말
		u = XMVectorGetX(XMVector3Dot(planeNormal, worldVertices[3] - point)) / XMVectorGetX(XMVector3Dot(planeNormal, dir));
		meetPoint = point + u * dir;

		if (isInRect(planeNormal, meetPoint, worldVertices))
		{
			if (SelectedActor != nullptr)
			{
				SelectedActor->GetColorEffect()->SetAlpha(1.0f);
				SelectedActor->GetColorEffect()->SetAlphaBlendingMode(false);

				SelectedActor->Release();
			}

			SelectedActor = actor;
			SelectedActor->AddRefCount();

			SelectedActor->GetColorEffect()->SetAlpha(0.5f);
			SelectedActor->GetColorEffect()->SetAlphaBlendingMode(true);

			break;
		}
		else
		{
			if (SelectedActor != nullptr)
			{
				SelectedActor->GetColorEffect()->SetAlpha(1.0f);
				SelectedActor->GetColorEffect()->SetAlphaBlendingMode(false);

				SelectedActor->Release();
				SelectedActor = nullptr;
			}
		}
	}
}

PickingManager::PickingManager()
	: Base()
	, SelectedActor(nullptr)
	, mScreenWidth(0.0f)
	, mScreenHeight(0.0f)
{
}

bool PickingManager::isInRect(XMVECTOR planeNormal, XMVECTOR planePoint, XMVECTOR* vertices)
{
	XMVECTOR side;
	XMVECTOR cross;
	float dot;

	for (int i = 0; i < 4; ++i)
	{
		side = vertices[(i + 1) % 4] - vertices[i];
		cross = XMVector3Cross(side, planeNormal);
		dot = XMVectorGetX(XMVector3Dot(cross, planePoint - vertices[i]));
		if (dot > 0)
		{
			return false;
		}
	}

	return true;
}

void PickingManager::destroy()
{
	ReleaseBase(SelectedActor);
}
