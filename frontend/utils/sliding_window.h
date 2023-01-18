#ifndef SIMPLE_SLIDING_WINDOW_H
#define SIMPLE_SLIDING_WINDOW_H

#include "../lexer/tokens/token.h"

#include <vector>
#include <span>
#include <memory>

template <typename T>
class SlidingWindow {
public:
    SlidingWindow(std::span<T> container): container(container), it(container.begin()) {}
    virtual ~SlidingWindow() = default;

    T peek() {
        assert(!atEnd());
        return *it;
    }

    T advance() {
        assert(!atEnd());
        return *it++;
    }

    bool atEnd() {
        return it == container.end();
    }

    void reset() {
        it = container.begin();
    }

private:
    typename std::span<T> container;
    typename std::span<T>::iterator it;
};

#endif