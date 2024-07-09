#pragma once

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif // !GLEW_STATIC

#ifndef DEBUG
#define DEBUG
#endif // !DEBUG

#ifndef DEBUG_SS // 流的debug，逐行输出shader文件 // 在此处定义似乎没生效
#define DEBUG_SS
#endif // !DEBUG_SS

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Debug.hpp"
#include "Init.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include <boost/algorithm/algorithm.hpp>

#define KETINPUT

#ifdef KETINPUT
#include "KeyControll.hpp"
#endif // KETINPUT

