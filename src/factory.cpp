//
// Created by Wiktor on 04.01.2022.
//
#include "factory.hpp"

//bool Factory::is_consistent() const {
//    std::map<const PackageSender*, NodeColor> color;
//    NodeCollection<Ramp>::const_iterator node_;
//
//    for (node_ = ramp_cbegin(); node_ != ramp_.end(); ++node_) {
//        color.insert({*node_, NodeColor::UNVISITED});
//    }
//}
NodeCollection<Ramp>::iterator Factory::find_ramp_by_id(ElementID id) {
    NodeCollection<Ramp>::iterator found;
    found = std::find_if(ramp_.begin(), ramp_.end(), [&](const Ramp& val){ return val.get_id() == id; } );
    return found;
}

[[nodiscard]] NodeCollection<Ramp>::const_iterator Factory::find_ramp_by_id(ElementID id) const {
    NodeCollection<Ramp>::const_iterator found;
    found = std::find_if(ramp_.begin(), ramp_.end(), [&](const Ramp& val){ return val.get_id() == id; } );
    return found;
}

NodeCollection<Worker>::iterator Factory::find_worker_by_id(ElementID id) {
    NodeCollection<Worker>::iterator found;
    found = std::find_if(worker_.begin(), worker_.end(), [&](const Worker& val){ return val.get_id() == id; } );
    return found;
}

[[nodiscard]] NodeCollection<Worker>::const_iterator Factory::find_worker_by_id(ElementID id) const {
    NodeCollection<Worker>::const_iterator found;
    found = std::find_if(worker_.begin(), worker_.end(), [&](const Worker& val){ return val.get_id() == id; } );
    return found;
}

NodeCollection<Storehouse>::iterator Factory::find_storehouse_by_id(ElementID id) {
    NodeCollection<Storehouse>::iterator found;
    found = std::find_if(storehouse_.begin(), storehouse_.end(), [&](const Storehouse& val){ return val.get_id() == id; } );
    return found;
}

[[nodiscard]] NodeCollection<Storehouse>::const_iterator Factory::find_storehouse_by_id(ElementID id) const {
    NodeCollection<Storehouse>::const_iterator found;
    found = std::find_if(storehouse_.begin(), storehouse_.end(), [&](const Storehouse& val){ return val.get_id() == id; } );
    return found;
}

void Factory::do_deliveries(Time t)
{
    NodeCollection<Ramp>::iterator node_;
    for (node_ = ramp_.begin(); node_ != ramp_.end(); ++node_)
    {
        node_->deliver_goods(t);
    }
}
