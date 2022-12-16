#pragma once
namespace nonstd {
    template <class T, class Allocator = allocator<T>>
    class list : public vector<T, Allocator> {
    public:
        using iterator = typename vector<T, Allocator>::iterator;
        using const_iterator = typename vector<T, Allocator>::const_iterator;
        using reverse_iterator = typename vector<T, Allocator>::reverse_iterator;
        using const_reverse_iterator = typename vector<T, Allocator>::const_reverse_iterator;

        // 拷贝构造函数
        list(const list& other) : vector<T, Allocator>(other) {}

        // 拷贝赋值操作符
        list& operator=(const list& other) {
            vector<T, Allocator>::operator=(other);
            return *this;
        }

        // 移动构造函数
        list(list&& other) : vector<T, Allocator>(nonstd::move(other)) {}

        // 移动赋值操作符
        list& operator=(list&& other) {
            vector<T, Allocator>::operator=(nonstd::move(other));
            return *this;
        }

        // 常用操作
        _INLINE void insert(const_iterator pos, const T& value) {
            vector<T, Allocator>::insert(pos, value);
        }
        _INLINE void insert(const_iterator pos, T&& value) {
            vector<T, Allocator>::insert(pos, nonstd::move(value));
        }
        _INLINE void erase(const_iterator pos) {
            vector<T, Allocator>::erase(pos);
        }
        _INLINE void erase(const_iterator first, const_iterator last) {
            vector<T, Allocator>::erase(first, last);
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
    };
}