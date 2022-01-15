//
// Created by Wiktor on 14.01.2022.
//


#include "reports.hpp"

bool SpecificTurnsReportNotifier::should_generate_report(Time t) {
    if (turns_.find(t) != turns_.end()) {
        return true;
    }
    return false;
}

bool IntervalReportNotifier::should_generate_report(Time t) {
    if (!(t - 1 % to_)) {
        return true;
    }
    return false;
}

void generate_structure_report(const Factory& f, std::ostream& os) {
    os << "\n== LOADING RAMPS ==\n" << std::endl;
    std::for_each(f.ramp_cbegin(),f.ramp_cend(),[&os](const Ramp &r){
        os << "LOADING RAMP #" << r.get_id() << std::endl << "  Delivery interval: "
        << r.get_delivery_interval() << std::endl << "  Receivers:" << std::endl;
        auto pref = r.receiver_preferences_.get_preferences();
        for (auto step : pref) {
            if (step.first->get_receiver_type() == ReceiverType::WORKER) {
                os << "    worker #" << step.first->get_id() << std::endl;
            }
            else {
                os << "    storehouse #" << step.first->get_id() << std::endl;
            }
        }
        os << std::endl;
    });

    os << "\n== WORKERS ==\n" << std::endl;
    std::for_each(f.worker_cbegin(),f.worker_cend(),[&os](const Worker &w){
        os << "WORKER #" << w.get_id() << std::endl << "  Processing time: "
           << w.get_processing_duration() << std::endl << "  Queue type: ";
        if (queue_type(w.get_queue()->get_queue_type()) == "LIFO") {
            os << "LIFO" << std::endl;
        }
        else {
            os << "FIFO" << std::endl;
        }
        os << "  Receivers:" << std::endl;
        auto pref = w.receiver_preferences_.get_preferences();
        for (auto step : pref) {
            if (step.first->get_receiver_type() == ReceiverType::WORKER) {
                os << "    worker #" << step.first->get_id() << std::endl;
            }
            else {
                os << "    storehouse #" << step.first->get_id() << std::endl;
            }
        }
        os << std::endl;
    });

    os << "\n== STOREHOUSES ==\n" << std::endl;
    std::for_each(f.storehouse_cbegin(),f.storehouse_cend(),[&os](const Storehouse &s){
        os << "STOREHOUSE #" << s.get_id() << std::endl;
        os << std::endl;
    });
}

void generate_simulation_turn_report(const Factory& f,std::ostream& os, Time t) {
    os << "=== [ Turn: " + std::to_string(t) + " ] ===\n" << std::endl;
    os << "== WORKERS ==\n" << std::endl;
    std::for_each(f.worker_cbegin(),f.worker_cend(),[&os, &t](const Worker &w) {
        os << "WORKER #" << w.get_id() << std::endl;
        os << "  PBuffer: ";
        if (!w.get_processing_buffer()) { os << "(empty)" << std::endl; }
        else { auto diff = t - w.get_package_processing_start_time() + 1;
            os << "#" << w.get_processing_buffer()->get_id() << " (pt = " << diff << ")" << std::endl; }
        os << "  Queue:";
        if (w.get_queue()->empty()) { os << " (empty)" << std::endl; }
        else {
            std::for_each(w.get_queue()->cbegin(), w.get_queue()->cend(),
                          [&os](const Package& p) { os << " #" << p.get_id(); });
            os << std::endl;
        }
        os << "  SBuffer:";
        if (!w.get_sending_buffer()) { os << " (empty)" << std::endl; }
        else { os << " #" << w.get_sending_buffer()->get_id() << std::endl; }
        os << std::endl;
    });
    os << "\n== STOREHOUSES ==\n" << std::endl;
    std::for_each(f.storehouse_cbegin(), f.storehouse_cend(), [&os](const Storehouse &s){
        os << "STOREHOUSE #" << s.get_id() << std::endl;
        os << "  Stock:";
        if (s.get_queue()->empty()) { os << " (empty)" << std::endl; }
        else {
            std::for_each(s.get_queue()->cbegin(), s.get_queue()->cend(), [&os](const Package &p){
                os << " #" << p.get_id();
            });
            os << std::endl;
        }
    });
    os << std::endl;
}
