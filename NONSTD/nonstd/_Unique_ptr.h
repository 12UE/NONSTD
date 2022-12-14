#pragma once
namespace nonstd {
    template<class T, class Alloc = allcator<T>>
    class unique_ptr {
    public:
        unique_ptr(T* p = nullptr) : ptr_(p) {}

        ~unique_ptr() {
            Alloc::deallocate(ptr_);
            //调用析构函数
            Alloc::destroy(ptr_);
        }

        unique_ptr(const unique_ptr&) = delete;
        unique_ptr& operator=(const unique_ptr&) = delete;

        unique_ptr(unique_ptr&& other) : ptr_(other.ptr_) {
            other.ptr_ = nullptr;
        }

        unique_ptr& operator=(unique_ptr&& other) {
            if (this != &other) {
                Alloc::deallocate(ptr_);
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
                Alloc::deallocate(ptr_);
                ptr_ = p;
            }
        }

        _INLINE T& operator*() const { return *ptr_; }
        _INLINE T* operator->() const { return ptr_; }

        operator bool() const { return ptr_ != nullptr; }

    private:
        T* ptr_;
    };
    //实现一个make_unique 使用内存分配器
    template<class T,class _Alloc=allcator<T>, class... Args>
    _INLINE unique_ptr<T> make_unique(Args&&... args) {
        //分配内存
        T* p = _Alloc::allocate(1);
        //构造对象
        _Alloc::construct(p, nonstd::forward<Args>(args)...);
        //返回unique_ptr
        return unique_ptr<T>(p);
    }
}