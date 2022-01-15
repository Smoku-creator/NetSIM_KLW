/*
Created by Antoni 11.01.2022
*/

#ifndef IMPLEMENTATION_REPORTS_HPP
#define IMPLEMENTATION_REPORTS_HPP

#include "factory.hpp"

class IntervalReportNotifier
{
public:
    explicit IntervalReportNotifier(TimeOffset to) : to_(to) {}
    bool should_generate_report(Time t);
private:
    TimeOffset to_;
};

class SpecificTurnsReportNotifier
{
public:
    explicit SpecificTurnsReportNotifier(std::set<Time> turn) :
            turns_(std::move(turn)) {}
    bool should_generate_report(Time t);
private:
    std::set<Time> turns_;
};

void generate_structure_report(const Factory&, std::ostream&);
void generate_simulation_turn_report(const Factory&,std::ostream&, Time);

#endif //IMPLEMENTATION_REPORTS_HPP
