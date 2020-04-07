#include "TSDL/TSDL_Font.hpp"
#include "TSDL/TSDL_Utility.hpp"

#ifdef TSDL_USE_FONTCONFIG
#include "TSDL/TSDL_Main.hpp"
#endif

TSDL_DEFINE_CONSTRUCT(TSDL, Font)

TSDL::TSDL_Font::TSDL_Font(TTF_Font* ptr): TSDL_Font(ptr, false) {}

TSDL::TSDL_Font::TSDL_Font(TTF_Font* ptr, bool handle_destroy): _internal_ptr(ptr), _destroy(handle_destroy) {}

TSDL::TSDL_Font::TSDL_Font(const std::_TSDL_U8(string)& file, int pt): _destroy(true)
{
    TTF_Font* _t_internal_ptr = TTF_OpenFont(reinterpret_cast<const char*>(file.c_str()), pt);
    if(_t_internal_ptr == NULL)
    {
        ::TSDL::safe_throw<std::runtime_error>("Font could not be loaded! SDL_TTF_Error: " + std::string(TTF_GetError()));
        return;
    }
    _internal_ptr = _t_internal_ptr;
}

TSDL::TSDL_Font::TSDL_Font(const std::_TSDL_U8(string)& file, int pt, long index): _destroy(true)
{
    TTF_Font* _t_internal_ptr = TTF_OpenFontIndex(reinterpret_cast<const char*>(file.c_str()), pt, index);
    if(_t_internal_ptr == NULL)
    {
        ::TSDL::safe_throw<std::runtime_error>("Font could not be loaded! SDL_TTF_Error: " + std::string(TTF_GetError()));
        return;
    }
    _internal_ptr = _t_internal_ptr;
}

TTF_Font* _create_font_from_buffer(SDL_RWops* buffer, int pt)
{
    TTF_Font* _t_ptr = TTF_OpenFontRW(buffer, 0, pt); // We free buffer by utilizing the scope
    if(_t_ptr == NULL)
    {
        TSDL::safe_throw<std::runtime_error>("Font could not be loaded! SDL_Mixer_Error: " + std::string(TTF_GetError()));
        return nullptr;
    }
    return _t_ptr;
}

TTF_Font* _create_font_from_buffer(SDL_RWops* buffer, int pt, long index)
{
    TTF_Font* _t_ptr = TTF_OpenFontIndexRW(buffer, 0, pt, index); // We free buffer by utilizing the scope
    if(_t_ptr == NULL)
    {
        TSDL::safe_throw<std::runtime_error>("Font could not be loaded! SDL_Mixer_Error: " + std::string(TTF_GetError()));
        return nullptr;
    }
    return _t_ptr;
}

TSDL::TSDL_Font::TSDL_Font(TSDL_Buffer& buffer, int pt)
{
    TTF_Font* _t_internal_ptr = _create_font_from_buffer(buffer, pt);
    _internal_ptr = _t_internal_ptr;
}

TSDL::TSDL_Font::TSDL_Font(TSDL_Buffer& buffer, int pt, long index)
{
    TTF_Font* _t_internal_ptr = _create_font_from_buffer(buffer, pt, index);
    _internal_ptr = _t_internal_ptr;
}

TSDL::TSDL_Font::TSDL_Font(const void* mem, size_t size, int pt)
{
    ::TSDL::TSDL_Buffer _t_buffer = ::TSDL::TSDL_Buffer(mem, size);
    TTF_Font* _t_internal_ptr = _create_font_from_buffer(_t_buffer, pt);
    _internal_ptr = _t_internal_ptr;
}

TSDL::TSDL_Font::TSDL_Font(const void* mem, size_t size, int pt, long index)
{
    ::TSDL::TSDL_Buffer _t_buffer = ::TSDL::TSDL_Buffer(mem, size);
    TTF_Font* _t_internal_ptr = _create_font_from_buffer(_t_buffer, pt, index);
    _internal_ptr = _t_internal_ptr;
}

TSDL::TSDL_Font::~TSDL_Font()
{
    if(_destroy && (_internal_ptr != nullptr)) TTF_CloseFont(*this);
}

TSDL::TSDL_Font::operator TTF_Font*() const
{
    return _internal_ptr;
}

#ifdef TSDL_USE_FONTCONFIG
std::vector<std::_TSDL_U8(string)> TSDL::get_all_font_filename()
{
    std::vector<std::_TSDL_U8(string)> ret;

    FcConfig* config = FcConfigGetCurrent();
    FcPattern* pat = FcPatternCreate();
    FcObjectSet* os = FcObjectSetBuild(FC_FILE, (const char*)(0));

    FcFontSet* fs = FcFontList(config, pat, os);

    FcPatternDestroy(pat);
    FcObjectSetDestroy(os);

    if(!fs) return ret;

    ret.reserve(fs->nfont);

    for (int i = 0; i < fs->nfont; i++)
    {
        FcPattern* font = fs->fonts[i];
        FcChar8* file;
        if (FcPatternGetString(font, FC_FILE, 0, &file) == FcResultMatch)
        {
            ret.emplace_back((const _TSDL_char*)(file)); //eww
        }
    }

    FcFontSetDestroy(fs);
    return ret;
}

// pattern built from FcPatternBuild, pattern is automatically freed
std::_TSDL_U8(string) _get_pattern_font_filename(FcPattern* pattern)
{
    FcPattern* result = NULL;

    FcConfig* config = FcConfigGetCurrent();
    if(FcConfigSubstitute(config, pattern, FcMatchPattern))
    {
        FcDefaultSubstitute(pattern);
        FcResult matched;
        result = FcFontMatch(config, pattern, &matched);
        if(matched != FcResultMatch)
        {
            FcPatternDestroy(result);
            result = NULL;
        }
    }
    
    FcPatternDestroy(pattern);

    if(!result) return "";

    FcChar8* file;
    FcResult got = FcPatternGetString(result, FC_FILE, 0, &file);
    if (got == FcResultMatch)
    {
        std::string ret((const _TSDL_char*)(file)); //eww
        FcPatternDestroy(result);
        return ret;
    }
    FcPatternDestroy(result);
    return "";
}

std::_TSDL_U8(string) TSDL::get_family_font_filename(const std::_TSDL_U8(string)& family)
{
    return _get_pattern_font_filename(
        FcPatternBuild(NULL, FC_FAMILY, FcTypeString, (const FcChar8*)(reinterpret_cast<const char*>(family.c_str())), (const char *) 0)
    );
}

std::string TSDL::get_name_font_filename(const std::_TSDL_U8(string)& name)
{
    return _get_pattern_font_filename(
        FcNameParse((const FcChar8*)(reinterpret_cast<const char*>(name.c_str())))
    );
}
#endif

#ifdef TSDL_EXPOSE_PYBIND11

_PY_EXPAND_DEFINE_TYPEERASE_FUNCTIONS(_PY, Font)

void _tsdl_font_py(const py::module& m)
{
    py::class_<_PY::_PY_GET_TYPEERASE(Font)>(m, "Font")
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Font)<const std::_TSDL_U8(string), int>())
        .def(_PY::_PY_GET_TYPEERASE_PY_INIT(Font)<const std::_TSDL_U8(string), int, long>())
        .def("__enter__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Font, enter_ctx), py::return_value_policy::reference)
        .def("create", &_PY::_PY_GET_TYPEERASE_FUNCTION(Font, enter_ctx), py::return_value_policy::reference)
        .def("__exit__", &_PY::_PY_GET_TYPEERASE_FUNCTION(Font, exit_ctx));
    py::class_<TSDL::TSDL_Font>(m, "_Font");
}

#endif