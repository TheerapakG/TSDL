//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/abstract/elements/EffectElement.hpp"
#include "TSDL/abstract/elements/WindowAdapter.hpp"
#include "TSDL/TSDL_Meta.hpp"

namespace TSDL::elements
{
    point_2d EffectElement::size() const
    {
        return _src_size.size();
    }

    void EffectElement::modify_texture_function(const TextureModifyFunc& function)
    {
        _texture_modifier = function;
    }
    
    void EffectElement::modify_texture_function()
    {
        _texture_modifier = nullptr;
    }

    void EffectElement::render(WindowAdapter& window, const ::TSDL::point_2d& dist)
    {
        if(_src_size.size()!=_current_texture_dim)
        {
            if(_src_texture!=nullptr) delete _src_texture;
            if(_return_texture!=nullptr) delete _return_texture;
            _current_texture_dim = _src_size.size();
            _src_texture = new TSDL_Texture(window.renderer(), _current_texture_dim);
            _return_texture = new TSDL_Texture(window.renderer(), _current_texture_dim);
        }

        auto _prev_target = window.renderer().target();

        if(_src_element.need_update())
        {
            window.renderer().target(make_optional_ref(*_src_texture));

            _src_element.render(window, {0, 0});
        }

        if(_texture_modifier)
        {
            window.renderer().target(make_optional_ref(*_return_texture));
            window.renderer().clear({0, 0, 0, 0});
            window.renderer().copy_from(
                *_src_texture, 
                make_optional_ref(rect{{0, 0}, _current_texture_dim}), 
                make_optional_ref(rect{{0, 0}, _current_texture_dim})
            );

            _texture_modifier(*_return_texture);

            window.renderer().target(_prev_target?make_optional_ref(_prev_target.value()):std::nullopt);
            window.renderer().copy_from(
                *_return_texture, 
                make_optional_ref(rect{{0, 0}, _current_texture_dim}), 
                make_optional_ref(rect{dist, dist+_current_texture_dim})
            );
        }
        else
        {
            window.renderer().target(_prev_target?make_optional_ref(_prev_target.value()):std::nullopt);
            window.renderer().copy_from(
                *_src_texture, 
                make_optional_ref(rect{{0, 0}, _current_texture_dim}), 
                make_optional_ref(rect{dist, dist+_current_texture_dim})
            );
        }
        
    }
}