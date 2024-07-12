#include "Gui.h"

Gui& Gui::getInstance()
{
    IMGUI_CHECKVERSION();
    static Gui instance;
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    instance._io = io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls

    return instance;
}

void Gui::SetFont(const std::string& fontPath, const float& fontScale)
{
    this->_fontScale = fontScale;
    _io.Fonts->Clear(); // Clear previous fonts
    static const ImWchar ranges[] = { 0x0020, 0x00FF, 0x4e00, 0x9FAF, 0 }; // Basic Latin + Chinese characters
    ImFont* font = _io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 8.0f * fontScale, NULL, ranges);
    if (!font)
        std::cerr << "Failed to load font!" << std::endl;
}

void Gui::SetFontGlobalScale(const float& fontScale)
{
    this->_io.FontGlobalScale = fontScale;
}

Gui::Gui()
    :_fontScale(0.0f)
{
    this->_io = ImGui::GetIO(); (void)_io;
}
