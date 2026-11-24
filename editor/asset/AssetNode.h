#pragma once

#include <string>
#include <filesystem>

namespace editor
{

struct TAssetNode
{
  std::filesystem::path   Path;
  std::string             Extention;
  std::string             Name;
  bool                    IsDirectory = false;
  std::vector<TAssetNode> Children;
};

} // namespace editor