#pragma once

// Utils
#include "Log.h"

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

// Rendering related libraries
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_stdinc.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <ImGui/imgui_impl_sdl.h>

// TODO: Add ifdef guards if more platforms
#include <Windows.h>

// Our stuff, only add things here that don't change
#include "Hollow/Log.h"
