// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <Winsock2.h>
#include <windows.h>
#include <WS2tcpip.h>

// C RunTime Header Files
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <atomic>

// DirectX Header Files
#include <d3d9.h>
#include <d3dx9.h>
#include <Xinput.h>

#include <Mmsystem.h>

// STD Header Files
#include <memory>
#include <vector>
#include <list>
#include <unordered_map>
#include <queue>
#include <stack>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <functional>
#include <array>
#include <future>


// Game Modules Header Files
#include "Engine/Constants.h"
#include "Engine/Vector2D.h"
#include "Engine/Utils.h"

#include "IO/json.h"
#include "IO/json-builder.h"
#include "IO/Serializable.h"
#include "IO/Stream.h"

#include "Network/ReliableUDP.h"
#include "Network/NetAddr.h"
#include "Network/NetSocket.h"
#include "Network/NetInit.h"

#include "Sound/Wave.h"
#include "Sound/Sound.h"

#include "Engine/Transform2D.h"
#include "Engine/UpdateList.h"
#include "Engine/Controller.h"
#include "Engine/Sprite.h"
#include "Engine/Object.h"

#include "IO/Input.h"

#include "Menu/Font.h"
#include "Menu/MenuItem.h"

#include "Engine/Camera.h"
#include "Engine/Engine.h"

// Game implementation Header Files
#include "Game/Game.h"

// TODO: reference additional headers your program requires here

#include "Game/Controllers/SinusoidalBehaviourController.h"
#include "Game/Network/NetObject.h"
#include "Game/Network/NetObjectAPI.h"
#include "Game/GameImpl/ResourceManager.h"
#include "Game/GameImpl/ObjectPool.h"

// ImGui
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"

// nlohmann
#include <nlohmann/json.hpp>
