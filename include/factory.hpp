//
// Created by Wiktor on 03.01.2022.
//

#ifndef IMPLEMENTATION_FACTORY_HPP
#define IMPLEMENTATION_FACTORY_HPP

#include "nodes.hpp"
#include <iostream>
#include <sstream>
#include <fstream>

enum class ElementType
{
    RAMP,
    WORKER,
    STOREHOUSE,
    LINK
};

extern std::map<std::string,ElementType> TypeMap;

struct ParsedLineData
{
    ElementType element_type;
    std::map<std::string,std::string> parameters;
};

ParsedLineData parse_line(const std::string& line);

enum class NodeColor
{
    UNVISITED,
    VISITED,
    VERIFIED
};

template <class Node>
class NodeCollection
{
public:
    using container_t = typename std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    void add(Node &&node);
    void remove_by_id(ElementID id);

    iterator find_by_id(ElementID id)
    {
        return std::find_if(collection_.begin(), collection_.end(), [&id](const Node &val)
                            { return val.get_id() == id; });
    }

    [[nodiscard]] const_iterator find_by_id(ElementID id) const
    {
        return std::find_if(collection_.cbegin(), collection_.cend(),
                            [&id](const Node &val)
                            { return val.get_id() == id; });
    }

    [[nodiscard]] const_iterator cbegin() const { return collection_.cbegin(); }
    [[nodiscard]] const_iterator cend() const { return collection_.cend(); }
    [[nodiscard]] const_iterator end() const { return collection_.end(); }
    [[nodiscard]] const_iterator begin() const { return collection_.begin(); }
    [[nodiscard]] iterator end() { return collection_.end(); }
    [[nodiscard]] iterator begin() { return collection_.begin(); }

private:
    container_t collection_;
};

template <class Node>
void NodeCollection<Node>::add(Node &&node)
{
    collection_.push_back(std::move(node));
}

template <class Node>
void NodeCollection<Node>::remove_by_id(ElementID id)
{
    auto it = std::find_if(collection_.begin(), collection_.end(), [&id](const Node &val)
                           { return val.get_id() == id; });
    if (it != collection_.end())
    {
        collection_.erase(it);
    }
}

class Factory
{
public:
    void add_ramp(Ramp &&p) { ramp_.add(std::move(p)); }
    void remove_ramp(ElementID id) { ramp_.remove_by_id(id); }

    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) { return ramp_.find_by_id(id); }
    [[nodiscard]] NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const { return ramp_.find_by_id(id); }
    [[nodiscard]] NodeCollection<Ramp>::const_iterator ramp_cbegin() const { return ramp_.cbegin(); }
    [[nodiscard]] NodeCollection<Ramp>::const_iterator ramp_cend() const { return ramp_.cend(); }

    void add_worker(Worker &&p) { worker_.add(std::move(p)); }
    void remove_worker(ElementID id)
    {
        remove_receiver(worker_, id);
        remove_receiver(ramp_, id);
        worker_.remove_by_id(id);
    }

    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) { return worker_.find_by_id(id); }
    [[nodiscard]] NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const { return worker_.find_by_id(id); }
    [[nodiscard]] NodeCollection<Worker>::const_iterator worker_cbegin() const { return worker_.cbegin(); }
    [[nodiscard]] NodeCollection<Worker>::const_iterator worker_cend() const { return worker_.cend(); }

    void add_storehouse(Storehouse &&p) { storehouse_.add(std::move(p)); }
    void remove_storehouse(ElementID id)
    {
        remove_receiver(worker_, id);
        storehouse_.remove_by_id(id);
    }

    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) { return storehouse_.find_by_id(id); }
    [[nodiscard]] NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const { return storehouse_.find_by_id(id); }
    [[nodiscard]] NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const { return storehouse_.cbegin(); }
    [[nodiscard]] NodeCollection<Storehouse>::const_iterator storehouse_cend() const { return storehouse_.cend(); }

    [[nodiscard]] bool is_consistent() const;
    bool has_reachable_storehouse(const PackageSender *sender, std::map<const PackageSender *, NodeColor> &node_colors) const;

    void do_deliveries(Time t);

    void do_package_passing();

    void do_work(Time t);

private:
    template <class Node>
    void remove_receiver(NodeCollection<Node> &collection, ElementID id)
    {
        for (auto &node : collection)
        {
            auto map = node.receiver_preferences_.get_preferences();
            for (auto &it : map)
            {
                if (it.first->get_id() == id)
                {
                    node.receiver_preferences_.remove_receiver(it.first);
                }
            }
        }
    }

    NodeCollection<Ramp> ramp_;
    NodeCollection<Worker> worker_;
    NodeCollection<Storehouse> storehouse_;
};


Factory load_factory_structure(std::istream& is);


#endif //IMPLEMENTATION_FACTORY_HPP
