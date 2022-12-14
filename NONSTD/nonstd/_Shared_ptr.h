#pragma once
namespace nonstd {
	template<typename T>
	class shared_ptr
	{
	public:
		shared_ptr() : m_ptr(nullptr), m_refCount(nullptr) {}

		shared_ptr(T* ptr) : m_ptr(ptr), m_refCount(allcator<size_t>::allocate(1)) {}

		shared_ptr(const shared_ptr<T>& other) : m_ptr(other.m_ptr), m_refCount(other.m_refCount)
		{
			if (m_refCount)
				++(*m_refCount);
		}
		~shared_ptr()
		{
			if (m_refCount && --(*m_refCount) == 0)
			{
				allcator<T>::deallocate(m_ptr);
				allcator<size_t>::deallocate(m_refCount);
			}
		}

		shared_ptr<T>& operator=(const shared_ptr<T>& other)
		{
			if (this != &other)
			{
				if (m_refCount && --(*m_refCount) == 0)
				{
					allcator<T>::deallocate(m_ptr);
					allcator<size_t>::deallocate(m_refCount);
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

	private:
		T* m_ptr;
		size_t* m_refCount;
	};
}