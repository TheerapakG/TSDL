#ifndef TSDL_UTILITY_
#define TSDL_UTILITY_

#include <SDL.h>
#include <functional>
#include <tuple>
#include <stdexcept>

#ifndef __cpp_exceptions
#include <new>
#endif

namespace TSDL
{
#ifdef __cpp_exceptions    
    template <typename Exc, typename ...Args>
    bool safe_throw(Args... args)
    {
        throw Exc(args...);
    }
#else
    extern std::exception* _current_exc;
    extern std::function<void(void)> unhandleable_exc_handler;

    void clear_exc();

    template <typename Exc, typename ...Args>
    bool safe_throw(Args... args)
    {
        delete _current_exc;
        _current_exc = new (std::nothrow) Exc(args...);
        if (_current_exc == nullptr) unhandleable_exc_handler();
        return false;
    }

    template <typename T, typename SDLType = typename T::SDL_Type>
    bool check_integrity(T& obj)
    {
        return static_cast<SDLType*>(obj) != nullptr;
    }

    template <typename T, std::enable_if_t<std::is_same_v<decltype(T::constructed), bool>>* = nullptr>
    bool check_integrity(T& obj)
    {
        return obj.constructed;
    }

    std::exception& get_exc();
#endif

    template <typename ...Ts>
    using tuple = std::tuple<Ts...>;

    using _point_2d = tuple<int, int>;
    class point_2d: public SDL_Point
    {
        public:
        point_2d(int x, int y);
        point_2d(SDL_Point& p);
        point_2d(_point_2d& p);

        operator _point_2d();
    };

    using _rect = tuple<point_2d, point_2d>;
    class rect: public SDL_Rect
    {
        public:
        rect(int x, int y, int w, int h);
        rect(SDL_Rect& r);
        rect(_rect& r);

        operator _rect();
    };

    using _color_rgb = tuple<Uint8, Uint8, Uint8>;
    class color_rgb: public SDL_Color
    {
        public:
        color_rgb(Uint8 r, Uint8 g, Uint8 b);
        color_rgb(SDL_Color& c);
        color_rgb(_color_rgb& c);

        operator _color_rgb();
    };

    using _color_rgba = tuple<Uint8, Uint8, Uint8, Uint8>;
    class color_rgba: public SDL_Color
    {
        public:
        #ifdef TSDL_USE_EMSCRIPTEN
        Uint8 a;
        #endif
        color_rgba(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
        color_rgba(SDL_Color& c);
        color_rgba(_color_rgba& c);

        color_rgba(color_rgb& c);

        operator _color_rgba();
    };
}

#endif