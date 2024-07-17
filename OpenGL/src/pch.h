#ifndef PCH_H
#define PCH_H

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif // !GLEW_STATIC

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// #include <glad/glad.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Gui.h"

#include "Debug.h"
#include "Init.hpp"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
//#include <boost/algorithm/algorithm.hpp>
#include "Window.h"
#include "KeyControll.hpp"

// 色彩
#define PURE_WHITE 1.0f, 1.0f, 1.0f
#define PURE_BLACK 0.0f, 0.0f, 0.0f
#define PURE_RED 1.0f, 0.0f, 0.0f
#define PRE_GREEN 0.0f, 1.0f, 0.0f
#define PURE_BLUE 0.0f, 0.0f, 1.0f
#define ARMY_GREEN 0.16f, 0.26f, 0.20f // 军绿
#define PERPLISH_BLUE 0.13f, 0.16f, 0.20f // 藏蓝
#define IMGUI_SKY_BLUE 0.45f, 0.55f, 0.60f // ImGui初始蓝绿色

#endif // !PCH_H