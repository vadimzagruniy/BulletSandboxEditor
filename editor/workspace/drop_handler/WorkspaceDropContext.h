#pragma once

#include "imgui.h"
#include <string>

namespace editor
{

constexpr const char * SCENE_OBJECT_PAYLOAD = "SCENE_OBJECT_PAYLOAD";

struct TWorkspaceDropContext
{
  std::string PayloadType;
  std::string Value;
  ImVec2      WorldPos;
};

} // namespace editor
