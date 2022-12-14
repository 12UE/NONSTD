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

}