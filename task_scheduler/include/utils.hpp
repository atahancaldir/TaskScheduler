#pragma once

#include <string>
#include <vector>
#include "constants.hpp"

namespace utils{
  std::string generateUUID();
  std::vector<std::string> splitString(std::string s, std::string delimiter=constants::TASK_BUILDER_DELIMITER);
}