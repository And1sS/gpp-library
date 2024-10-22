#ifndef GPP_GPP_H
#define GPP_GPP_H

#include <unordered_map>
#include <string>

std::unordered_map<std::string, std::string> decode_gpp_string(std::string&& encoded);

std::string encode_gpp_sections(std::unordered_map<std::string, std::string>&& sections);

#endif //GPP_GPP_H
