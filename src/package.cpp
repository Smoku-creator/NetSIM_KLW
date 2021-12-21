#include "package.hpp"


// Initialization of static members
std::set<ElementID> Package::assigned_IDs = {};
std::set<ElementID> Package::freed_IDs = {};

// Initialization of the first constructor
Package::Package() {
    if (freed_IDs.empty()) {
        if (assigned_IDs.empty()) {
            semi_prod_ = 1;
        }
        else {
//            Checking for the biggest ID assigned
            semi_prod_ = *(assigned_IDs.rbegin()) + 1;
        }
    }
    else {
//        Checking for the smallest ID freed
        semi_prod_ = *freed_IDs.begin();
//        Erasing that ID from static member 'freed_IDs'
        freed_IDs.erase(semi_prod_);
    }
//    Inserting that ID to static member 'assigned_IDs'
    assigned_IDs.insert(semi_prod_);
}


Package::Package(Package&& to_move) noexcept {
    semi_prod_ = to_move.semi_prod_;
    to_move.semi_prod_ = err_;
}


// Initialization of the destructor
Package::~Package() {
//    When the life of a Package ends, withdraw from assigned_IDs and insert to freed_IDs
    if (semi_prod_ != err_) {
        assigned_IDs.erase(semi_prod_);
        freed_IDs.insert(semi_prod_);
    }
}


// Initialization of the second constructor
Package::Package(ElementID m) {
//    Checking for m in assigned_IDs and freed_IDs
    auto i = assigned_IDs.find(m) != assigned_IDs.end();
    auto j = freed_IDs.find(m) != freed_IDs.end();
//    If found in neither of them, create a new Package ID and insert it to assigned_IDs
    if (!i && !j) {
        semi_prod_ = m;
        assigned_IDs.insert(semi_prod_);
    }
//    If appeared only in assigned_IDs
    else if (!i && j) {
        semi_prod_ = m;
        freed_IDs.erase(semi_prod_);
        assigned_IDs.insert(semi_prod_);
    }
    else {
        throw std::invalid_argument("ID already in use.");
    }
}

Package &Package::operator=(Package&& alfa) noexcept {
    if (semi_prod_ != err_) {
        assigned_IDs.erase(semi_prod_);
        freed_IDs.insert(semi_prod_);
    }
    semi_prod_ = alfa.semi_prod_;
    alfa.semi_prod_ = err_;
    return *this;
}
