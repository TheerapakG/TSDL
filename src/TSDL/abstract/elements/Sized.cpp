#include "TSDL/abstract/elements/Sized.hpp"

TSDL::elements::Sized::Sized(const point_2d& size): _size(size) {}

void TSDL::elements::Sized::size(const point_2d& size)
{
    _size = size;
}
const TSDL::point_2d& TSDL::elements::Sized::size()
{
    return _size;
}