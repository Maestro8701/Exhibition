#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "utils.h"
#include "list.h"


int main() {
    setlocale(LC_ALL, "Russian");
    const std::string input_file = "inlet.in";
    const std::string output_file = "outlet.out";

    std::vector<std::pair<std::string, int32_t>> entries = parse::LoadFromFile(input_file);
    if (entries.size() == 0) {
        return 1;
    }

    LinkedList list;
    list.Build(entries);

    if (!list.Serialize(output_file)) { std::cerr << "Сериализация не удалась.\n"; return 1; }

    LinkedList restored;
    if (!restored.Deserialize(output_file)) { std::cerr << "Десериализация не удалась.\n"; return 1; }

    if (restored.Compare(entries))
        std::cout << "Сравнение прошло успешно.\n";
    else
        std::cerr << "Сравнение не совпало.\n";

    restored.DumpText("dump.txt");
    std::cout << "Дамп записан в dump.txt\n";

    return 0;
}

