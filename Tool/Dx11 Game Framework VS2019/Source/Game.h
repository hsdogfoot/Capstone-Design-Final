#pragma once
#include "Application.h"

class Game final : public Application
{
public:
	static bool Create(HINSTANCE hInstance, HWND parentHandle, std::string windowTitle, std::string windowClassName, int windowWidth, int windowHeight, Game** outIstance);

	void ResetWindowSize();

	void Update();
	void Render();

	void SaveData();
	void LoadData();

	void TestingCode();
	void RegisterPrefab(const WCHAR* prefabPath, const WCHAR* prefabName);

private:
	Game();
	virtual ~Game() override = default;

	bool initialize(HINSTANCE hInstance, HWND parentHandle, std::string windowTitle, std::string windowClassName, int windowWidth, int windowHeight);
	bool loadComponents() const;
	bool loadResources() const;
	bool loadActors() const;
	bool loadPrefabs() const;
	bool loadCameras() const;
	   
	virtual void destroy() override;

public:
	bool IsEditMode = false;
};