#pragma once
namespace nonstd {
    template <class T, class Allocator = allocator<T>>
    class deque : public vector<T, Allocator> {
    public:
        // ...
        _INLINE void push_front(const T& value) {
            vector<T, Allocator>::insert(begin(), value);
        }
        _INLINE void push_front(T&& value) {
            vector<T, Allocator>::insert(begin(), nonstd::move(value));
        }
        _INLINE void push_back(const T& value) {
            vector<T, Allocator>::push_back(value);
        }
        _INLINE void push_back(T&& value) {
            vector<T, Allocator>::push_back(nonstd::move(value));
        }
        _INLINE void pop_front() {
            vector<T, Allocator>::erase(begin());
        }
        _INLINE void pop_back() {
            vector<T, Allocator>::pop_back();
        }
        _INLINE T& front() {
            return vector<T, Allocator>::front();
        }
        _INLINE const T& front() const {
            return vector<T, Allocator>::front();
        }
        _INLINE T& back() {
            return vector<T, Allocator>::back();
        }
        _INLINE const T& back() const {
            return vector<T, Allocator>::back();
        }
        _INLINE size_t size() const {
            return vector<T, Allocator>::size();
        }
        using iterator = vector<T, Allocator>::iterator;
        using const_iterator = vector<T, Allocator>::const_iterator;
        _INLINE iterator begin() {
            return vector<T, Allocator>::m_data;
        }
        _INLINE const_iterator begin() const {
            return vector<T, Allocator>::m_data;
        }
    };
}