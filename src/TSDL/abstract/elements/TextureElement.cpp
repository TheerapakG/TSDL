#include "TSDL/abstract/elements/TextureElement.hpp"
#include "TSDL/TSDL_Meta.hpp"

TSDL::elements::TextureElement::TextureElement(const TextureElement& other):
    Element(other.renderer()), sized<RenderSizedElement>(other.renderer(), other.size()), _texture(other._texture) {}

TSDL::elements::TextureElement::TextureElement(::TSDL::TSDL_Renderer& renderer, const ::TSDL::point_2d& size, std::shared_ptr<::TSDL::TSDL_Texture> texture):
    Element(renderer), sized<RenderSizedElement>(renderer, size), _texture(texture) {}

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