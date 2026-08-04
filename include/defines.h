#pragma once
#include <cstdint>
#include <memory>

//=====Types redefinition ===========

using char8 = std::int8_t;
using int8  = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;

using byte   = std::uint8_t;
using uint8  = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

//===================================

template <typename T>
using uptr = std::unique_ptr<T>;

template <typename T>
using sptr = std::shared_ptr<T>;

template <typename T>
using wptr = std::weak_ptr<T>;

//===================================

enum class OS { WIN, MAC, LINUX };

#if defined(_WIN32) || defined(_WIN64)
#   define OS_WIN
#elif __APPLE__
#   include "TargetConditionals.h"
#   if TARGET_OS_MAC
#       define OS_MAC
#   endif
#elif defined(__linux__)
#   define OS_LINUX
#else
#   error "Unsupported os"
#endif

//===================================

#define ENUM_CLASS_FLAGS(EnumType)                                                \
using T = std::underlying_type_t<EnumType>;                                       \
inline constexpr EnumType operator|(EnumType lhs, EnumType rhs) {                 \
    return static_cast<EnumType>(static_cast<T>(lhs) | static_cast<T>(rhs));      \
}                                                                                 \
inline constexpr EnumType& operator|=(EnumType& lhs, EnumType rhs) {              \
    lhs = lhs | rhs;                                                              \
    return lhs;                                                                   \
}                                                                                 \
inline constexpr EnumType operator&(EnumType lhs, EnumType rhs) {                 \
    return static_cast<EnumType>(static_cast<T>(lhs) & static_cast<T>(rhs));      \
}                                                                                 \
inline constexpr EnumType& operator&=(EnumType& lhs, EnumType rhs) {              \
    lhs = lhs & rhs;                                                              \
    return lhs;                                                                   \
}                                                                                 \
inline constexpr EnumType operator~(EnumType rhs) {                               \
    return static_cast<EnumType>(~static_cast<T>(rhs));                           \
}                                                                                 \
inline constexpr bool Any(EnumType value) {                                       \
    return static_cast<T>(value) != 0;                                            \
}                                                                                 \
inline constexpr T cast(EnumType value)                                           \
{                                                                                 \
    return static_cast<T>(value);                                                 \
}
