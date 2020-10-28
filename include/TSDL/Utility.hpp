//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_UTILITY_
#define TSDL_UTILITY_

#include <SDL2/SDL.h>
#include <functional>
#include <tuple>
#include <type_traits>
#include <thread>
#include <stdexcept>

#ifndef __cpp_exceptions
#include <new>
#endif

namespace TSDL
{
    extern thread_local const std::thread::id current_thread_id;

    /*
    Helper type allowing class to call its private constructor
    */
    struct _pconstruct_t {};

    inline constexpr _pconstruct_t pconstruct;

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

    template <typename T>
    T or_enum(T _self, T _other)
    {
        using underlying_t = std::underlying_type_t<T>;
        return static_cast<T>(static_cast<underlying_t>(_self)|static_cast<underlying_t>(_other));
    }

    template <typename T>
    T and_enum(T _self, T _other)
    {
        using underlying_t = std::underlying_type_t<T>;
        return static_cast<T>(static_cast<underlying_t>(_self)&static_cast<underlying_t>(_other));
    }

    enum class Orientation
    {
        HORIZONTAL = 0b1,
        X = 0b1,
        VERTICAL = 0b10,
        Y = 0b10
    };

    Orientation operator|(Orientation lhs, Orientation rhs);
    Orientation operator&(Orientation lhs, Orientation rhs);

    template <typename ...Ts>
    struct get_tuple
    {
        using type = std::tuple<Ts...>;
    };

    template <typename T1, typename T2>
    struct get_tuple<T1, T2>
    {
        using type = std::pair<T1, T2>;
    };

    template <typename ...Ts>
    using get_tuple_t = typename get_tuple<Ts...>::type;

    template <typename ...Ts>
    std::tuple<Ts...> make_tuple(Ts&&... args)
    {
        return std::make_tuple(std::forward<Ts>(args)...);
    }

    template <typename T1, typename T2>
    std::pair<T1, T2> make_tuple(T1&& arg1, T2&& arg2)
    {
        return std::make_pair(std::forward<T1>(arg1), std::forward<T2>(arg2));
    }

    using _point_2d = get_tuple_t<int, int>;
    class point_2d: public SDL_Point
    {
        public:
        point_2d() = default;
        point_2d(int x, int y);
        point_2d(SDL_Point& p);
        point_2d(_point_2d& p);

        point_2d operator+() const;
        point_2d operator+(const point_2d& other) const;
        point_2d operator-() const;
        point_2d operator-(const point_2d& other) const;

        point_2d operator*(const int other) const;
        point_2d operator/(const int other) const;

        point_2d& operator+=(const point_2d& other);
        point_2d& operator-=(const point_2d& other);

        bool operator!=(const point_2d& other) const;

        operator _point_2d() const;
    };

    using _rect = get_tuple_t<point_2d, point_2d>;
    class rect: public SDL_Rect
    {
        public:
        rect(int x, int y, int w, int h);
        rect(SDL_Rect& r);
        rect(_rect& r);
        rect(const std::pair<point_2d, point_2d>& point_pair);
        rect(const point_2d& topleft, const point_2d& bottomright);

        point_2d topleft() const;
        point_2d bottomright() const;

        bool operator!=(const rect & other) const;

        operator _rect() const;
    };

    using _color_rgb = get_tuple_t<Uint8, Uint8, Uint8>;
    class color_rgb: public SDL_Color
    {
        public:
        color_rgb(Uint8 r, Uint8 g, Uint8 b);
        color_rgb(SDL_Color& c);
        color_rgb(_color_rgb& c);

        operator _color_rgb();
    };

    using _color_rgba = get_tuple_t<Uint8, Uint8, Uint8, Uint8>;
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

    using _premul_color_rgba = get_tuple_t<double, double, double, double>;
    class premul_color_rgba
    {
        public:
        double r, g, b, a;
        premul_color_rgba(double r, double g, double b, double a);
        premul_color_rgba(_premul_color_rgba& c);

        operator _premul_color_rgba();
    };
}

bool operator==(const TSDL::point_2d& lhs, const TSDL::point_2d& rhs);

TSDL::point_2d operator*(const int lhs, const TSDL::point_2d& rhs);

#endif