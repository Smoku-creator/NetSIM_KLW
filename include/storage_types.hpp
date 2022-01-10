#ifndef IMPLEMENTATION_STORAGE_TYPES_HPP
#define IMPLEMENTATION_STORAGE_TYPES_HPP

#include "package.hpp"

#include <list>


enum class PackageQueueType {
    FIFO, LIFO
};

class IPackageStockpile {
public:

//    User-defined aliases for different types of iterators
    using const_iterator = std::list<Package>::const_iterator;
    using iterator = std::list<Package>::iterator;

//    Using 'move constructor' from class Package insert a Package into PackageQueue
    virtual void push(Package&&) = 0;
//    Returns boolean info if PackageQueue is empty or not
    [[nodiscard]] virtual bool empty() const = 0;
//    Returns size of the PackageQueue (aka 'unsigned long long')
    [[nodiscard]] virtual size_t size() const = 0;

//    Declared iterators for 'Read-only' operations
    [[nodiscard]] virtual const_iterator cbegin() const = 0;
    [[nodiscard]] virtual const_iterator cend() const = 0;
    [[nodiscard]] virtual const_iterator begin() const = 0;
    [[nodiscard]] virtual const_iterator end() const = 0;

    virtual ~IPackageStockpile() = default;
};

class IPackageQueue : public IPackageStockpile {
public:

//    Gives last (in LIFO) or first (in FIFO) element in queue_
    virtual Package pop() = 0;
//    Gives information about PackageQueue's type FIFO/LIFO
    [[nodiscard]] virtual PackageQueueType get_queue_type() const = 0;
};

class PackageQueue : public IPackageQueue {
public:

//    User-defined explicit constructor
    explicit PackageQueue(PackageQueueType queue_type) : queue_(), queue_type_(queue_type) {};

//    Implementation of virtual methods from IPackageQueue
    Package pop() override;
    [[nodiscard]] PackageQueueType get_queue_type() const override { return queue_type_; }

//    Implementation of virtual methods from IPackageStockpile
    [[nodiscard]] size_t size() const override { return queue_.size(); }
    void push(Package&& to_push) override { queue_.emplace_back(std::move(to_push)); }
    [[nodiscard]] bool empty() const override { return queue_.empty(); }

//    Implementation of IPackageStockpile 'Read-only' iterators
    [[nodiscard]] IPackageStockpile::const_iterator cbegin() const override { return queue_.cbegin(); }
    [[nodiscard]] IPackageStockpile::const_iterator cend() const override { return queue_.cend(); }
    [[nodiscard]] IPackageStockpile::const_iterator begin() const override { return queue_.begin(); }
    [[nodiscard]] IPackageStockpile::const_iterator end() const override { return queue_.end(); }

private:
    std::list<Package> queue_;
    PackageQueueType queue_type_;
};


#endif //IMPLEMENTATION_STORAGE_TYPES_HPP