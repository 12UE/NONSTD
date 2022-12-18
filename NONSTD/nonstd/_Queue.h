#pragma once
namespace nonstd {
    template<class T, class Alloc = allocator<T>>
    class queue {
    public:
        using value_type = T;
        using pointer = T*;
        using size_type = size_t;
        using reference = T&;
        using const_reference = const T&;

        queue() : head_(nullptr), tail_(nullptr), size_(0) {}

        ~queue() { clear(); }

        bool empty() const { return size_ == 0; }

        size_type size() const { return size_; }

        void push(const T& value) {
            auto new_node = Alloc::Alloc(1);
            Alloc::construct(new_node, value);
            if (tail_) {
                tail_->next_ = new_node;
                tail_ = new_node;
            }
            else {
                head_ = tail_ = new_node;
            }
            ++size_;
        }

        void pop() {
            if (head_) {
                auto temp = head_;
                head_ = head_->next_;
                Alloc::Free((T*)temp);
                if (!head_) {
                    tail_ = nullptr;
                }
                --size_;
            }
        }

        reference front() { return head_->value_; }
        const_reference front() const { return head_->value_; }

        void clear() {
            while (head_) {
                auto temp = head_;
                head_ = head_->next_;
                Alloc::destroy(temp);
                Alloc::Free(temp, 1);
            }
            size_ = 0;
            head_ = tail_ = nullptr;
        }

    private:
        struct node {
            T value_;
            node* next_;
        };

        node* head_;
        node* tail_;
        size_type size_;
    };
}