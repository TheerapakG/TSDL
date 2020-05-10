//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/abstract/elements/attrs/Sizable.hpp"

TSDL::elements::attrs::Sizable::Sizable(const point_2d& size): _size(size) {}

void TSDL::elements::attrs::Sizable::size(const point_2d& size)
{
    _size = size;
}
TSDL::point_2d TSDL::elements::attrs::Sizable::size() const
{
    return _size;
}