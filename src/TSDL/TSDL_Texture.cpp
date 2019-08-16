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

TSDL::TSDL_Texture::~TSDL_Texture()
{
    SDL_DestroyTexture(_internal_ptr);
}

TSDL::TSDL_Texture::operator SDL_Texture*() const
{
    return _internal_ptr;
}

_TSDL_EXPAND_DECLARE_MASK_TYPE(TSDL, Texture)

#ifdef TSDL_EXPOSE_PYBIND11

_PY_EXPAND_DEFINE_TYPEERASE_FUNCTIONS(_PY, Texture)

#endif