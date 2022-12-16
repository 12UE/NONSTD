#pragma once
namespace nonstd {
#define _NONSTD_BEGIN namespace nonstd {
#define _NONSTD_END  }	
#define _NONSTD_USING using namespace nonstd;
#define _INLINE_VAR inline
#define _INLINE inline
#define _NODISCARD [[nodiscard]]
#define _CONSTEVAL consteval
#define _NOEXCEPT noexcept
#define _NORETURN [[noreturn]]
#define _LIKELY [[likely]]
#define _UNLIKELY [[unlikely]]
#define _CONSTEXPR constexpr

using max_align_t = double; // most aligned type
#define _EMPTY_ARGUMENT // for empty macro argument
#define _CLASS_DEFINE_CV_REF_NOEXCEPT(CLASS) \
    CLASS(_EMPTY_ARGUMENT)                   \
    CLASS(const)                             \
    CLASS(volatile)                          \
    CLASS(const volatile)                    \
    CLASS(&)                                 \
    CLASS(const&)                            \
    CLASS(volatile&)                         \
    CLASS(const volatile&)                   \
    CLASS(&&)                                \
    CLASS(const&&)                           \
    CLASS(volatile&&)                        \
    CLASS(const volatile&&)                  \
    CLASS(noexcept)                          \
    CLASS(const noexcept)                    \
    CLASS(volatile noexcept)                 \
    CLASS(const volatile noexcept)           \
    CLASS(&noexcept)                         \
    CLASS(const& noexcept)                   \
    CLASS(volatile& noexcept)                \
    CLASS(const volatile& noexcept)          \
    CLASS(&&noexcept)                        \
    CLASS(const&& noexcept)                  \
    CLASS(volatile&& noexcept)               \
    CLASS(const volatile&& noexcept)

}