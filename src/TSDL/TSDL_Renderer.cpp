#include "TSDL/TSDL_Renderer.hpp"
#include <stdexcept>
#include <string>

TSDL::TSDL_Renderer::TSDL_Renderer(TSDL::TSDL_Window window): TSDL_Renderer(window, 0) {}

TSDL::TSDL_Renderer::TSDL_Renderer(TSDL::TSDL_Window window, Uint32 flags)
{
    SDL_Renderer* _t_internal_ptr = SDL_CreateRenderer(window, -1, flags);
    if(_t_internal_ptr == NULL)
    {
        throw std::runtime_error("Renderer could not be created! SDL Error: " + std::string(SDL_GetError()));
    }
    _internal_ptr = _t_internal_ptr;
    try
    {
        this->set_render_color(0xFF, 0xFF, 0xFF, 0xFF);
    }
    catch(...)
    {
        SDL_DestroyRenderer(_internal_ptr);
        throw;
    }
}

TSDL::TSDL_Renderer::~TSDL_Renderer()
{
    SDL_DestroyRenderer(_internal_ptr);
}

TSDL::TSDL_Renderer::operator SDL_Renderer*() const
{
    return _internal_ptr;
}

int TSDL::TSDL_Renderer::set_render_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int _t = SDL_SetRenderDrawColor(_internal_ptr, r, g, b, a);
    if(_t != 0)
    {
        throw std::runtime_error("Could not set renderer color! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::TSDL_Renderer::set_viewport(const SDL_Rect* rect)
{
    int _t = SDL_RenderSetViewport(_internal_ptr, rect);
    if(_t != 0)
    {
        throw std::runtime_error("Could not set renderer viewport! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::TSDL_Renderer::set_viewport(int x, int y, int w, int h)
{
    SDL_Rect rect = {x, y, w, h};
    return this->set_viewport(&rect);
}

int TSDL::TSDL_Renderer::clear()
{
    int _t = SDL_RenderClear(_internal_ptr);
    if(_t != 0)
    {
        throw std::runtime_error("Could not clear render target! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::TSDL_Renderer::copy_from(TSDL::TSDL_Texture texture, const SDL_Rect *srcrect, const SDL_Rect *dstrect)
{
    int _t = SDL_RenderCopy(_internal_ptr, texture, srcrect, dstrect);
    if(_t != 0)
    {
        throw std::runtime_error("Could not copy to render target! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::TSDL_Renderer::copy_from(TSDL::TSDL_Texture texture, const SDL_Rect *srcrect, const SDL_Rect *dstrect, const double angle, const SDL_Point* center, const SDL_RendererFlip flip)
{
    int _t = SDL_RenderCopyEx(_internal_ptr, texture, srcrect, dstrect, angle, center, flip);
    if(_t != 0)
    {
        throw std::runtime_error("Could not copy/rotate to render target! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::TSDL_Renderer::fill_rect(const SDL_Rect* rect)
{
    int _t = SDL_RenderFillRect(_internal_ptr, rect);
    if(_t != 0)
    {
        throw std::runtime_error("Could not fill rectangle on render target! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::TSDL_Renderer::fill_rect(int x, int y, int w, int h)
{
    SDL_Rect rect = {x, y, w, h};
    return this->fill_rect(&rect);
}

int TSDL::TSDL_Renderer::draw_rect(const SDL_Rect* rect)
{
    int _t = SDL_RenderDrawRect(_internal_ptr, rect);
    if(_t != 0)
    {
        throw std::runtime_error("Could not draw rectangle on render target! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::TSDL_Renderer::draw_rect(int x, int y, int w, int h)
{
    SDL_Rect rect = {x, y, w, h};
    return this->draw_rect(&rect);
}

int TSDL::TSDL_Renderer::draw_line(int x1, int y1, int x2, int y2)
{
    int _t = SDL_RenderDrawLine(_internal_ptr, x1, y1, x2, y2);
    if(_t != 0)
    {
        throw std::runtime_error("Could not draw line on render target! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::TSDL_Renderer::draw_point(int x, int y)
{
    int _t = SDL_RenderDrawPoint(_internal_ptr, x, y);
    if(_t != 0)
    {
        throw std::runtime_error("Could not draw point on render target! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::TSDL_Renderer::update()
{
    SDL_RenderPresent(_internal_ptr);
    return this->clear();
}

_TSDL_EXPAND_DECLARE_MASK_TYPE(TSDL, Renderer)