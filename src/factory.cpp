//
// Created by Wiktor on 04.01.2022.
//
#include "factory.hpp"

//bool Factory::is_consistent() const {
//    using mapa = std::map<const PackageSender*, NodeColor>;
//    mapa color = mapa();
//
//    for (auto& node : ramp_) {
//        color.emplace(&node, NodeColor::UNVISITED);
//    }
//}

void Factory::do_deliveries(Time t)
{
    NodeCollection<Ramp>::iterator node_;
    for (node_ = ramp_.begin(); node_ != ramp_.end(); ++node_)
    {
        node_->deliver_goods(t);
    }
}

void Factory::do_package_passing()
{
    NodeCollection<Ramp>::iterator node_;
    for (node_ = ramp_.begin(); node_ != ramp_.end(); ++node_)
    {
        node_->send_package();
    }
    NodeCollection<Worker>::iterator node1_;
    for (node1_ = worker_.begin(); node1_ != worker_.end(); ++node1_)
    {
        if (!node1_->get_package_processing_start_time())
        {
            node1_->send_package();
        }
    }
}

void Factory::do_work(Time t)
{
    NodeCollection<Worker>::iterator node1_;
    for (node1_ = worker_.begin(); node1_ != worker_.end(); ++node1_)
    {
        node1_->do_work(t);
    }
}
