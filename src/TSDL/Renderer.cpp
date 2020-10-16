//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/Renderer.hpp"
#include "TSDL/Utility.hpp"
#include <string>

TSDL_DEFINE_CONSTRUCT(TSDL, Renderer)

TSDL::Renderer::Renderer(TSDL::Window& window): Renderer(window, 0) {}

TSDL::Renderer::Renderer(TSDL::Window& window, Uint32 flags): _destroy(true)
{
    SDL_Renderer* _t_internal_ptr = SDL_CreateRenderer(window, -1, flags);
    if(_t_internal_ptr == NULL)
    {
        TSDL::safe_throw<std::runtime_error>("Renderer could not be created! SDL Error: " + std::string(SDL_GetError()));
        return;
    }
    _internal_ptr = _t_internal_ptr;
}

TSDL::Renderer::~Renderer()
{
    if(_destroy && (_internal_ptr != nullptr)) SDL_DestroyRenderer(_internal_ptr);
}

TSDL::Renderer::operator SDL_Renderer*() const
{
    return _internal_ptr;
}

std::optional<TSDL::Texture> TSDL::Renderer::target()
{
    SDL_Texture* _t = SDL_GetRenderTarget(_internal_ptr);
    if(_t == NULL) return std::optional<TSDL::Texture>();
    else return Texture(_t);
}

int TSDL::Renderer::target(std::optional<std::reference_wrapper<TSDL::Texture>> texture)
{
    int _t = SDL_SetRenderTarget(_internal_ptr, texture ? static_cast<SDL_Texture*>(texture.value().get()): NULL);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not set render target! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

TSDL::point_2d TSDL::Renderer::render_size()
{
    int x, y;
    int _t = SDL_GetRendererOutputSize(_internal_ptr, &x, &y);
    if(_t != 0)
    {
        //TODO: noexcept signify error
        TSDL::safe_throw<std::runtime_error>("Could not get renderer size! SDL Error: " + std::string(SDL_GetError()));
    }
    return TSDL::point_2d(x, y);
}

TSDL::color_rgba TSDL::Renderer::render_color()
{
    Uint8 r, g, b, a;
    int _t = SDL_GetRenderDrawColor(_internal_ptr, &r, &g, &b, &a);
    if(_t != 0)
    {
        //TODO: noexcept signify error
        TSDL::safe_throw<std::runtime_error>("Could not set renderer color! SDL Error: " + std::string(SDL_GetError()));
    }
    return TSDL::color_rgba(r, g, b, a);
}

int TSDL::Renderer::render_color(const color_rgba& c)
{
    return this->render_color(c.r, c.g, c.b, c.a);
}

int TSDL::Renderer::render_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int _t = SDL_SetRenderDrawColor(_internal_ptr, r, g, b, a);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not set renderer color! SDL Error: " + std::string(SDL_GetError()));
    }
    _t = SDL_SetRenderDrawBlendMode(_internal_ptr, a==0xFF?SDL_BLENDMODE_NONE:SDL_BLENDMODE_BLEND);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not set renderer blend mode! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::Renderer::viewport(const rect& rect)
{
    int _t = SDL_RenderSetViewport(_internal_ptr, &rect);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not set renderer viewport! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::Renderer::viewport(int x, int y, int w, int h)
{
    return this->viewport(TSDL::rect(x, y, w, h));
}

int TSDL::Renderer::clear()
{
    int _t = SDL_RenderClear(_internal_ptr);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not clear render target! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::Renderer::clear(const color_rgba& c)
{
    color_rgba prev_col = render_color();
    render_color(c); // TODO: check when noexcept signify error
    int _t = clear();
    if(_t != 0) return _t;
    render_color(prev_col); // TODO: check when noexcept signify error
	return _t;
}

int TSDL::Renderer::copy_from(
    TSDL::Texture& texture, 
    optional_const_reference<rect> srcrect, 
    optional_const_reference<rect> dstrect
)
{
    int _t = SDL_RenderCopy(
        _internal_ptr, 
        texture, 
        srcrect ? &srcrect.value().get(): NULL, 
        dstrect ? &dstrect.value().get(): NULL
    );
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not copy to render target! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::Renderer::copy_from(
    TSDL::Texture& texture, 
    optional_const_reference<rect> srcrect,
    optional_const_reference<rect> dstrect,
    const double angle, 
    optional_const_reference<point_2d> center,
    const SDL_RendererFlip flip
)
{
    int _t = SDL_RenderCopyEx(
        _internal_ptr, 
        texture, 
        srcrect ? &srcrect.value().get(): NULL, 
        dstrect ? &dstrect.value().get(): NULL, 
        angle, 
        center ? &center.value().get(): NULL, 
        flip
    );
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not copy/rotate to render target! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::Renderer::fill_rect(const rect& rect)
{
    int _t = SDL_RenderFillRect(_internal_ptr, &rect);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not fill rectangle on render target! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::Renderer::fill_rect(int x, int y, int w, int h)
{
    return fill_rect(rect(x, y, w, h));
}

int TSDL::Renderer::fill_rect(const ::TSDL::color_rgba& c, const ::TSDL::rect& rect)
{
    color_rgba prev_col = render_color();
    render_color(c); // TODO: check when noexcept signify error
    int _t = fill_rect(rect);
    if(_t != 0) return _t;
    render_color(prev_col); // TODO: check when noexcept signify error
	return _t;
}

int TSDL::Renderer::fill_rect(const ::TSDL::color_rgba& c, int x, int y, int w, int h)
{
    return fill_rect(c, rect(x, y, w, h));
}

int TSDL::Renderer::draw_rect(const rect& rect)
{
    int _t = SDL_RenderDrawRect(_internal_ptr, &rect);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not draw rectangle on render target! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::Renderer::draw_rect(int x, int y, int w, int h)
{
    return this->draw_rect(rect(x, y, w, h));
}

int TSDL::Renderer::draw_line(int x1, int y1, int x2, int y2)
{
    int _t = SDL_RenderDrawLine(_internal_ptr, x1, y1, x2, y2);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not draw line on render target! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::Renderer::draw_point(int x, int y)
{
    int _t = SDL_RenderDrawPoint(_internal_ptr, x, y);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not draw point on render target! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::Renderer::update()
{
    SDL_RenderPresent(_internal_ptr);
    return clear();
}

int TSDL::Renderer::update(const color_rgba& c)
{
    SDL_RenderPresent(_internal_ptr);
    return clear(c);
}