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

ParsedLineData parse_line(const std::string& line) {
    std::istringstream token_stream(line);
    std::vector<std::string> tokens;
    char delimiter = ' ';
    char delim2 = '=';
    std::string token;
    ParsedLineData data;
    std::vector<std::string> temp;
    int i = 0;

    while (std::getline(token_stream, token, delimiter)) {
        if (!i) {
            data.element_type = TypeMap.find(token)->second;
        }
        else {
            tokens.push_back(token);
        }
        ++i;
    }

    for (const auto& j : tokens) {
        std::istringstream tokens_stream(j);
        while (std::getline(tokens_stream, token, delim2)) {
            temp.push_back(token);
        }
        data.parameters.insert({*temp.begin(),*temp.rbegin()});
//        delivery-interval wyświetla się pierwsze po wypisaniu
        temp.clear();
    }

    return data;
}

std::map<std::string,ElementType> TypeMap = {
        {"LOADING_RAMP", ElementType::RAMP},
        {"WORKER", ElementType::WORKER},
        {"STOREHOUSE",ElementType::STOREHOUSE},
        {"LINK",ElementType::LINK}
};

Factory load_factory_structure(std::istream& is) {
    Factory factory;
    std::string line;
    ParsedLineData cont;

    while (std::getline(is, line)) {
        if (line[0] != ';' && !line.empty()) {
            cont = parse_line(line);
            switch (cont.element_type) {
                case ElementType::RAMP: {
                    factory.add_ramp(Ramp(std::stoi(cont.parameters.rbegin()->second),
                                          std::stoi(cont.parameters.begin()->second)));
                    break;
                }
                case ElementType::WORKER: {
                    if (queue_type(PackageQueueType::FIFO) == cont.parameters.rbegin()->second) {
                        factory.add_worker(Worker(std::stoi(cont.parameters.begin()->second),
                                                  std::stoi(std::next(cont.parameters.begin())->second),
                                                  std::make_unique<PackageQueue>(PackageQueueType::FIFO)));
                    }
                    else {
                        factory.add_worker(Worker(std::stoi(cont.parameters.begin()->second),
                                                  std::stoi(std::next(cont.parameters.begin())->second),
                                                  std::make_unique<PackageQueue>(PackageQueueType::LIFO)));
                    }
                    break;
                }
                case ElementType::STOREHOUSE: {
                    factory.add_storehouse(Storehouse(std::stoi(cont.parameters.begin()->second)));
                    break;
                }
                case ElementType::LINK: {
                    char delim3 = '-';
                    std::string token;
                    std::vector<std::string> new_things;
                    std::vector<std::string> temp;
                    new_things.push_back(cont.parameters.rbegin()->second);
                    new_things.push_back(cont.parameters.begin()->second);
                    for (const auto& i : new_things) {
                        std::istringstream tokens_stream(i);
                        while (std::getline(tokens_stream, token, delim3)) {
                            temp.push_back(token);
                        }
                    }
                    int id_1 = std::stoi(*std::next(temp.begin()));
                    int id_2 = std::stoi(temp.back());
                    if (*temp.front().begin() == 'r') {
                        Ramp &r = *(factory.find_ramp_by_id(id_1));
                        if (*(std::next(std::next(temp.begin())))->begin() == 'w') {
                            Worker &w = *(factory.find_worker_by_id(id_2));
                            r.receiver_preferences_.add_receiver(&w);
                        }
                        else {
                            Storehouse &s = *(factory.find_storehouse_by_id(id_2));
                            r.receiver_preferences_.add_receiver(&s);
                        }
                    }
                    else {
                        Worker &w = *(factory.find_worker_by_id(id_1));
                        if (*(std::next(std::next(temp.begin())))->begin() == 'w') {
                            Worker &w1 = *(factory.find_worker_by_id(id_2));
                            w.receiver_preferences_.add_receiver(&w1);
                        }
                        else {
                            Storehouse &s = *(factory.find_storehouse_by_id(id_2));
                            w.receiver_preferences_.add_receiver(&s);
                        }
                    }
                    break;
                }
            }
        }
    }
    return factory;
}
