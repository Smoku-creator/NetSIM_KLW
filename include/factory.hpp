//
// Created by Wiktor on 03.01.2022.
//

#ifndef IMPLEMENTATION_FACTORY_HPP
#define IMPLEMENTATION_FACTORY_HPP

#endif //IMPLEMENTATION_FACTORY_HPP

class Factory {
public:
    add_ramp(Ramp&&) : void
    remove_ramp(id: ElementID) : void
    find_ramp_by_id(id: ElementID) : NodeCollection<Ramp>::iterator
    find_ramp_by_id(id: ElementID) : NodeCollection<Ramp>::const_iterator {query}
    ramp_cbegin() : NodeCollection<Ramp>::const_iterator {query}
    ramp_cend() : NodeCollection<Ramp>::const_iterator {query}
    remove_receiver(collection: NodeCollection<Node>&, id: ElementID) : void

    add_worker(worker&&) : void
    remove_worker(id: ElementID) : void
    find_worker_by_id(id: ElementID) : NodeCollection<worker>::iterator
    find_worker_by_id(id: ElementID) : NodeCollection<worker>::const_iterator {query}
    worker_cbegin() : NodeCollection<worker>::const_iterator {query}
    worker_cend() : NodeCollection<worker>::const_iterator {query

    add_storehouse(storehouse&&) : void
    remove_storehouse(id: ElementID) : void
    find_storehouse_by_id(id: ElementID) : NodeCollection<storehouse>::iterator
    find_storehouse_by_id(id: ElementID) : NodeCollection<storehouse>::const_iterator {query}
    storehouse_cbegin() : NodeCollection<storehouse>::const_iterator {query}
    storehouse_cend() : NodeCollection<storehouse>::const_iterator {query

    is_consistent(void) : bool {query}

    do_deliveries(Time) : void

    do_package_passing(void) : void

    do_work(Time) : void
};

class NodeCollection<Node> {
public:
    add(node: Node&&) : void
    remove_by_id(id: ElementID) : void
    find_by_id(id: ElementID) : NodeCollection<Node>::iterator
    find_by_id(id: ElementID) : NodeCollection<Node>::const_iterator {query}
};