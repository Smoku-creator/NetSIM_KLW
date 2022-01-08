#ifndef IMPLEMENTATION_NODES_HPP
#define IMPLEMENTATION_NODES_HPP

#include "storage_types.hpp"
#include "helpers.hpp"

#include <memory>
#include <map>
#include <optional>
#include <utility>

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
    explicit Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> package_depot = std::make_unique<PackageQueue>(
            PackageQueueType::LIFO)) : _id(id), package_stockpile_(std::move(package_depot)) {}
    [[nodiscard]] ElementID get_id() const override { return _id; }
    [[nodiscard]] ReceiverType get_node_type() const override { return _type; }
    void receive_package(Package&& p) override { package_stockpile_->push(std::move(p)); }

    IPackageStockpile::const_iterator cbegin() override { return package_stockpile_->cbegin(); }
    IPackageStockpile::const_iterator cend() override { return package_stockpile_->cend(); }
    IPackageStockpile::iterator begin() override { return package_stockpile_->begin(); }
    IPackageStockpile::iterator end() override { return package_stockpile_->end(); }

private:
    ElementID _id;
    std::unique_ptr<IPackageStockpile> package_stockpile_;
    const static ReceiverType _type = ReceiverType::STOREHOUSE;
};

class ReceiverPreferences
{
public:
    explicit ReceiverPreferences(ProbabilityGenerator pg = probability_generator) : pg_(std::move(pg)) {}

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

    preferences_t &get_preferences() { return map_; };

private:
    preferences_t map_;
    ProbabilityGenerator pg_;
};

//class PackageSender : public ReceiverPreferences
//{
//public:
//    PackageSender(PackageSender &&) = default;
//
//    void send_package();
//    std::optional<Package> &get_sending_buffer() const;
//
//    ReceiverPreferences receiver_preferences_;
//
//protected:
//    void push_package(Package &&) { buffer_.emplace(this); };
//
//private:
//    std::optional<Package> buffer_;
//};

//class Worker : public IPackageReceiver, public PackageSender
//{
//public:
//    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q);
//    void do_work(Time t);
//    TimeOffset get_processing_duration() const;
//    Time get_package_processing_start_time() const;
//
//private:
//    ElementID id_;
//    TimeOffset pd_;
//    std::unique_ptr<IPackageQueue> q_;
//};
//
//class Ramp : public PackageSender
//{
//public:
//    Ramp(ElementID id, TimeOffset di);
//
//    void deliver_goods(time t);
//    TimeOffset get_delivery_interval() const;
//    ElementID get_id() const;
//
//private:
//    ElementID id_;
//    TimeOffset pd_;
//};

#endif //IMPLEMENTATION_NODES_HPP
