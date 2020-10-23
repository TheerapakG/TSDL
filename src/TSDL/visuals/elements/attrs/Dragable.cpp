//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/visuals/elements/attrs/Dragable.hpp"

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