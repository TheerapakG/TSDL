#include "TSDL/TSDL_Renderer.hpp"
#include "TSDL/TSDL_Utility.hpp"
#include <string>

TSDL_DEFINE_CONSTRUCT(TSDL, Renderer)

TSDL::TSDL_Renderer::TSDL_Renderer(TSDL::TSDL_Window& window): TSDL_Renderer(window, 0) {}

TSDL::TSDL_Renderer::TSDL_Renderer(TSDL::TSDL_Window& window, Uint32 flags): _destroy(true)
{
    SDL_Renderer* _t_internal_ptr = SDL_CreateRenderer(window, -1, flags);
    if(_t_internal_ptr == NULL)
    {
        TSDL::safe_throw<std::runtime_error>("Renderer could not be created! SDL Error: " + std::string(SDL_GetError()));
        return;
    }
    _internal_ptr = _t_internal_ptr;
}

TSDL::TSDL_Renderer::~TSDL_Renderer()
{
    if(_destroy && (_internal_ptr != nullptr)) SDL_DestroyRenderer(_internal_ptr);
}

TSDL::TSDL_Renderer::operator SDL_Renderer*() const
{
    return _internal_ptr;
}

std::optional<TSDL::_SDL_Texture> TSDL::TSDL_Renderer::target()
{
    SDL_Texture* _t = SDL_GetRenderTarget(_internal_ptr);
    if(_t == NULL) return std::optional<TSDL::_SDL_Texture>();
    else return _t;
}

int TSDL::TSDL_Renderer::target(std::optional<std::reference_wrapper<TSDL::_SDL_Texture>> texture)
{
    int _t = SDL_SetRenderTarget(_internal_ptr, texture ? texture.value().get(): NULL);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not set render target! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

TSDL::point_2d TSDL::TSDL_Renderer::render_size()
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

TSDL::color_rgba TSDL::TSDL_Renderer::render_color()
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

int TSDL::TSDL_Renderer::render_color(const color_rgba& c)
{
    return this->render_color(c.r, c.g, c.b, c.a);
}

int TSDL::TSDL_Renderer::render_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int _t = SDL_SetRenderDrawColor(_internal_ptr, r, g, b, a);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not set renderer color! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::TSDL_Renderer::viewport(const rect& rect)
{
    int _t = SDL_RenderSetViewport(_internal_ptr, &rect);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not set renderer viewport! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::TSDL_Renderer::viewport(int x, int y, int w, int h)
{
    return this->viewport(TSDL::rect(x, y, w, h));
}

int TSDL::TSDL_Renderer::clear()
{
    int _t = SDL_RenderClear(_internal_ptr);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not clear render target! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::TSDL_Renderer::clear(const color_rgba& c)
{
    color_rgba prev_col = render_color();
    render_color(c); // TODO: check when noexcept signify error
    int _t = clear();
    if(_t != 0) return _t;
    render_color(prev_col); // TODO: check when noexcept signify error
	return _t;
}

int TSDL::TSDL_Renderer::copy_from(
    TSDL::_SDL_Texture texture, 
    std::optional<const std::reference_wrapper<rect>> srcrect, 
    std::optional<const std::reference_wrapper<rect>> dstrect
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

int TSDL::TSDL_Renderer::copy_from(
    TSDL::_SDL_Texture texture, 
    std::optional<const std::reference_wrapper<rect>> srcrect, 
    std::optional<const std::reference_wrapper<rect>> dstrect, 
    const double angle, 
    std::optional<const std::reference_wrapper<point_2d>> center, 
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

int TSDL::TSDL_Renderer::fill_rect(const rect& rect)
{
    int _t = SDL_RenderFillRect(_internal_ptr, &rect);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not fill rectangle on render target! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::TSDL_Renderer::fill_rect(int x, int y, int w, int h)
{
    return fill_rect(rect(x, y, w, h));
}

int TSDL::TSDL_Renderer::fill_rect(const ::TSDL::color_rgba& c, const ::TSDL::rect& rect)
{
    color_rgba prev_col = render_color();
    render_color(c); // TODO: check when noexcept signify error
    int _t = fill_rect(rect);
    if(_t != 0) return _t;
    render_color(prev_col); // TODO: check when noexcept signify error
	return _t;
}

int TSDL::TSDL_Renderer::fill_rect(const ::TSDL::color_rgba& c, int x, int y, int w, int h)
{
    return fill_rect(c, rect(x, y, w, h));
}

int TSDL::TSDL_Renderer::draw_rect(const rect& rect)
{
    int _t = SDL_RenderDrawRect(_internal_ptr, &rect);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not draw rectangle on render target! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::TSDL_Renderer::draw_rect(int x, int y, int w, int h)
{
    return this->draw_rect(rect(x, y, w, h));
}

int TSDL::TSDL_Renderer::draw_line(int x1, int y1, int x2, int y2)
{
    int _t = SDL_RenderDrawLine(_internal_ptr, x1, y1, x2, y2);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not draw line on render target! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::TSDL_Renderer::draw_point(int x, int y)
{
    int _t = SDL_RenderDrawPoint(_internal_ptr, x, y);
    if(_t != 0)
    {
        TSDL::safe_throw<std::runtime_error>("Could not draw point on render target! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::TSDL_Renderer::update()
{
    SDL_RenderPresent(_internal_ptr);
    return this->clear();
}

_TSDL_EXPAND_DECLARE_MASK_TYPE(TSDL, Renderer)