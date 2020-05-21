//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_ELEMENTS_EFFECTELEMENT_
#define TSDL_ELEMENTS_EFFECTELEMENT_

#include "TSDL/abstract/elements/attrs/Sizable.hpp"
#include "TSDL/abstract/elements/EventloopAdapter.hpp"

#include "TSDL/TSDL_Meta.hpp"

namespace TSDL
{
    namespace elements
    {
        class EffectElement: public attrs::sized<DependentElement>
        {
            using TextureModifyFunc = std::function<void(TSDL_Texture&)>;

            private:
            attrs::Sized& _src_size;
            DependentElement& _src_element;
            point_2d _current_texture_dim;
            // TODO: texture modifier as effect object vector as documented
            TextureModifyFunc _texture_modifier;
            TSDL_Texture* _src_texture = nullptr; 
            TSDL_Texture* _return_texture = nullptr; 

            public:
            template<
                typename T, 
                typename U = std::enable_if_t<
                    _and_v<
                        std::is_base_of_v<attrs::Sized, T>,
                        std::is_base_of_v<DependentElement, T>
                    >
                >
            >
            EffectElement(T& src): _src_size(src), _src_element(src){}

            virtual point_2d size() const override;

            void modify_texture_function(const TextureModifyFunc& function);
            void modify_texture_function();

            /*
            Re-render this element
            */
            virtual void render(WindowAdapter& window, const ::TSDL::point_2d& dist) override;
        };
    }
}

#endif