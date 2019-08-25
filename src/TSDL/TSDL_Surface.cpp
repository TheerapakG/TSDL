#include "TSDL/TSDL_Surface.hpp"
#include <stdexcept>

TSDL::TSDL_Surface::TSDL_Surface(SDL_Surface* ptr): TSDL_Surface(ptr, false) {}

TSDL::TSDL_Surface::TSDL_Surface(SDL_Surface* ptr, bool handle_destroy): _internal_ptr(ptr), _destroy(handle_destroy) {}

TSDL::TSDL_Surface::TSDL_Surface(const std::string& file): _destroy(true)
{
    SDL_Surface* _t_internal_ptr = IMG_Load(file.c_str());
    if(_t_internal_ptr == NULL)
    {
        throw std::runtime_error("Surface could not be created! SDL_Error: " + std::string(SDL_GetError()));
    }
    _internal_ptr = _t_internal_ptr;
}

TSDL::TSDL_Surface::TSDL_Surface(const std::string& text, TTF_Font* font, Uint8 r, Uint8 g, Uint8 b, TSDL::TTF_Rendermethod m):
    TSDL::TSDL_Surface(text, font, r, g, b, 255, m){}

TSDL::TSDL_Surface::TSDL_Surface(const std::string& text, TTF_Font* font, Uint8 r, Uint8 g, Uint8 b, Uint8 a, TSDL::TTF_Rendermethod m): _destroy(true)
{
    SDL_Surface* _t_internal_ptr = nullptr;
    switch (m)
    {
    case TSDL::TTF_Rendermethod::Solid:
        _t_internal_ptr = TTF_RenderUTF8_Solid(font, text.c_str(), {r, g, b, a});
        break;

    case TSDL::TTF_Rendermethod::Shaded:
        // assume white bg with 0 as alpha
        _t_internal_ptr = TTF_RenderUTF8_Shaded(font, text.c_str(), {r, g, b, a}, {255, 255, 255, 0});
        break;

    case TSDL::TTF_Rendermethod::Blended:
        _t_internal_ptr = TTF_RenderUTF8_Blended(font, text.c_str(), {r, g, b, a});
        break;
    
    default:
        throw std::runtime_error("Invalid Rendermethod specified!");
        break;
    }
    
    if(_t_internal_ptr == NULL)
    {
        throw std::runtime_error("Surface could not be created! SDL_Error: " + std::string(TTF_GetError()));
    }
    _internal_ptr = _t_internal_ptr;
}

TSDL::TSDL_Surface::TSDL_Surface(const std::string& text, TTF_Font* font, Uint8 fr, Uint8 fg, Uint8 fb, Uint8 br, Uint8 bg, Uint8 bb):
    TSDL::TSDL_Surface(text, font, fr, fg, fb, 255, br, bg, bb, 0){}

TSDL::TSDL_Surface::TSDL_Surface(const std::string& text, TTF_Font* font, Uint8 fr, Uint8 fg, Uint8 fb, Uint8 fa, Uint8 br, Uint8 bg, Uint8 bb, Uint8 ba): 
    _destroy(true)
{
    SDL_Surface* _t_internal_ptr = TTF_RenderUTF8_Shaded(font, text.c_str(), {fr, fg, fb, fa}, {br, bg, bb, ba});
    if(_t_internal_ptr == NULL)
    {
        throw std::runtime_error("Surface could not be created! SDL_Error: " + std::string(SDL_GetError()));
    }
    _internal_ptr = _t_internal_ptr;
}

TSDL::TSDL_Surface::~TSDL_Surface()
{
    if(_destroy) SDL_FreeSurface(_internal_ptr);
}

TSDL::TSDL_Surface::operator SDL_Surface*() const
{
    return _internal_ptr;
}

const SDL_PixelFormat* TSDL::TSDL_Surface::get_format() const
{
    return _internal_ptr->format;
}

int TSDL::TSDL_Surface::copy_from(_SDL_Surface src, const SDL_Rect* srcrect, SDL_Rect* dstrect)
{
    int _t = SDL_BlitSurface(src, srcrect, _internal_ptr, dstrect);
    if(_t != 0)
    {
        throw std::runtime_error("Cannot copy from surface! SDL_Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::TSDL_Surface::scale_from(_SDL_Surface src, const SDL_Rect* srcrect, SDL_Rect* dstrect)
{
    int _t = SDL_BlitScaled(src, srcrect, _internal_ptr, dstrect);
    if(_t != 0)
    {
        throw std::runtime_error("Cannot scaled copy from surface! SDL_Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::TSDL_Surface::fill_rect(const SDL_Rect* rect, Uint32 color)
{
    int _t = SDL_FillRect(_internal_ptr, rect, color);
    if(_t != 0)
    {
        throw std::runtime_error("Cannot copy from surface! SDL_Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::TSDL_Surface::fill_rect(const SDL_Rect* rect, Uint8 r, Uint8 g, Uint8 b)
{
    return this->fill_rect(rect, this->map_rgb(r, g, b));
}

int TSDL::TSDL_Surface::set_color_key(bool flag, Uint32 key)
{
    int _t = SDL_SetColorKey(_internal_ptr, flag, key);
    if(_t != 0)
    {
        throw std::runtime_error("Could not set surface color key! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::TSDL_Surface::set_color_key(bool flag, Uint8 r, Uint8 g, Uint8 b)
{
    return this->set_color_key(flag, this->map_rgb(r, g, b));
}

Uint32 TSDL::TSDL_Surface::map_rgb(Uint8 r, Uint8 g, Uint8 b) const
{
    return SDL_MapRGB(_internal_ptr->format, r, g, b);
}

SDL_Surface* _safe_convert_surface(SDL_Surface* src, const SDL_PixelFormat* fmt, Uint32 flags)
{
    SDL_Surface* _t = SDL_ConvertSurface(src, fmt, flags);
    if(_t == NULL)
    {
        throw std::runtime_error("Cannot convert surface! SDL_Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

TSDL::TSDL_Surface TSDL::TSDL_Surface::get_converted_surface(const SDL_PixelFormat* fmt, Uint32 flags) const
{
    return TSDL::TSDL_Surface(_safe_convert_surface(_internal_ptr, fmt, flags), true);
}

void TSDL::TSDL_Surface::convert_surface(const SDL_PixelFormat* fmt, Uint32 flags)
{
    SDL_Surface* _t_internal_ptr = _safe_convert_surface(_internal_ptr, fmt, flags);
    if(_destroy) SDL_FreeSurface(_internal_ptr);
    _internal_ptr = _t_internal_ptr;
    _destroy = true;
}

_TSDL_EXPAND_DECLARE_MASK_TYPE(TSDL, Surface)

#ifdef TSDL_EXPOSE_PYBIND11

_PY_EXPAND_DEFINE_TYPEERASE_FUNCTIONS(_PY, Surface)

#endif