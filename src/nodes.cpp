<<<<<<< HEAD
//#include "nodes.hpp"
//
//ReceiverPreferences::ReceiverPreferences(ProbabilityGenerator pg){
//
//}
//
//void ReceiverPreferences::add_receiver(IPackageReceiver *r){
//
//}
//
//void ReceiverPreferences::remove_receiver(IPackageReceiver *r){
//
//}
//
//IPackageReceiver* ReceiverPreferences::choose_receiver(){
//
//}
//
//preferences_t& const ReceiverPreferences::get_preferences(){
//
//}
=======
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
    double sum = 0;
    for (auto it = map_.begin(); it != map_.end(); ++it)
    {
        sum += it->second;
        if (sum >= pg_)
        {
            return it->first;
        }
    }
}

void PackageSender::send_package()
{
    if (buffer_)
        return *buffer_;
}
>>>>>>> 77779e5480b54a5088eda0472e113884bd06fdf9
