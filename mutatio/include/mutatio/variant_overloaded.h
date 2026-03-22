#pragma once

#include <variant>

namespace mutatio {

template <class... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};

template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

template <typename, typename>
constexpr bool is_one_of_variants_types = false;

template <typename... Ts, typename T>
constexpr bool is_one_of_variants_types<std::variant<Ts...>, T> =
    (std::is_same_v<T, Ts> || ...);

}  // namespace mutatio
