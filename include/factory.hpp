//
// Created by Wiktor on 03.01.2022.
//

#ifndef IMPLEMENTATION_FACTORY_HPP
#define IMPLEMENTATION_FACTORY_HPP

#include "nodes.hpp"
#include <vector>

enum class NodeColor { UNVISITED, VISITED, VERIFIED };

template <class Node>
class NodeCollection
{
public:
    using container_t = typename std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    void add(Node&& node) { _collection.template emplace_back(std::move(node)); }
    void remove_by_id(ElementID id) {}
    iterator find_by_id(ElementID id) {auto missing =
    std::find_if(_collection.begin(), _collection.end(), [&](const Node& val){ return val.get_id() == id; }); return missing; }
    [[nodiscard]] const_iterator find_by_id(ElementID id) const {auto missing =
    std::find_if(_collection.begin(), _collection.end(), [&](const Node& val){ return val.get_id() == id; }); return missing; }
    [[nodiscard]] const_iterator cbegin() { return _collection.cbegin(); }
    [[nodiscard]] const_iterator cend() { return _collection.cend(); }
    iterator end() { return _collection.end(); }
    iterator begin() { return _collection.begin(); }
    container_t _collection;
};

class Factory
{
public:
    void add_ramp(Ramp&& p) { ramp_.emplace_back(std::move(p)); }
    void remove_ramp(ElementID id) {}
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id);
    [[nodiscard]] NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const;
    [[nodiscard]] NodeCollection<Ramp>::const_iterator ramp_cbegin() const { return ramp_.cbegin(); }
    [[nodiscard]] NodeCollection<Ramp>::const_iterator ramp_cend() const { return ramp_.cend(); }

    void add_worker(Worker&& p) { worker_.emplace_back(std::move(p)); }
    void remove_worker(ElementID id) {}
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id);
    [[nodiscard]] NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const;
    [[nodiscard]] NodeCollection<Worker>::const_iterator worker_cbegin() const { return worker_.cbegin(); }
    [[nodiscard]] NodeCollection<Worker>::const_iterator worker_cend() const { return worker_.cend(); }

    void add_storehouse(Storehouse&& p) { storehouse_.emplace_back(std::move(p)); }
    void remove_storehouse(ElementID id) {}
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id);
    [[nodiscard]] NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const;
    [[nodiscard]] NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const { return storehouse_.cbegin(); }
    [[nodiscard]] NodeCollection<Storehouse>::const_iterator storehouse_cend() const { return storehouse_.cend(); }

    [[nodiscard]] bool is_consistent() const {}
    bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors) const {}

    void do_deliveries(Time t);

    void do_package_passing() {}

    void do_work(Time) {}

private:
    void remove_receiver(NodeCollection<Worker>& collection, ElementID id) {}
    void remove_receiver(NodeCollection<Storehouse>& collection, ElementID id) {}
    NodeCollection<Ramp>::container_t ramp_;
    NodeCollection<Worker>::container_t worker_;
    NodeCollection<Storehouse>::container_t storehouse_;
};

#endif //IMPLEMENTATION_FACTORY_HPP
