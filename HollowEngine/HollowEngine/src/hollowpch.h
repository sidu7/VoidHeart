#pragma once

// Common
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <fstream>
#include <time.h>
#include <filesystem>
#include <random>
#include <limits>

// Multi-Threading
#include <future>
#include <concrt.h>
#include <concurrent_queue.h>

// Data structures
#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>

//Indexing
#include <typeindex>

// Rendering related libraries
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_mouse.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <ImGui/imgui_impl_sdl.h>
#include <stbi_image/stbi_image.h>
#include <assimp/Importer.hpp>
#include <assimp/assimp/scene.h>
#include <assimp/assimp/postprocess.h>
#include <assimp/assimp/cimport.h>

// FMOD Audio library
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_studio.hpp>

// Serialization library
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/ostreamwrapper.h>

// TODO: Add ifdef guards if more platforms
#define NOMINMAX
#include <Windows.h>

// GetObject() is defined by Windows and conflicts with GetObject() of 10004905

#ifdef _MSC_VER   
#undef GetObject
#endif