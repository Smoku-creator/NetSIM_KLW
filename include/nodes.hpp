#ifndef IMPLEMENTATION_NODES_HPP
#define IMPLEMENTATION_NODES_HPP

#include "storage_types.hpp"
#include "helpers.hpp"
#include "config.hpp"

#include <memory>
#include <map>
#include <optional>
#include <utility>

enum class ReceiverType { WORKER, STOREHOUSE };

class IPackageReceiver
{
public:
    virtual void receive_package(Package &&p) = 0;
    [[nodiscard]] virtual ElementID get_id() const = 0;
    #if (EXERCISE_ID > EXERCISE_ID_NODES)
        [[nodiscard]] virtual ReceiverType get_receiver_type() const = 0;

        [[nodiscard]] virtual IPackageStockpile::const_iterator cbegin() const = 0;
        [[nodiscard]] virtual IPackageStockpile::const_iterator cend() const = 0;
        [[nodiscard]] virtual IPackageStockpile::const_iterator begin() const = 0;
        [[nodiscard]] virtual IPackageStockpile::const_iterator end() const = 0;
    #endif
};

class Storehouse : public IPackageReceiver
{
public:
    explicit Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d =
            std::make_unique<PackageQueue>(PackageQueueType::LIFO))
            : _id(id), d_(std::move(d)) {}

    [[nodiscard]] ElementID get_id() const override { return _id; }
    void receive_package(Package&& p) override { d_->push(std::move(p)); }
    #if (EXERCISE_ID > EXERCISE_ID_NODES)
        [[nodiscard]] ReceiverType get_receiver_type() const override { return _type; }

        [[nodiscard]] IPackageStockpile::const_iterator cbegin() const override { return d_->cbegin(); }
        [[nodiscard]] IPackageStockpile::const_iterator cend() const override { return d_->cend(); }
        [[nodiscard]] IPackageStockpile::const_iterator begin() const override { return d_->begin(); }
        [[nodiscard]] IPackageStockpile::const_iterator end() const override { return d_->end(); }
    #endif
private:
    ElementID _id;
    std::unique_ptr<IPackageStockpile> d_;
    const static ReceiverType _type = ReceiverType::STOREHOUSE;
};

class ReceiverPreferences
{
public:

    explicit ReceiverPreferences(ProbabilityGenerator pg = probability_generator) : pg_(std::move(pg)) {}


    using preferences_t = std::map<IPackageReceiver *, double>;
    using const_iterator = preferences_t::const_iterator;
    using iterator = preferences_t::iterator;

    [[nodiscard]] const_iterator cbegin() const { return map_.cbegin(); }
    [[nodiscard]] const_iterator cend() const { return map_.cend(); }
    [[nodiscard]] const_iterator begin() const { return map_.begin(); }
    [[nodiscard]] const_iterator end() const { return map_.end(); }

    void add_receiver(IPackageReceiver *r);
    void remove_receiver(IPackageReceiver *r);

    IPackageReceiver *choose_receiver();


    [[nodiscard]] const preferences_t &get_preferences() const { return map_; }
    size_t size() { return map_.size(); }


private:
    preferences_t map_;
    ProbabilityGenerator pg_;
};

class PackageSender : public ReceiverPreferences
{
public:
    PackageSender() = default;
    PackageSender(PackageSender &&) = default;

    void send_package();
    std::optional<Package> &get_sending_buffer();

    ReceiverPreferences receiver_preferences_;

protected:

    virtual void push_package(Package &&p) { buffer_.emplace(std::move(p)); }


private:
    std::optional<Package> buffer_;
};

class Worker : public IPackageReceiver, public PackageSender
{
public:

    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q)
    : id_(id), pd_(pd), q_(std::move(q)), t_(0) {}
    void do_work(Time t);
    [[nodiscard]] TimeOffset get_processing_duration() const { return pd_; }
    [[nodiscard]] Time get_package_processing_start_time() const { return t_; }

    [[nodiscard]] ElementID get_id() const override { return id_; }
    void receive_package(Package&& p) override { q_->push(std::move(p)); }

    #if (EXERCISE_ID > EXERCISE_ID_NODES)
        [[nodiscard]] ReceiverType get_receiver_type() const override { return _type; }

        [[nodiscard]] IPackageStockpile::const_iterator cbegin() const override { return q_->cbegin(); }
        [[nodiscard]] IPackageStockpile::const_iterator cend() const override { return q_->cend(); }
        [[nodiscard]] IPackageStockpile::const_iterator begin() const override { return q_->begin(); }
        [[nodiscard]] IPackageStockpile::const_iterator end() const override { return q_->end(); }
    #endif

private:
    ElementID id_;
    TimeOffset pd_;
    std::unique_ptr<IPackageQueue> q_;
    Time t_;
    const static ReceiverType _type = ReceiverType::WORKER;
};

class Ramp : public PackageSender
{
public:
    Ramp(ElementID id, TimeOffset di) : id_(id), di_(di) {}
    void deliver_goods(Time t) { if ((t - 1) % di_ == 0) push_package(Package()); }
    [[nodiscard]] TimeOffset get_delivery_interval() const {return di_; }
    [[nodiscard]] ElementID get_id() const { return id_; }

private:
    ElementID id_;
    TimeOffset di_;
};

#endif //IMPLEMENTATION_NODES_HPP
