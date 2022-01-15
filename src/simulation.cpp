/*
Created by Antoni 11.01.2022
*/
#include "simulation.hpp"

void simulate(Factory &f, TimeOffset d, const std::function<void(Factory&, Time)>&) {
    for ( int turn = 1; turn != d + 1; turn++ ) {
//        Pierwszy etap - delivery_goods w rampach
        f.do_deliveries(turn);
//        Drugi etap - produkty od nadawców do odbiorców
        f.do_package_passing();
//        Trzeci etap - przetwarzanie produktów do_work
        f.do_work(turn);
//        Czwarty etap - czy trzeba robić raport?
    }
}
