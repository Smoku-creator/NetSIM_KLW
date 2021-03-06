#include "nodes.hpp"

void ReceiverPreferences::add_receiver(IPackageReceiver *r)
{
    if (map_.empty())
    {
        map_.insert({r, 1.0});
    }
    else
    {
        auto n_1 = static_cast<double>(map_.size());
        map_.insert({r, 1.0 / (n_1 + 1.0)});
        auto n_2 = static_cast<double>(map_.size());
        if (n_2 > n_1) {
            for (auto& it: map_) {
                it.second = 1.0 / n_2;
            }
        }
    }
}

void ReceiverPreferences::remove_receiver(IPackageReceiver *r)
{
    auto n = static_cast<double>(map_.size());
    auto it_find = map_.find(r);
    map_.erase(it_find);
    if (n > 1)
    {
        for (auto & it : map_)
        {
            it.second = 1 / (n - 1);
        }
    }
}

IPackageReceiver *ReceiverPreferences::choose_receiver()
{
    double value = pg_.operator()();
    double sum = 0;
    for (auto & it : map_)
    {
        sum += it.second;
        if (sum >= value)
        {
            return it.first;
        }
    }
    return nullptr;
}

void PackageSender::send_package() {
    if (buffer_)
    {
        receiver_preferences_.choose_receiver()->receive_package(std::move(buffer_.value()));
        buffer_.reset();
    }
}

void Worker::do_work(Time t) {
    if (!t_ && !pbuffer_) {
        t_ = t;
        if (pd_ == 1) {
            push_package(q_->pop());
        }
        else {
            pbuffer_.emplace(q_->pop());
        }
    }
    else if (t_) {
        if (t_ + pd_ - 1 == t) {
            push_package(std::move(pbuffer_.value()));
            pbuffer_.reset();
            t_ = 0;
        }
    }
}
