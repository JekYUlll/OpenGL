#ifndef GUI_H
#define GUI_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "Debug.h"
#include "KeyControll.hpp"

class Gui
{
private:
	ImGuiIO _io;
	float _fontScale;
public:
	static Gui& getInstance();
	// inline void SetIo(const ImGuiIO& io) { this->_io = io; }
	void SetFont(const std::string& fontPath, const float& scale);
	inline void SetFontGlobalScale(const float& scale);
	inline float GetFontGlobalScale() { return this->_fontScale; }
private:
	Gui();
	Gui(const Gui&) = delete;
	Gui& operator=(const Gui&) = delete;
	
};











#endif // !GUI_H



