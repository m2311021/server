//
// Created by ristle on 9/16/24.
//
module;

#include <concepts>

export module tools;


export namespace tools {
    template<typename T>
    concept OverridableOperator = requires(T t) {
        { t& t } -> std::same_as<T>;
    };
}