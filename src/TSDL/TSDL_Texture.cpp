#include "TSDL/TSDL_Texture.hpp"
#include <SDL.h>
#include <stdexcept>

TSDL::TSDL_Texture::TSDL_Texture(TSDL::TSDL_Renderer renderer, const std::string& file)
{
    SDL_Surface* _t_surface = IMG_Load(file.c_str());
    if(_t_surface == NULL)
    {
        throw std::runtime_error("Texture surface could not be created! SDL_Error: " + std::string(SDL_GetError()));
    }
    SDL_Texture* _t_internal_ptr = SDL_CreateTextureFromSurface(renderer, _t_surface);
    SDL_FreeSurface(_t_surface);
    if(_t_internal_ptr == NULL)
    {
        throw std::runtime_error("Texture could not be created! SDL Error: " + std::string(SDL_GetError()));
    }
    _internal_ptr = _t_internal_ptr;
}

TSDL::TSDL_Texture::TSDL_Texture(TSDL::TSDL_Renderer renderer, const std::string& file, Uint8 r, Uint8 g, Uint8 b)
{
    SDL_Surface* _t_surface = IMG_Load(file.c_str());
    if(_t_surface == NULL)
    {
        throw std::runtime_error("Texture surface could not be created! SDL_Error: " + std::string(SDL_GetError()));
    }
    int _t = SDL_SetColorKey(_t_surface, true, SDL_MapRGB(_t_surface->format, r, g, b));
    if(_t != 0)
    {
        SDL_FreeSurface(_t_surface);
        throw std::runtime_error("Could not set texture surface color key! SDL Error: " + std::string(SDL_GetError()));
    }
    SDL_Texture* _t_internal_ptr = SDL_CreateTextureFromSurface(renderer, _t_surface);
    SDL_FreeSurface(_t_surface);
    if(_t_internal_ptr == NULL)
    {
        throw std::runtime_error("Texture could not be created! SDL Error: " + std::string(SDL_GetError()));
    }
    _internal_ptr = _t_internal_ptr;
}

TSDL::TSDL_Texture::TSDL_Texture(TSDL::TSDL_Renderer renderer, _SDL_Surface surface)
{
    SDL_Texture* _t_internal_ptr = SDL_CreateTextureFromSurface(renderer, surface);
    if(_t_internal_ptr == NULL)
    {
        throw std::runtime_error("Texture could not be created! SDL Error: " + std::string(SDL_GetError()));
    }
    _internal_ptr = _t_internal_ptr;
}

TSDL::TSDL_Texture::~TSDL_Texture()
{
    SDL_DestroyTexture(_internal_ptr);
}

TSDL::TSDL_Texture::operator SDL_Texture*() const
{
    return _internal_ptr;
}

int TSDL::TSDL_Texture::set_blend_mode(SDL_BlendMode mode)
{
    int _t = SDL_SetTextureBlendMode(_internal_ptr, mode);
    if(_t != 0)
    {
        throw std::runtime_error("Could not set blend mode! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::TSDL_Texture::get_blend_mode(SDL_BlendMode* mode)
{
    int _t = SDL_GetTextureBlendMode(_internal_ptr, mode);
    if(_t != 0)
    {
        throw std::runtime_error("Could not get blend mode! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::TSDL_Texture::set_alpha_multiplier(Uint8 a)
{
    int _t = SDL_SetTextureAlphaMod(_internal_ptr, a);
    if(_t != 0)
    {
        throw std::runtime_error("Could not set alpha! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::TSDL_Texture::get_alpha_multiplier(Uint8* a)
{
    int _t = SDL_GetTextureAlphaMod(_internal_ptr, a);
    if(_t != 0)
    {
        throw std::runtime_error("Could not get alpha! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::TSDL_Texture::set_color_multiplier(Uint8 r, Uint8 g, Uint8 b)
{
    int _t = SDL_SetTextureColorMod(_internal_ptr, r, g, b);
    if(_t != 0)
    {
        throw std::runtime_error("Could not set color multiplier! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

int TSDL::TSDL_Texture::get_color_multiplier(Uint8* r, Uint8* g, Uint8* b)
{
    int _t = SDL_GetTextureColorMod(_internal_ptr, r, g, b);
    if(_t != 0)
    {
        throw std::runtime_error("Could not get color multiplier! SDL Error: " + std::string(SDL_GetError()));
    }
    return _t;
}

_TSDL_EXPAND_DECLARE_MASK_TYPE(TSDL, Texture)

#ifdef TSDL_EXPOSE_PYBIND11

_PY_EXPAND_DEFINE_TYPEERASE_FUNCTIONS(_PY, Texture)

#endif