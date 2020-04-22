#include "fixed.hpp"

std::ostream &operator<<(std::ostream &os, Fixed const &value)
{
    os << value.double_value();
    return os;
}
