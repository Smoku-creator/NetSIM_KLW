#ifndef IMPLEMENTATION_NODES_HPP
#define IMPLEMENTATION_NODES_HPP

#include "storage_types.hpp"

#include <memory>


enum class ReceiverType {
    WORKER, STOREHOUSE
};

class IPackageReceiver {
public:
    virtual void receive_package(Package&& p) = 0;
    [[nodiscard]] virtual ElementID get_id() const = 0;
    [[nodiscard]] virtual ReceiverType get_node_type() const = 0;

    virtual IPackageStockpile::const_iterator cbegin() = 0;
    virtual IPackageStockpile::const_iterator cend() = 0;
    virtual IPackageStockpile::iterator begin() = 0;
    virtual IPackageStockpile::iterator end() = 0;
};

class Storehouse : public IPackageReceiver {
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

#endif //IMPLEMENTATION_NODES_HPP
