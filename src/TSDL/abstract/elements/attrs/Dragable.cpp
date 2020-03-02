#include "TSDL/abstract/elements/attrs/Dragable.hpp"

TSDL::elements::attrs::Dragable::Dragable(DragablePosTransformer pos_transform, ::TSDL::point_2d origin):
    _pos_transform_fun(pos_transform), _pos(origin) {}

::TSDL::point_2d TSDL::elements::attrs::Dragable::pos() const
{
    return _pos;
}

void TSDL::elements::attrs::Dragable::pos(const ::TSDL::point_2d& prev_pos, const ::TSDL::point_2d& dist)
{
    _pos = _pos_transform_fun(prev_pos, dist);
}