#include "TSDL/abstract/elements/TextureElement.hpp"
#include "TSDL/abstract/elements/EventloopAdapter.hpp"
#include "TSDL/TSDL_Meta.hpp"

TSDL::elements::TextureElement::TextureElement(const TextureElement& other):
    Element(other.renderer()), sized<RenderSizedElement>(other.eventloop(), other.size()), _texture(other._texture) {}

TSDL::elements::TextureElement::TextureElement(EventloopAdapter& evloop, const ::TSDL::point_2d& size, std::shared_ptr<::TSDL::TSDL_Texture> texture):
    Element(evloop.renderer()), sized<RenderSizedElement>(evloop, size), _texture(texture) {}

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