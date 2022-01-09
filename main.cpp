//
// Created by Wiktor on 19.12.2021.
//

#include "nodes.hpp"


#include <iostream>

int main() {
    std::vector<int> dob;
    dob.push_back(1);
    dob.push_back(2);
    std::vector<int>::iterator it;
    it = dob.begin();
    std::cout << *it << std::endl;
    *it = *it + 1;
    std::cout << *it << std::endl;
}
