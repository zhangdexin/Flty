#ifndef __UTILS_H__
#define __UTILS_H__

#include <optional>

#define PACK_SET_MEMBER(classname, member, type, param)  \
    classname& param(const type& param) {                \
        member = param;                                  \
        return *this;                                    \
    }                                                    \

namespace flty {

// utils
class Utils
{

};

// optional
//template <typename T>
//constexpr bool has_value(const std::optional<T> x) noexcept
//{
//    return x.has_value();
//}
//
//template <typename T, typename...Args>
//constexpr bool has_value(const std::optional<T>& first,
//    const std::optional<Args>&... other) noexcept
//{
//    return first.has_value() && has_value(other...);
//}
//
//template <typename F>
//auto lift_optional(F &&f)
//{
//    return [f = std::forward<F>(f)](auto &&... args) {
//        typedef
//            std::decay_t<decltype(f(std::forward<decltype(args)>(args).value()...))>
//            result_type;
//        if (has_value(args...)) {
//            return std::optional<result_type>(
//                f(std::forward<decltype(args)>(args).value()...));
//        }
//        else {
//            return std::optional<result_type>();
//        }
//    }
//}

}


#endif