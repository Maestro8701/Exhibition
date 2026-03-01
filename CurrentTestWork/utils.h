#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace parse {

    bool ParseLine(const std::string& line, std::string& out_data, int32_t& out_rand_index);

    std::vector<std::pair<std::string, int32_t>> LoadFromFile(const std::string& filename);

} // namespace parse




