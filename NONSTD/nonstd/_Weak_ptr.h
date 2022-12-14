#pragma once
namespace nonstd {
    template<class T, class Alloc = allcator<T>>
    class weak_ptr {
    public:
        weak_ptr() : ptr_(nullptr), ref_count_(nullptr) {}

        weak_ptr(const shared_ptr<T>& other)
            : ptr_(other.get()), ref_count_(other.ref_count_) {
            if (ref_count_) ++(*ref_count_);
        }

        weak_ptr(const weak_ptr& other)
            : ptr_(other.ptr_), ref_count_(other.ref_count_) {
            if (ref_count_) ++(*ref_count_);
        }

        ~weak_ptr() {
            if (ref_count_ && --(*ref_count_) == 0) {
                Alloc::deallocate(ptr_);
                Alloc::deallocate(ref_count_);
            }
        }

        weak_ptr& operator=(const weak_ptr& other) {
            if (this != &other) {
                if (ref_count_ && --(*ref_count_) == 0) {
                    Alloc::deallocate(ptr_);
                    Alloc::deallocate(ref_count_);
                }
                ptr_ = other.ptr_;
                ref_count_ = other.ref_count_;
                if (ref_count_) ++(*ref_count_);
            }
            return *this;
        }

        shared_ptr<T> lock() const {
            if (ref_count_ && *ref_count_ > 0) {
                return shared_ptr<T>(ptr_, ref_count_);
            }
            return shared_ptr<T>();
        }

        operator bool() const {
            return ref_count_ && *ref_count_ > 0 && ptr_ != nullptr;
        }

    private:
        T* ptr_;
        int* ref_count_;
    };
}