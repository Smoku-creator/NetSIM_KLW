#include "nodes.hpp"

void ReceiverPreferences::add_receiver(IPackageReceiver *r)
{
    if (map_.empty())
    {
        map_.insert({r, 1.0});
    }
    else
    {
        auto n = static_cast<double>(map_.size());
        for (auto & it : map_)
        {
            it.second = 1.0 / (n + 1.0);
        }
        map_.insert({r, 1.0 / (n + 1.0)});
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
//    auto sum = 0;
//    for (auto it = map_.begin(); it != map_.end(); ++it)
//    {
//        sum += it->second;
//        if (sum >= pg_)
//        {
//            return it->first;
//        }
//    }
return reinterpret_cast<IPackageReceiver*>(this);
}

//void PackageSender::send_package()
//{
//    if (buffer_)
//        return *buffer_;
//}
