#ifndef IMPLEMENTATION_PACKAGE_HPP
#define IMPLEMENTATION_PACKAGE_HPP

#include "types.hpp"

#include <set>
#include <optional>

class Package
{
public:
    //    Main Constructors' declaration
    Package();
    explicit Package(ElementID m);

<<<<<<< HEAD
//    Move Constructor by default
    Package(Package&& to_move) noexcept ;
=======
    Package(Package &to_copy) = delete;
    Package operator=(Package &beta) = delete;

    //    Move Constructor by default
    Package(Package &&to_move) noexcept;
>>>>>>> 77779e5480b54a5088eda0472e113884bd06fdf9

    //    Operator='s definition by default
    Package &operator=(Package &&alfa) noexcept;

    //    Method returning package's ID
    [[nodiscard]] ElementID get_id() const { return semi_prod_; }

    //    Destructor's declaration
    ~Package();

private:
    //    Static Members
    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;
    const static ElementID err_ = -1;

    //    ID of the Package
    ElementID semi_prod_;
};

#endif //IMPLEMENTATION_PACKAGE_HPP