#include "storage_types.hpp"
#include "package.hpp"


// Initialization of pop() method
Package PackageQueue::pop() {
//    Declaring a temporary Package
    Package popped;
//    Conditional instructions in a whole PackageQueueType spectrum
    switch (queue_type_) {
        case PackageQueueType::FIFO: {
//            First out, so take the front element, using operator=
            popped = std::move(queue_.front());
//            Don't forget to remove the redundant one
            queue_.pop_front();
            break;
        }
        case PackageQueueType::LIFO: {
//            Last out, so take the back element, using operator=
            popped = std::move(queue_.back());
//            Don't forget to remove the redundant one
            queue_.pop_back();
            break;
        }
    }
//    Return conditioned Package
    return popped;
}
