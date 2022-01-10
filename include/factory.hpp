//
// Created by Wiktor on 03.01.2022.
//

#ifndef IMPLEMENTATION_FACTORY_HPP
#define IMPLEMENTATION_FACTORY_HPP

#include "nodes.hpp"


enum class NodeColor { UNVISITED, VISITED, VERIFIED };

template <class Node>
class NodeCollection
{
public:
    using container_t = typename std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    void add(Node && node);
    void remove_by_id(ElementID id) { id = id + 1; }

    iterator find_by_id(ElementID id) {return std::find_if(collection_.begin(),
collection_.end(), [&id](const Node& val){ return val.get_id() == id; }); }

    [[nodiscard]] const_iterator find_by_id(ElementID id) const {return
    std::find_if(collection_.cbegin(), collection_.cend(),
[&id](const Node& val){ return val.get_id() == id; });}

    [[nodiscard]] const_iterator cbegin() const { return collection_.cbegin(); }
    [[nodiscard]] const_iterator cend() const { return collection_.cend(); }
    [[nodiscard]] const_iterator end() const { return collection_.end(); }
    [[nodiscard]] const_iterator begin() const { return collection_.begin(); }
    [[nodiscard]] iterator end() { return collection_.end(); }
    [[nodiscard]] iterator begin() { return collection_.begin(); }
    container_t collection_;
};

template<class Node>
void NodeCollection<Node>::add(Node && node) {
    collection_.emplace_back(std::move(node));
}

class Factory
{
public:
    void add_ramp(Ramp&& p) {ramp_.add(std::move(p));}
    void remove_ramp(ElementID id) { id = id + 1; }
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) { return ramp_.find_by_id(id); }
    [[nodiscard]] NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const { return ramp_.find_by_id(id); }
    [[nodiscard]] NodeCollection<Ramp>::const_iterator ramp_cbegin() const { return ramp_.cbegin(); }
    [[nodiscard]] NodeCollection<Ramp>::const_iterator ramp_cend() const { return ramp_.cend(); }

    void add_worker(Worker&& p) { worker_.add(std::move(p)); }
    void remove_worker(ElementID id) { id = id + 1; }
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) { return worker_.find_by_id(id); }
    [[nodiscard]] NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const { return worker_.find_by_id(id); }
    [[nodiscard]] NodeCollection<Worker>::const_iterator worker_cbegin() const { return worker_.cbegin(); }
    [[nodiscard]] NodeCollection<Worker>::const_iterator worker_cend() const { return worker_.cend(); }

    void add_storehouse(Storehouse&& p) { storehouse_.add(std::move(p)); }
    void remove_storehouse(ElementID id) { id = id + 1; }
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) { return storehouse_.find_by_id(id); }
    [[nodiscard]] NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const { return storehouse_.find_by_id(id); }
    [[nodiscard]] NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const { return storehouse_.cbegin(); }
    [[nodiscard]] NodeCollection<Storehouse>::const_iterator storehouse_cend() const { return storehouse_.cend(); }

    [[nodiscard]] bool is_consistent() const;
    bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors) const;

    void do_deliveries(Time t);

    void do_package_passing();

    void do_work(Time t);

private:
//    void remove_receiver(NodeCollection<Worker>& collection, ElementID id);
//    void remove_receiver(NodeCollection<Storehouse>& collection, ElementID id);
    NodeCollection<Ramp> ramp_;
    NodeCollection<Worker> worker_;
    NodeCollection<Storehouse> storehouse_;
};

#endif //IMPLEMENTATION_FACTORY_HPP
