#ifndef PCH_H
#define PCH_H

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif // !GLEW_STATIC

#ifndef DEBUG
#define DEBUG
#endif // !DEBUG

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Debug.h"
#include "Init.hpp"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include "vender/stb_image/stb_image.h"
//#include <boost/algorithm/algorithm.hpp>
#include "KeyControll.hpp"
// É«²Ê
#define PURE_WHITE 1.0f, 1.0f, 1.0f
#define PURE_BLACK NULL
#define PURE_RED 1.0f, 0.0f, 0.0f
#define PRE_GREEN 0.0f, 1.0f, 0.0f
#define PURE_BLUE 0.0f, 0.0f, 1.0f
#define ARMY_GREEN 0.16f, 0.26f, 0.20f // ¾üÂÌ

#endif // !PCH_H