#include "utils.hpp"
#include <random>
#include <sstream>
#include <iomanip>

namespace utils{
  std::string generateUUID(){
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

  std::vector<std::string> splitString(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    if (pos_start < s.length()) {
        res.push_back(s.substr(pos_start));
    }
    return res;
  }
}