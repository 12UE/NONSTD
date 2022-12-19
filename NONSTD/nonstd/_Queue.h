#pragma once
namespace nonstd {
    template <typename T, typename Allocator = nonstd::allocator<T>>
    class queue {
    private:
        vector<T, Allocator> m_data;

    public:
        // Constructors
        queue() : m_data() {}
        queue(const queue& other) : m_data(other.m_data) {}
        queue(queue&& other) : m_data(nonstd::move(other.m_data)) {}
        queue(std::initializer_list<T> init) : m_data(init) {}

        // Destructor
        ~queue() {}

        // Assignment operators
        queue& operator=(const queue& other) {
            m_data = other.m_data;
            return *this;
        }
        queue& operator=(queue&& other) {
            m_data = nonstd::move(other.m_data);
            return *this;
        }

        // Modifiers
        void push(const T& value) {
            m_data.push_back(value);
        }
        void push(T&& value) {
            m_data.push_back(nonstd::move(value));
        }
        void pop() {
            m_data.erase(m_data.begin());
        }
        void swap(queue& other) {
            nonstd::swap(m_data, other.m_data);
        }

        // Capacity
        bool empty() const {
            return m_data.empty();
        }
        size_t size() const {
            return m_data.size();
        }

        // Element access
        T& front() {
            return m_data.front();
        }
        const T& front() const {
            return m_data.front();
        }
        T& back() {
            return m_data.back();
        }
        const T& back() const {
            return m_data.back();
        }
    };

}