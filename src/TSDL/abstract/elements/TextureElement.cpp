#include "TSDL/abstract/elements/TextureElement.hpp"
#include "TSDL/abstract/elements/EventloopAdapter.hpp"
#include "TSDL/TSDL_Meta.hpp"

TSDL::elements::TextureElement::TextureElement(const TextureElement& other):
    attrs::sizable<RenderSizedElement>(other.eventloop(), other.renderer(), other.size()), _texture(other._texture) {}

TSDL::elements::TextureElement::TextureElement(EventloopAdapter& evloop, TSDL_Renderer& renderer, const ::TSDL::point_2d& size, std::shared_ptr<::TSDL::TSDL_Texture> texture):
    attrs::sizable<RenderSizedElement>(evloop, renderer, size), _texture(texture) {}

void TSDL::elements::TextureElement::render(const ::TSDL::point_2d& dist)
{
    render(dist, size());
}

void TSDL::elements::TextureElement::render(const ::TSDL::point_2d& dist, const ::TSDL::point_2d& size)
{
    renderer().copy_from(
        *_texture, 
        make_optional_const_ref<::TSDL::rect>(), 
        make_optional_const_ref<::TSDL::rect>({dist, dist+size})
    );

    not_update();
}