#include "gpp.h"

#include <iostream>

std::unordered_map<std::string, std::string> decode_gpp_string(std::string &&ignored) {
    return {{"test", "it works!"}};
}

std::string encode_gpp_sections(std::unordered_map<std::string, std::string> &&sections) {
    std::cout << "sections" << std::endl;
    for (const auto &section: sections)
        std::cout << "key: " << section.first << " , value: " << section.second << "; ";
    std::cout << std::endl;
    std::cout.flush();

    return "encoded~gpp~sections~from~cpp";
}