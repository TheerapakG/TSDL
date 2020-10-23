//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/visuals/elements/TextureElement.hpp"
#include "TSDL/visuals/elements/WindowAdapter.hpp"
#include "TSDL/Meta.hpp"

TSDL::elements::TextureElement::TextureElement(const TextureElement& other):
    attrs::sizable<RenderSizedElement>(other.size()), _texture(other._texture) {}

TSDL::elements::TextureElement::TextureElement(const ::TSDL::point_2d& size, std::shared_ptr<::TSDL::Texture> texture):
    attrs::sizable<RenderSizedElement>(size), _texture(texture) {}

TSDL::Texture& TSDL::elements::TextureElement::texture()
{
    return *_texture;
}

void TSDL::elements::TextureElement::render(WindowAdapter& window, const ::TSDL::point_2d& dist)
{
    render(window, dist, size());
}

void TSDL::elements::TextureElement::render(WindowAdapter& window, const ::TSDL::point_2d& dist, const ::TSDL::point_2d& size)
{
    window.renderer().copy_from(
        *_texture, 
        make_optional_const_ref<::TSDL::rect>(), 
        make_optional_const_ref<::TSDL::rect>({dist, dist+size})
    );

    not_update();
}