#include "TSDL/abstract/elements/attrs/Sizable.hpp"

TSDL::elements::attrs::Sizable::Sizable(const point_2d& size): _size(size) {}

void TSDL::elements::attrs::Sizable::size(const point_2d& size)
{
    _size = size;
}
const TSDL::point_2d& TSDL::elements::attrs::Sizable::size() const
{
    return _size;
}