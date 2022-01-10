//
// Created by Wiktor on 04.01.2022.
//
#include "factory.hpp"

bool Factory::is_consistent() const {
    std::map<const PackageSender*, NodeColor> color;

    for (auto& node : ramp_) {
        color.emplace(&node, NodeColor::UNVISITED);
    }
    for (auto& node : worker_) {
        color.emplace(&node, NodeColor::UNVISITED);
    }

    try
    {
        for (auto& colored : color) {
            if (!has_reachable_storehouse(colored.first,color)) {
                throw std::logic_error("This PackageSender doesn't have any connection!");
            }
        }
    }
    catch(const std::logic_error& logic_err) {
        return false;
    }
    return true;
}

//bool Factory::has_reachable_storehouse(const PackageSender* sender,
//                                       std::map<const PackageSender*, NodeColor>& node_colors) const {
//    if (node_colors.at(sender) == NodeColor::VERIFIED) { return true; }
//    node_colors.at(sender) = NodeColor::VISITED;
//    if (sender->receiver_preferences_.get_preferences().empty())
//    {
//        throw std::logic_error("There is no receiver for this sender!");
//    }
//    bool any_diff_than_sender = false;
//    for (auto receiver : sender->receiver_preferences_.get_preferences()) {
//        if (receiver.first->get_receiver_type() == ReceiverType::STOREHOUSE) {
//            any_diff_than_sender = true;
//        }
//        else if (receiver.first->get_receiver_type() == ReceiverType::WORKER) {
//            IPackageReceiver* receiver_ptr = receiver.first;
//            auto worker_ptr = dynamic_cast<Worker*>(receiver_ptr);
//            auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);
//            if (sender == sendrecv_ptr) {
//                sendrecv_ptr += 1;
//            }
//            any_diff_than_sender = true;
//            if (node_colors.at(sendrecv_ptr) == NodeColor::UNVISITED) {
//                has_reachable_storehouse(sendrecv_ptr,node_colors);
//            }
//        }
//    }
//    node_colors.at(sender) = NodeColor::VERIFIED;
//    if (!any_diff_than_sender) {
//        return true;
//    }
//    throw std::logic_error("This sender doesn't have any reachable storehouse!");
//}

bool Factory::has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors) const{
    if (node_colors[sender] == NodeColor::VERIFIED) {
        return true;
    }
    node_colors[sender] = NodeColor::VISITED;

    if (sender->receiver_preferences_.get_preferences().empty()) {
        throw std::logic_error("No defined receivers.");
    }

    if (std::find_if(sender->receiver_preferences_.begin(), sender->receiver_preferences_.end(),
                     [&sender](auto & elem){ return dynamic_cast<PackageSender*>(elem.first) != sender; }) == sender->receiver_preferences_.end()) {

        for (auto& receiver : sender->receiver_preferences_.get_preferences()) {

            if (receiver.first->get_receiver_type() == ReceiverType::STOREHOUSE) {
                if (std::find_if(sender->receiver_preferences_.begin(), sender->receiver_preferences_.end(),
                                 [&sender](auto& elem) {
                                     return dynamic_cast<PackageSender*>(elem.first) != sender;
                                 }) != sender->receiver_preferences_.end())
                    return true;

            }

            else if (receiver.first->get_receiver_type() == ReceiverType::WORKER) {
                IPackageReceiver* receiver_ptr = receiver.first;
                auto worker_ptr = dynamic_cast<Worker*>(receiver_ptr);
                auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);

                if (sendrecv_ptr == sender)
                    continue;
                if (std::find_if(sender->receiver_preferences_.begin(), sender->receiver_preferences_.end(),
                                 [&sender](auto& elem) {
                                     return dynamic_cast<PackageSender*>(elem.first) != sender;
                                 }) != sender->receiver_preferences_.end()) {
                    return true;
                }

                if (node_colors[sendrecv_ptr] == NodeColor::UNVISITED)
                    has_reachable_storehouse(sendrecv_ptr, node_colors);
            }
        }
    }
    node_colors[sender] = NodeColor::VERIFIED;

    if (std::find_if(sender->receiver_preferences_.begin(), sender->receiver_preferences_.end(),
                     [&sender](auto & elem){ return dynamic_cast<PackageSender*>(elem.first) != sender; }) != sender->receiver_preferences_.end())
        return true;
    else
        throw std::logic_error("No defined receivers");
}

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

