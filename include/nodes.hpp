#ifndef IMPLEMENTATION_NODES_HPP
#define IMPLEMENTATION_NODES_HPP

#include "storage_types.hpp"
#include "helpers.hpp"

#include <memory>
#include <map>
#include <optional>

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
    ReceiverPreferences(ProbabilityGenerator pg = probability_generator) : pg_(std::move(pg)) {}

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
    preferences_t &get_preferences() { return map_; }

private:
    preferences_t map_;
    ProbabilityGenerator pg_;
};

class PackageSender : public ReceiverPreferences
{
public:
    PackageSender(PackageSender &&) = default;

    void send_package();
    std::optional<Package> &get_sending_buffer();

    ReceiverPreferences receiver_preferences_;

protected:
    void push_package(Package &&p) { buffer_.emplace(std::move(p)); }

private:
    std::optional<Package> buffer_;
};

class Worker : public IPackageReceiver, public PackageSender, public IPackageQueue
{
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q) : id_(id), pd_(pd), q_(q) {}
    void do_work(Time t);
    TimeOffset get_processing_duration() const { return pd_; };
    Time get_package_processing_start_time() const { return t_; };

private:
    ElementID id_;
    TimeOffset pd_;
    std::unique_ptr<IPackageQueue> q_;
    Time t_ = 0;
    PackageQueue queue_;
};

class Ramp : public PackageSender
{
public:
    Ramp(ElementID id, TimeOffset di) : id_(id), di_(di) {}

    void deliver_goods(Time t)
    {
        if ((t - 1) % di_)
        {
            push_package(Package());
        }
    }
    TimeOffset get_delivery_interval() const { return di_; };
    ElementID get_id() const { return id_; };

private:
    ElementID id_;
    TimeOffset di_;
};

#endif //IMPLEMENTATION_NODES_HPP
