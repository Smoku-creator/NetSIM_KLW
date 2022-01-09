#include "nodes.hpp"

void ReceiverPreferences::add_receiver(IPackageReceiver *r)
{
    if (map_.empty())
    {
        map_.insert({r, 1.0});
    }
    else
    {
        int n = map_.size();
        for (auto it = map_.begin(); it != map_.end(); ++it)
        {
            it->second = 1 / (n + 1);
        }
        map_.insert({r, 1 / (n + 1)});
    }
}

void ReceiverPreferences::remove_receiver(IPackageReceiver *r)
{
    int n = map_.size();
    auto it = map_.find(r);
    map_.erase(it);
    if (n > 1)
    {
        for (auto it = map_.begin(); it != map_.end(); ++it)
        {
            it->second = 1 / (n - 1);
        }
    }
}

IPackageReceiver *ReceiverPreferences::choose_receiver()
{
    double value = pg_.operator()();
    double sum = 0;
    for (auto it = map_.begin(); it != map_.end(); ++it)
    {
        sum += it->second;
        if (sum >= value)
        {
            return it->first;
        }
    }
    return nullptr;
}

std::optional<Package> &PackageSender::get_sending_buffer()
{
    if (buffer_)
        return buffer_;
}

void PackageSender::send_package()
{
    if (buffer_)
    {
        receiver_preferences_.choose_receiver()->receive_package((Package &&) std::move(buffer_));
    }
}

void Worker::do_work(Time t) {
    if (!t_) {
        t_ = t;
        push_package(q_->pop());
    }
    else {
        if (t_ + pd_ == t) {
            t_ = 0;
        }
    }
}