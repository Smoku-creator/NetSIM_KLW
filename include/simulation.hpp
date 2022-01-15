//
// Created by Wiktor on 14.01.2022.
//

#ifndef IMPLEMENTATION_SIMULATION_HPP
#define IMPLEMENTATION_SIMULATION_HPP

#include <utility>

#include "reports.hpp"

void simulate(Factory &f, TimeOffset d, const std::function<void(Factory&, Time)>&);

#endif //IMPLEMENTATION_SIMULATION_HPP
