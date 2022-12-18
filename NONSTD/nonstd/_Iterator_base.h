#pragma once
namespace nonstd {
	//迭代器类型
	struct normal_iterator_tag {};
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};
    //迭代器基类
    template<class T>
    class _Iterator_base {
    public:
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using difference_type = ptrdiff_t;
        using iterator_category = nonstd::normal_iterator_tag;
        _Iterator_base() : p(nullptr) {}
        _Iterator_base(pointer ptr) : p(ptr) {}
        _Iterator_base& operator++() {
            ++p;
            return *this;
        }
        _Iterator_base operator++(int) {
            _Iterator_base tmp = *this;
            ++p;
            return tmp;
        }
        _Iterator_base& operator--() {
            --p;
            return *this;
        }
        _Iterator_base operator--(int) {
            _Iterator_base tmp = *this;
            --p;
            return tmp;
        }
		
        
        _Iterator_base operator+(difference_type n) {
            _Iterator_base tmp = *this;
            tmp.p += n;
            return tmp;
        }
        _Iterator_base operator-(difference_type n) {
            _Iterator_base tmp = *this;
            tmp.p -= n;
            return tmp;
        }
        _Iterator_base& operator+=(difference_type n) {
            p += n;
            return *this;
        }
        _Iterator_base& operator-=(difference_type n) {
            p -= n;
            return *this;
        }
        difference_type operator-(const _Iterator_base& other) {
            return p - other.p;
        }
        reference operator*() {
            return *p;
        }
        pointer operator->() {
            return p;
        }
        pointer operator&() const { return p; }
        bool operator==(const _Iterator_base& other) {
            return p == other.p;
        }
        bool operator!=(const _Iterator_base& other) {
            return p != other.p;
        }
        bool operator<(const _Iterator_base& other) {
            return p < other.p;
        }
        bool operator>(const _Iterator_base& other) {
            return p > other.p;
        }
        bool operator<=(const _Iterator_base& other) {
            return p <= other.p;
        }
        bool operator>=(const _Iterator_base& other) {
            return p >= other.p;
        }
    private:
        pointer p;
    };
    template<class T>
    using const_iterator_base = const _Iterator_base<T>;
    
    template<class T>
    using RandomAccessIterator = _Iterator_base<T>;
	
    template <typename T>
    struct iterator_traits {
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = nonstd::random_access_iterator_tag;
    };

    template <typename T>
    struct iterator_traits<T*> {
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = nonstd::random_access_iterator_tag;
    };

    template <typename Iter>
    using difference_type = typename iterator_traits<Iter>::difference_type;

    template <typename Iter>
    using value_type = typename iterator_traits<Iter>::value_type;

    template <typename Iter>
    using pointer = typename iterator_traits<Iter>::pointer;

    template <typename Iter>
    using reference = typename iterator_traits<Iter>::reference;

    template <typename Iter>
    using iterator_category = typename iterator_traits<Iter>::iterator_category;

    template <typename Iter>
    using iterator_traits_v = typename iterator_traits<Iter>::value_type;



   
}