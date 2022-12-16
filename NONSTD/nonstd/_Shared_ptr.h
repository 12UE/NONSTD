#pragma once
namespace nonstd {
	template<typename T>
	class shared_ptr
	{
	public:
		shared_ptr() : m_ptr(nullptr), m_refCount(nullptr) {}

		shared_ptr(T* ptr) : m_ptr(ptr), m_refCount(allocator<size_t>::Alloc(1)) {}

		shared_ptr(const shared_ptr<T>& other) : m_ptr(other.m_ptr), m_refCount(other.m_refCount)
		{
			if (m_refCount)
				++(*m_refCount);
		}
		~shared_ptr()
		{
			if (m_refCount && --(*m_refCount) == 0)
			{
				allocator<T>::Free(m_ptr);
				allocator<size_t>::Free(m_refCount);
			}
		}

		shared_ptr<T>& operator=(const shared_ptr<T>& other)
		{
			if (this != &other)
			{
				if (m_refCount && --(*m_refCount) == 0)
				{
					allocator<T>::Free(m_ptr);
					allocator<size_t>::Free(m_refCount);
				}

				m_ptr = other.m_ptr;
				m_refCount = other.m_refCount;

				if (m_refCount)
					++(*m_refCount);
			}

			return *this;
		}

		T* operator->() const
		{
			return m_ptr;
		}

		T& operator*() const
		{
			return *m_ptr;
		}

		size_t use_count() const
		{
			return m_refCount ? *m_refCount : 0;
		}
		T* get() const { return m_ptr; }
		
		//жиди[]
		T& operator[](size_t index) const
		{
			return m_ptr[index];
		}

	private:
		T* m_ptr;
		size_t* m_refCount;
	};
	//make_shared
	template<typename T>
	shared_ptr<T> make_shared(T* ptr){
		return shared_ptr<T>(ptr);
	}
}