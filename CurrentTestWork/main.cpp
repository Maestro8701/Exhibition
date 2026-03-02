#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "utils.h"
#include "list.h"

int main() {
    setlocale(LC_ALL, "Russian");
    //на стал усложнять 2 строки снизу можно перенести в отдельный файл конфига используя библиотеку tomlplusplus и cmake(скачивание и сборка через cmake)
    //там можно добавить аргументы и два типа запуска первый стандартный где все работает нормально  и нет в main Compare и DumpText
    // второй где Compare и DumpText присутствуют и запуск идет через другой файл(тут так не реализовано чтобы не усложнять)
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

