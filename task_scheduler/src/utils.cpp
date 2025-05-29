#include "utils.hpp"
#include <random>
#include <sstream>
#include <iomanip>

namespace utils{
  std::string generateUUID() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static const char* hex = "0123456789abcdef";

    std::stringstream ss;
    ss << std::hex;
    for (int i = 0; i < 8; ++i) {
        ss << hex[dis(gen)];
    }
    ss << "-";
    for (int i = 0; i < 4; ++i) {
        ss << hex[dis(gen)];
    }
    ss << "-4";
    for (int i = 0; i < 3; ++i) {
        ss << hex[dis(gen)];
    }
    ss << "-";
    for (int i = 0; i < 4; ++i) {
        ss << hex[dis(gen)];
    }
    ss << "-";
    for (int i = 0; i < 12; ++i) {
        ss << hex[dis(gen)];
    }
    return ss.str();
  }
}