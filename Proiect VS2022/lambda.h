#pragma once
#include <functional>

template <class F>
void (* partial(F&& f))() {
    static F fn = std::forward<F>(f);
    return [] {
        return fn();
    };
}

template <class F>
void (*to_func(F&& f))(int, int) {
    static F fn = std::forward<F>(f);
    return [] (int x, int y) {
        return fn(x, y);
    };
}

template <class F>
void (*to_func2(F&& f))(unsigned char, int, int) {
    static F fn = std::forward<F>(f);
    return [](unsigned char c, int x, int y) {
        return fn(c, x, y);
    };
}

template <class F>
void (*to_func3(F&& f))(int, int, int, int) {
    static F fn = std::forward<F>(f);
    return [](int i1, int i2, int i3, int i4) {
        return fn(i1, i2, i3, i4);
    };
}
