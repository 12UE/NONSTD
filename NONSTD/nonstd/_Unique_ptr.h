#pragma once
namespace nonstd {
    template<class T, class Alloc = allocator<T>>
    class unique_ptr {
    public:
        unique_ptr(T* p = nullptr) : ptr_(p) {}
        ~unique_ptr() {
            Alloc::Free(ptr_);
            Alloc::destroy(ptr_);
        }
        unique_ptr(const unique_ptr&) = delete;
        unique_ptr& operator=(const unique_ptr&) = delete;
        unique_ptr(unique_ptr&& other) : ptr_(other.ptr_) {
            other.ptr_ = nullptr;
        }
        unique_ptr& operator=(unique_ptr&& other) {
            if (this != &other) {
                Alloc::Free(ptr_);
                ptr_ = other.ptr_;
                other.ptr_ = nullptr;
            }
            return *this;
        }
        _INLINE T* get() const { return ptr_; }
        _INLINE  T* release() {
            T* p = ptr_;
            ptr_ = nullptr;
            return p;
        }
        _INLINE void reset(T* p = nullptr) {
            if (ptr_ != p) {
                Alloc::Free(ptr_);
                ptr_ = p;
            }
        }
        _INLINE T& operator*() const { return *ptr_; }
        _INLINE T* operator->() const { return ptr_; }
        operator bool() const { return ptr_ != nullptr; }

    private:
        T* ptr_;
    };
    template<class T,class _Alloc=allocator<T>, class... Args>
    _INLINE unique_ptr<T> make_unique(Args&&... args) {
        T* p = _Alloc::Alloc(1);
        _Alloc::construct(p, nonstd::forward<Args>(args)...);
        return unique_ptr<T>(p);
    }
}