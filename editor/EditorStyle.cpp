#include "EditorStyle.h"

#include <imgui.h>

namespace ui
{

namespace // unnamed
{

constexpr unsigned int CANVAS_COLOR       = 0x1E1F22;
constexpr unsigned int MENU_COLOR         = 0x25262A;
constexpr unsigned int PANEL_COLOR        = 0x2B2D30;
constexpr unsigned int RAISED_COLOR       = 0x313338;
constexpr unsigned int FRAME_COLOR        = 0x393B40;
constexpr unsigned int FRAME_HOVER_COLOR  = 0x43464C;
constexpr unsigned int FRAME_ACTIVE_COLOR = 0x4A4D53;
constexpr unsigned int BORDER_COLOR       = 0x3C3F44;
constexpr unsigned int TEXT_COLOR         = 0xDFE1E5;
constexpr unsigned int MUTED_TEXT_COLOR   = 0x6F737A;
constexpr unsigned int ACCENT_COLOR       = 0x3574F0;
constexpr unsigned int SELECTION_COLOR    = 0x2E436E;

//
// Utils
//

ImVec4 ToColor(unsigned int _Rgb, float _Alpha = 1.0f)
{
  return ImVec4(static_cast<float>((_Rgb >> 16) & 0xFF) / 255.0f,
         static_cast<float>((_Rgb >> 8) & 0xFF) / 255.0f,
         static_cast<float>( _Rgb & 0xFF) / 255.0f,
         _Alpha);
}

void ApplyColors(ImGuiStyle & _Style)
{
  auto & Colors = _Style.Colors;

  Colors[ImGuiCol_Text]           = ToColor(TEXT_COLOR);
  Colors[ImGuiCol_TextDisabled]   = ToColor(MUTED_TEXT_COLOR);
  Colors[ImGuiCol_TextSelectedBg] = ToColor(ACCENT_COLOR, 0.35f);

  Colors[ImGuiCol_WindowBg]         = ToColor(PANEL_COLOR);
  Colors[ImGuiCol_ChildBg]          = ToColor(PANEL_COLOR, 0.0f);
  Colors[ImGuiCol_PopupBg]          = ToColor(RAISED_COLOR);
  Colors[ImGuiCol_MenuBarBg]        = ToColor(MENU_COLOR);
  Colors[ImGuiCol_TitleBg]          = ToColor(PANEL_COLOR);
  Colors[ImGuiCol_TitleBgActive]    = ToColor(PANEL_COLOR);
  Colors[ImGuiCol_Border]           = ToColor(BORDER_COLOR);
  Colors[ImGuiCol_Separator]        = ToColor(BORDER_COLOR);
  Colors[ImGuiCol_SeparatorHovered] = ToColor(ACCENT_COLOR, 0.6f);
  Colors[ImGuiCol_SeparatorActive]  = ToColor(ACCENT_COLOR);

  Colors[ImGuiCol_FrameBg]        = ToColor(FRAME_COLOR);
  Colors[ImGuiCol_FrameBgHovered] = ToColor(FRAME_HOVER_COLOR);
  Colors[ImGuiCol_FrameBgActive]  = ToColor(FRAME_ACTIVE_COLOR);
  Colors[ImGuiCol_Button]         = ToColor(FRAME_COLOR);
  Colors[ImGuiCol_ButtonHovered]  = ToColor(FRAME_HOVER_COLOR);
  Colors[ImGuiCol_ButtonActive]   = ToColor(FRAME_ACTIVE_COLOR);

  Colors[ImGuiCol_Header]        = ToColor(SELECTION_COLOR);
  Colors[ImGuiCol_HeaderHovered] = ToColor(FRAME_HOVER_COLOR);
  Colors[ImGuiCol_HeaderActive]  = ToColor(SELECTION_COLOR);

  Colors[ImGuiCol_CheckMark]        = ToColor(ACCENT_COLOR);
  Colors[ImGuiCol_SliderGrab]       = ToColor(ACCENT_COLOR);
  Colors[ImGuiCol_SliderGrabActive] = ToColor(ACCENT_COLOR);
  Colors[ImGuiCol_DragDropTarget]   = ToColor(ACCENT_COLOR);

  Colors[ImGuiCol_ScrollbarBg]          = ToColor(PANEL_COLOR, 0.0f);
  Colors[ImGuiCol_ScrollbarGrab]        = ToColor(FRAME_ACTIVE_COLOR);
  Colors[ImGuiCol_ScrollbarGrabHovered] = ToColor(FRAME_HOVER_COLOR);
  Colors[ImGuiCol_ScrollbarGrabActive]  = ToColor(FRAME_HOVER_COLOR);

  Colors[ImGuiCol_Tab]                       = ToColor(PANEL_COLOR);
  Colors[ImGuiCol_TabHovered]                = ToColor(FRAME_HOVER_COLOR);
  Colors[ImGuiCol_TabSelected]               = ToColor(RAISED_COLOR);
  Colors[ImGuiCol_TabSelectedOverline]       = ToColor(ACCENT_COLOR);
  Colors[ImGuiCol_TabDimmed]                 = ToColor(PANEL_COLOR);
  Colors[ImGuiCol_TabDimmedSelected]         = ToColor(RAISED_COLOR);
  Colors[ImGuiCol_TabDimmedSelectedOverline] = ToColor(FRAME_ACTIVE_COLOR);

  Colors[ImGuiCol_DockingPreview] = ToColor(ACCENT_COLOR, 0.5f);
  Colors[ImGuiCol_DockingEmptyBg] = ToColor(CANVAS_COLOR);

  Colors[ImGuiCol_ModalWindowDimBg] = ToColor(0x000000, 0.5f);
}

void ApplyMetrics(ImGuiStyle & _Style)
{
  _Style.WindowPadding      = ImVec2(8.0f, 8.0f);
  _Style.FramePadding       = ImVec2(8.0f, 4.0f);
  _Style.ItemSpacing        = ImVec2(8.0f, 6.0f);
  _Style.ItemInnerSpacing   = ImVec2(6.0f, 4.0f);
  _Style.IndentSpacing      = 14.0f;
  _Style.ScrollbarSize      = 10.0f;

  _Style.WindowRounding     = 0.0f;
  _Style.FrameRounding      = 4.0f;
  _Style.PopupRounding      = 4.0f;
  _Style.GrabRounding       = 4.0f;
  _Style.ScrollbarRounding  = 6.0f;
  _Style.TabRounding        = 4.0f;

  _Style.WindowBorderSize   = 1.0f;
  _Style.FrameBorderSize    = 0.0f;
  _Style.TabBorderSize      = 0.0f;
  _Style.TabBarOverlineSize = 2.0f;

  _Style.WindowMenuButtonPosition = ImGuiDir_None;
}

} // namespace unnamed

//
// Interface
//

void ApplyEditorStyle(float _DpiScale)
{
  ImGuiStyle & Style = ImGui::GetStyle();

  ImGui::StyleColorsDark(&Style);
  Style.FontSizeBase = 16.0f;
  Style.FrameRounding = 3.0f;
  Style.GrabRounding = 3.0f;

  ApplyColors(Style);
  ApplyMetrics(Style);

  Style.ScaleAllSizes(_DpiScale);
  Style.FontScaleDpi = _DpiScale;
}

} // namespace ui