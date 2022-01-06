#ifndef IMPLEMENTATION_NODES_HPP
#define IMPLEMENTATION_NODES_HPP

#include "storage_types.hpp"
#include "helpers.hpp"

#include <memory>
#include <map>

enum class ReceiverType
{
    WORKER,
    STOREHOUSE
};

class IPackageReceiver
{
public:
    virtual void receive_package(Package &&p) = 0;
    [[nodiscard]] virtual ElementID get_id() const = 0;
    [[nodiscard]] virtual ReceiverType get_node_type() const = 0;

    virtual IPackageStockpile::const_iterator cbegin() = 0;
    virtual IPackageStockpile::const_iterator cend() = 0;
    virtual IPackageStockpile::iterator begin() = 0;
    virtual IPackageStockpile::iterator end() = 0;
};

class Storehouse : public IPackageReceiver
{
public:
    explicit Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> = nullptr) { id_ = id; }
    [[nodiscard]] ElementID get_id() const override { return id_; }
    [[nodiscard]] ReceiverType get_node_type() const override { return type_; }

    IPackageStockpile::const_iterator cbegin() override { return storage_.cbegin(); }
    IPackageStockpile::const_iterator cend() override { return storage_.cend(); }
    IPackageStockpile::iterator begin() override { return storage_.begin(); }
    IPackageStockpile::iterator end() override { return storage_.end(); }

private:
    ElementID id_;
    std::list<Package> storage_;
    const static ReceiverType type_ = ReceiverType::STOREHOUSE;
};

class ReceiverPreferences
{
public:
    ReceiverPreferences(ProbabilityGenerator pg);
    using preferences_t = std::map<IPackageReceiver *, double>;
    using const_iterator = preferences_t::const_iterator;
    using iterator = preferences_t::iterator;
    const_iterator cbegin() { return map_.cbegin(); }
    const_iterator cend() { return map_.cend(); }
    iterator begin() { return map_.begin(); }
    iterator end() { return map_.end(); }
    void add_receiver(IPackageReceiver *r);
    void remove_receiver(IPackageReceiver *r);
    IPackageReceiver *choose_receiver();
    preferences_t &const get_preferences();

private:
    preferences_t map_;
};

class PackageSender : public ReceiverPreferences
{
public:
private:
};

class Worker : public IPackageReceiver, public PackageSender
{
public:
private:
};

class Ramp : public PackageSender
{
public:
private:
};

#endif //IMPLEMENTATION_NODES_HPP
