#pragma once
namespace nonstd {
    template<class T, class Allocator = allcator<T>>
    class vector {
    public:
        using value_type = T;
        using size_type = size_t;
        using reference = T&;
        using const_reference = const T&;
        using pointer = T*;
        using const_pointer = const T*;
        using iterator = RandomAccessIterator<T>;
        using const_iterator = const iterator;
        
        vector() : m_size(0), m_capacity(0), m_data(nullptr) {}

        vector(size_type count) : m_size(count), m_capacity(count), m_data(Allocator::allocate(count)) {}

        vector(size_type count, const_reference value) : m_size(count), m_capacity(count), m_data(Allocator::allocate(count)) {
            Allocator::fill(m_data, m_data + m_size, value);
        }

        vector(const vector& other) : m_size(other.m_size), m_capacity(other.m_capacity), m_data(Allocator::allocate(other.m_capacity)) {
            Allocator::copy(m_data, other.m_data, m_size);
        }

        vector(vector&& other) : m_size(other.m_size), m_capacity(other.m_capacity), m_data(other.m_data) {
            other.m_size = 0;
            other.m_capacity = 0;
            other.m_data = nullptr;
        }

        template<typename ...elem>
        vector(elem... args) : m_size(sizeof...(args)), m_capacity(sizeof...(args)), m_data(Allocator::allocate(sizeof...(args))) {
            T arr[] = { args... };
            Allocator::copy(m_data, arr, m_size);
        }
        //迭代器构造
        vector(iterator first, iterator last){
            if (first <= last) {
                m_size = last - first;
                m_capacity = last - first;
                m_data = Allocator::allocate(m_capacity);
                nonstd::copy_n(first, m_size, m_data);
            }
        }

        ~vector() {
            clear();
        }

        vector& operator=(const vector& other) {
            if (this != &other) {
                if (m_capacity < other.m_size) {
                    clear();
                    m_data = Allocator::allocate(other.m_capacity);
                    m_capacity = other.m_capacity;
                }
                m_size = other.m_size;
                Allocator::copy(m_data, other.m_data, m_size);
            }
            return *this;
        }

        vector& operator=(vector&& other) {
            if (this != &other) {
                clear();
                m_size = other.m_size;
                m_capacity = other.m_capacity;
                m_data = other.m_data;
                other.m_size = 0;
                other.m_capacity = 0;
                other.m_data = nullptr;
            }
            return *this;
        }

        _CONSTEXPR _INLINE void push_back(const_reference value) {
            if (m_size == m_capacity) {
                auto new_capacity = m_capacity == 0 ? 1 : m_capacity * 2;
                m_data = Allocator::realloc(m_data, new_capacity);
                m_capacity = new_capacity;
            }
            m_data[m_size++] = value;
        }
        template<class T>
        _CONSTEXPR _INLINE void emplace_back(T&& value) {
			if (m_size == m_capacity) {
				auto new_capacity = m_capacity == 0 ? 1 : m_capacity * 2;
				m_data = Allocator::realloc(m_data, new_capacity);
				m_capacity = new_capacity;
			}
			m_data[m_size++] = nonstd::move(value);
		}
        template< class... Args >
        _CONSTEXPR _INLINE void emplace_back(Args&&... args) {
            if (m_size == m_capacity) {
                auto new_capacity = m_capacity == 0 ? 1 : m_capacity * 2;
                m_data = Allocator::realloc(m_data, new_capacity);
                m_capacity = new_capacity;
            }
            Allocator::construct(m_data + m_size, nonstd::forward<Args>(args)...);
            ++m_size;
        }
        template< class... Args >
        _CONSTEXPR _INLINE reference emplace_back(Args&&... args) {
            if (m_size == m_capacity) {
                auto new_capacity = m_capacity == 0 ? 1 : m_capacity * 2;
                m_data = Allocator::realloc(m_data, new_capacity);
                m_capacity = new_capacity;
            }
            Allocator::construct(m_data + m_size, nonstd::forward<Args>(args)...);
            ++m_size;
            return m_data[m_size - 1];
        }
        _CONSTEXPR _INLINE void pop_back() {
            if (m_size > 0) {
                --m_size;
            }
        }
        _CONSTEXPR _INLINE reference front() {
            return m_data[0];
        }

        _CONSTEXPR _INLINE const_reference front() const {
            return m_data[0];
        }

        _CONSTEXPR _INLINE reference back() {
            return m_data[m_size - 1];
        }

        _CONSTEXPR _INLINE const_reference back() const {
            return m_data[m_size - 1];
        }

        _CONSTEXPR _INLINE pointer data() {
            return m_data;
        }

        _CONSTEXPR _INLINE const_pointer data() const noexcept {
            return m_data;
        }

        _CONSTEXPR _INLINE bool empty() const noexcept {
            return m_size == 0;
        }

        _CONSTEXPR _INLINE size_type size() const noexcept {
            return m_size;
        }

        _CONSTEXPR _INLINE size_type capacity() const noexcept {
            return m_capacity;
        }

        _CONSTEXPR _INLINE void clear() {
            if (m_data) {
                for (size_type i = 0; i < m_size; ++i) {
                    m_data[i].~T();
                }
                Allocator::deallocate(m_data);
                m_data = nullptr;
                m_size = 0;
                m_capacity = 0;
            }
        }

        _CONSTEXPR _INLINE void resize(size_type count) {
            resize(count, T());
        }

        _CONSTEXPR _INLINE void resize(size_type count, const_reference value) {
            if (count > m_capacity) {
                auto new_capacity = m_capacity == 0 ? 1 : m_capacity * 2;
                while (new_capacity < count) {
                    new_capacity *= 2;
                }
                m_data = Allocator::realloc(m_data, new_capacity);
                m_capacity = new_capacity;
            }
            if (count > m_size) {
                Allocator::fill(m_data + m_size, m_data + count, value);
            }
            m_size = count;
        }

        _CONSTEXPR _INLINE void reserve(size_type count) {
            if (count > m_capacity) {
                m_data = Allocator::realloc(m_data, count);
                m_capacity = count;
            }
        }

        _CONSTEXPR _INLINE reference operator[](size_type index) {
            return m_data[index];
        }

        _CONSTEXPR _INLINE const_reference operator[](size_type index) const {
            return m_data[index];
        }

        _CONSTEXPR _INLINE reference at(size_type index) {
            if (index >= m_size) {
                DbgPrint("vector::at!");
            }
            return m_data[index];
        }
        //写一个erase
        _CONSTEXPR _INLINE iterator erase(const_iterator pos) {
            if (pos < begin() || pos >= end()) {
                DbgPrint("vector::erase!");
            }
            auto index = pos - begin();
            for (auto i = index; i < m_size - 1; ++i) {
                m_data[i] = m_data[i + 1];
            }
            --m_size;
            return begin() + index;
        }
        //范围删除
        _CONSTEXPR _INLINE iterator erase(iterator first,iterator last) {
            if (first < begin() || first >= end() || last < begin() || last > end() || first > last) {
                DbgPrint("vector::erase!");
            }
            auto index = first - begin();
            auto count = last - first;
            for (auto i = index; i < m_size - count; ++i) {
                m_data[i] = m_data[i + count];
            }
            m_size -= count;
            return begin() + index;
        }
        _CONSTEXPR _INLINE void shrink_to_fit() {
            if (m_size < m_capacity) {
                m_data = Allocator::realloc(m_data, m_size);
                m_capacity = m_size;
            }
        }


        _CONSTEXPR _INLINE const_reference at(size_type index) const {
            if (index >= m_size) {
                DbgPrint("vector::at!");
            }
            return m_data[index];
        }
        _CONSTEXPR _INLINE void swap(vector& other) {
            nonstd::swap(m_size, other.m_size);
            nonstd::swap(m_capacity, other.m_capacity);
            nonstd::swap(m_data, other.m_data);
        }
        // insert 方法
        _CONSTEXPR _INLINE iterator insert(const_iterator pos, const T& value) {
            auto p = Allocator::allocate(size() + 1);
            if (p == nullptr) throw DbgPrint("bad_alloc()");
            Allocator::copy(p, data(), pos - begin());
            *(p + (pos - begin())) = value;
            Allocator::copy(p + (pos - begin()) + 1, pos, end() - pos);
            Allocator::deallocate(data());
            m_data = p;
            return p + (pos - begin());
        }
        _CONSTEXPR _INLINE iterator insert(const_iterator pos, T&& value) {
            auto p = Allocator::allocate(size() + 1);
            if (p == nullptr) DbgPrint("bad_alloc()");
            Allocator::copy(p, data(), pos - begin());
            *(p + (pos - begin())) = nonstd::move(value);
            Allocator::copy(p + (pos - begin()) + 1, pos, end() - pos);
            Allocator::deallocate(data());
            m_data = p;
            return p + (pos - begin());
        }
        iterator begin() { return iterator(m_data); }
        iterator end() { return iterator(m_data) + m_size; }

        const_iterator cbegin() const { return const_iterator(m_data); }
        const_iterator cend() const { return  (const_iterator)iterator(data) + m_size; }

    private:
        size_type m_size;
        size_type m_capacity;
        pointer m_data;
    };
    template< class T, class Alloc, class Pred >
    nonstd::vector<T, Alloc>::size_type erase(nonstd::vector<T, Alloc>& c, Pred pred) {
        auto it = nonstd::remove_if(c.begin(), c.end(), pred);
        auto r = nonstd::distance(it, c.end());
        c.erase(it, c.end());
        return r;
    }
    
}
