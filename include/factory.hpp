//
// Created by Wiktor on 03.01.2022.
//

#ifndef IMPLEMENTATION_FACTORY_HPP
#define IMPLEMENTATION_FACTORY_HPP

#endif //IMPLEMENTATION_FACTORY_HPP

class Factory {
public:
    virtual void add_ramp(Ramp&&) = 0;
    virtual void remove_ramp(id: ElementID) = 0;
    NodeCollection<Ramp>::iterator find_ramp_by_id(id: ElementID);
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(id: ElementID);
    NodeCollection<Ramp>::const_iterator ramp_cbegin();
    NodeCollection<Ramp>::const_iterator ramp_cend();
    void remove_receiver(collection: NodeCollection<Node>&, id: ElementID) : void;

    virtual void add_worker(worker&&) = 0;
    virtual void remove_worker(id: ElementID) = 0;
    NodeCollection<worker>::iterator find_worker_by_id(id: ElementID);
    NodeCollection<worker>::const_iterator find_worker_by_id(id: ElementID);
    NodeCollection<worker>::const_iterator worker_cbegin();
    NodeCollection<worker>::const_iterator worker_cend();
    void remove_receiver(collection: NodeCollection<Node>&, id: ElementID);

    virtual void add_storehouses(storehouses&&) = 0;
    virtual void remove_storehouses(id: ElementID) = 0;
    NodeCollection<storehouses>::iterator find_storehouses_by_id(id: ElementID);
    NodeCollection<storehouses>::const_iterator find_storehouses_by_id(id: ElementID);
    NodeCollection<storehouses>::const_iterator storehouses_cbegin();
    NodeCollection<storehouses>::const_iterator storehouses_cend();
    void remove_receiver(collection: NodeCollection<Node>&, id: ElementID);

    is_consistent(void) : bool {query}

    do_deliveries(Time) : void

    do_package_passing(void) : void

    do_work(Time) : void
};

class NodeCollection<Node> {
public:
    virtual void add(node: Node&&) = 0;
    virtual void remove_by_id(id: ElementID) = 0;
    NodeCollection<Node>::iterator find_by_id(id: ElementID);
    NodeCollection<Node>::const_iterator find_by_id(id: ElementID);
};