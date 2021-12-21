//
// Created by Wiktor on 19.12.2021.
//

#include "storage_types.hpp"


#include <iostream>

int main() {
//    Package p1(1);
//    Package p2(3);
//
//    std::cout << "Package ID p1:" << p1.get_id() << std::endl;
//    std::cout << "Package ID p2:" << p2.get_id() << std::endl;
//    {
//        Package p3;
//        std::cout << "Package ID p3:" << p3.get_id() << std::endl;
//    }
//    Package p4;
//
//    Package p5(std::move(p1));
//
//    std::cout << "Package ID p4:" << p4.get_id() << std::endl;
//    std::cout << "Package ID p5:" << p5.get_id() << std::endl;
//
//    Package p6 = std::move(p2);
//    Package p7 = std::move(p5);
//
//    std::cout << "Package ID p6:" << p6.get_id() << std::endl;
//    std::cout << "Package ID p7:" << p7.get_id() << std::endl;
    PackageQueue q(PackageQueueType::FIFO);
    std::cout << "Czy jestem pusta? ";
    if (q.empty()) {
        std::cout << "Tak!" << std::endl;
    }
    else {
        std::cout << "Nie!" << std::endl;
    }

    std::cout << "Co to za kolejka? ";
    if (q.get_queue_type() == PackageQueueType::FIFO) {
        std::cout << "FIFO" << std::endl;
    }
    else {
        std::cout << "LIFO" << std::endl;
    }

    q.push(Package(1));
    q.push(Package(2));

    std::cout << "Rozmiar kolejki: " << q.size() << std::endl;
    std::cout << "Czy jestem pusta? ";
    if (q.empty()) {
        std::cout << "Tak!" << std::endl;
    }
    else {
        std::cout << "Nie!" << std::endl;
    }

    for (auto & i : q) {
        std::cout << "Moje unikalne ID to: " << i.get_id() << std::endl;
    }

    Package p(q.pop());

    std::cout << "Usunieto: " << p.get_id() << std::endl;
    for (auto & i : q) {
        std::cout << "Moje unikalne ID to: " << i.get_id() << std::endl;
    }

    p = q.pop();

    std::cout << "Usunieto: " << p.get_id() << std::endl;
}
