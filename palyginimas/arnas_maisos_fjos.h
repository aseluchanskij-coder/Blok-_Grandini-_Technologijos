#pragma once
#include <vector>
#include <cstdint>
#include <string>

const int ISVESTIES_BAITU_SK = 32;

std::vector<std::uint8_t> gaut_256bit_hasha(std::vector<std::uint32_t> &&blokai);
std::vector<std::uint8_t> str_i_hasha(const std::string &str);