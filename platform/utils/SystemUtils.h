#pragma once

#include <optional>
#include <string>
#include <filesystem>

namespace utils
{

std::optional<std::string> SelectFolder(const std::string & _PreferredPath);

std::filesystem::path GetExecDirectory();

} // namespace utils