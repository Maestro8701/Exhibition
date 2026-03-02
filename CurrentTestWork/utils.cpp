#include <iostream>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

#include "utils.h"

namespace parse {

    bool ParseLine(const std::string& line, std::string& out_data, int32_t& out_rand_index) {
        try {
            std::size_t sep = line.rfind(';');
            if (sep == std::string::npos) {
                return false;
            }

            out_data = line.substr(0, sep);

            out_rand_index = std::stoi(line.substr(sep + 1));
        }
        catch (...) {
            return false;
        }
        return true;
    }

    std::vector<std::pair<std::string, int32_t>> LoadFromFile(const std::string& filename) {
        std::vector<std::pair<std::string, int32_t>> entries;
        std::ifstream infile(filename);
        if (!infile) {
            std::cerr << "Ошибка чтения входного файла '" << filename << "'.\n";
            return entries; 
        }

        std::string line;
        while (std::getline(infile, line)) {
            if (line.empty()) continue;
            std::string data;
            int32_t rand_idx;
            if (!ParseLine(line, data, rand_idx)) {
                std::cerr << "Некорректная строка: " << line << '\n';
                continue;
            }
            entries.emplace_back(std::move(data), rand_idx);
        }
        infile.close();
        return entries;
    }

} // namespace parse
