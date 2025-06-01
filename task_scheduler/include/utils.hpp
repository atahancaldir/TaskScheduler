#pragma once

#include <string>
#include <vector>

namespace utils{
  std::string generateUUID();
  std::vector<std::string> splitString(std::string s, std::string delimiter);
}