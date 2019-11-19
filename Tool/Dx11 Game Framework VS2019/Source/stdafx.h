#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <cstdint>
#include <memory>
#include <string>
#include <iostream>
#include <IO.h>
#include <comdef.h>

#include <list>
#include <map>
#include <vector>
#include <unordered_map>
#include <algorithm>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <dinput.h>

using namespace DirectX;

#include <WICTextureLoader.h>

#include "ErrorLogger.h"
#include "FrameworkUtility.h"
#include "StringConverter.h"

#include "Structures.h"
#include "BufferTypes.h"
#include "Resources.h"
#include "Components.h"
#include "Singletons.h"
