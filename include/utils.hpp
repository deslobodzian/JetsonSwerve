#pragma once

#include <iostream>

inline void info(const std::string& msg) {
    std::cout << "[INFO]" << msg << "\n";
}
inline void error(const std::string& msg) {
    std::cout << "\033[1;31m[ERROR]\033[0m" << msg << "\n";
}
inline void debug(const std::string& msg) {
    std::cout << "\033[1;33m[DEBUG]\033[0m" << msg << "\n";
}