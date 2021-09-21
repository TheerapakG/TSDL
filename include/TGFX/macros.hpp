#ifndef _TGFX_MACROS
#define _TGFX_MACROS

#define TGFX_NAMESPACE_BEGIN namespace tgfx {
#define TGFX_NAMESPACE_END }

#define TGFX_SUBSPACE_BEGIN(subspace) namespace tgfx::subspace {
#define TGFX_SUBSPACE_END }

#define TGFX_IMPL_BEGIN TGFX_SUBSPACE_BEGIN(impl)
#define TGFX_IMPL_END TGFX_SUBSPACE_END

#define TGFX_SUBSPACE_IMPL_BEGIN(subspace) namespace tgfx::subspace::impl {
#define TGFX_SUBSPACE_IMPL_END }

#define TGFX_STD_BEGIN namespace std {
#define TGFX_STD_END }

#ifdef TGFX_ENABLE_BOOST_STACKTRACE
#define TGFX_ENABLE_STACKTRACE
#endif

#ifdef TGFX_HEADER_ONLY
#define TGFX_INLINE_HEADER inline
#else
#define TGFX_INLINE_HEADER
#endif

#endif
