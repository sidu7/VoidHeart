#pragma once

// Common
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <fstream>

// Data structures
#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>

//Indexing
#include <typeindex>

// Rendering related libraries
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_stdinc.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <ImGui/imgui_impl_sdl.h>

// Serialization library
#include <rapidjson/document.h>
//#ifdef _MSC_VER   // Might need it for rapidjson... 
//#undef GetObject
//#endif

// TODO: Add ifdef guards if more platforms
#include <Windows.h>

// Our stuff, only add things here that don't change
#include "Hollow/Log.h"
#include "Utils/Singleton.h"
#include "Utils/Profiler.h"