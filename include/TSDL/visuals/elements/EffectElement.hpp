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

#include "TSDL/visuals/elements/attrs/Sizable.hpp"
#include "TSDL/visuals/elements/EventloopAdapter.hpp"

#include "TSDL/Meta.hpp"

namespace TSDL
{
    namespace elements
    {
        class EffectElement
        {
            public:
            using TextureModifyFunc = std::function<void(Texture&)>;

            private:
            // TODO: texture modifier as effect object vector as documented
            TextureModifyFunc _texture_modifier;

            public:
            virtual TextureModifyFunc& modify_texture_function();
            virtual const TextureModifyFunc& modify_texture_function() const;
        };

        template <typename T, typename Enable = void>
        class effectelement;

        template <typename T>
        class effectelement<
            T,
            typename std::enable_if_t<
                _and_v<
                    std::is_base_of_v<attrs::Sized, T>,
                    std::is_base_of_v<DependentElement, T>
                >
            >
        >:
        public T, public EffectElement
        {
            private:
            mutable point_2d _current_texture_dim;
            mutable Texture* _src_texture = nullptr; 
            mutable Texture* _return_texture = nullptr;
            mutable bool _req_update = true;
            
            public:
            template <typename ...Args>
            effectelement(Args... args): T(args...) {}

            ~effectelement()
            {
                if(_src_texture!=nullptr) delete _src_texture;
                if(_return_texture!=nullptr) delete _return_texture;
            }

            virtual TextureModifyFunc& modify_texture_function() override
            {
                _req_update = true;
                return EffectElement::modify_texture_function();
            }

            virtual bool need_update() const override
            {
                return _req_update || T::need_update();
            }

            /*
            Re-render this element
            */
            virtual void render(WindowAdapter& window, const ::TSDL::point_2d& dist) const override
            {
                if(size()!=_current_texture_dim)
                {
                    if(_src_texture!=nullptr) delete _src_texture;
                    if(_return_texture!=nullptr) delete _return_texture;
                    _current_texture_dim = size();
                    _src_texture = new Texture(window.renderer(), _current_texture_dim);
                    _return_texture = new Texture(window.renderer(), _current_texture_dim);
                }

                auto _prev_target = window.renderer().target();

                if(T::need_update())
                {
                    window.renderer().target(make_optional_ref(*_src_texture));
                    T::render(window, {0, 0});
                }

                const TextureModifyFunc& func = static_cast<const EffectElement*>(this)->modify_texture_function();

                if(func)
                {
                    window.renderer().target(make_optional_ref(*_return_texture));
                    window.renderer().clear({0, 0, 0, 0});
                    window.renderer().copy_from(
                        *_src_texture, 
                        make_optional_ref(rect{{0, 0}, _current_texture_dim}), 
                        make_optional_ref(rect{{0, 0}, _current_texture_dim})
                    );

                    func(*_return_texture);

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
                    _req_update = false;
                }
                
            }
        };
    }
}

#endif