#pragma once
namespace nonstd {
    template <class T, class Allocator = allcator<T>>
    class stack : public vector<T, Allocator> {
    public:
        _INLINE void push(const T& value) {
            vector<T, Allocator>::push_back(value);
        }
        _INLINE void push(T&& value) {
            vector<T, Allocator>::push_back(nonstd::move(value));
        }
        _INLINE void pop() {
            vector<T, Allocator>::pop_back();
        }
        _INLINE T& top() {
            return vector<T, Allocator>::back();
        }
        _INLINE const T& top() const {
            return vector<T, Allocator>::back();
        }
        _INLINE size_t size() const {
            return vector<T, Allocator>::size();
        }
    };
}