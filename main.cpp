//
// Created by Wiktor on 19.12.2021.
//
#include "factory.hpp"


int main() {
    std::string abba = "LINK src=ramp-1 dest=worker-1";
    auto ojcze = parse_line(abba);
    bool scr = (ojcze.element_type) == ElementType::WORKER;
    std::cout << scr << std::endl;

    for (const auto& i : ojcze.parameters) {
        std::cout << i.first << std::endl;
        std::cout << i.second << std::endl;
    }
}
