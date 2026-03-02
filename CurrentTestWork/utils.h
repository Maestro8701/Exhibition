#pragma once
#include <cstdint>
#include <string>
#include <vector>


namespace parse {

    bool ParseLine(const std::string& line, std::string& out_data, int32_t& out_rand_index);

    std::vector<std::pair<std::string, int32_t>> LoadFromFile(const std::string& filename);

} // namespace parse




