#pragma once
template<class T>
class allcator {//�ڴ������
public:
	using allocator_type = T;
	_INLINE static T* allocate(size_t size) {//�����ڴ�
		size_t AllocateSize = size * sizeof(T);
		if (size == 0)return nullptr;
		T* p = (T*)malloc(AllocateSize);
		if (p != nullptr) {
			::memset(p,0,AllocateSize);
			return p;
		}
		return nullptr;
	}
	_INLINE static void deallocate(T* p) {if (p) ::free(p), p = nullptr;}
	_INLINE static void copy(T* dest, T* src, size_t size) {//�����ڴ�
		if (dest == nullptr || src == nullptr)return;
		::memcpy(dest, src, size * sizeof(T));
	}
	_INLINE static void move(T* dest, T* src, size_t size) {//�ƶ��ڴ�
		if (dest == nullptr || src == nullptr)return;
		::memcpy(dest, src, size * sizeof(T));
	}
	template <typename T>
	_INLINE  static T* realloc(T* org, size_t size) {
		if (size == 0) {
			deallocate(org);
			return nullptr;
		}
		auto p = allocate(size);
		if (!p)return nullptr;
		allcator::copy(p, org, size);
		deallocate(org);
		return p;
	}
	_INLINE  static void fill(T* first, T* last, const T& value) {
		if (first == nullptr || last == nullptr)return;
		for (auto p = first; p != last; ++p) *p = value;
	}
	_INLINE  static void construct(T* p, const T& value) {if(p!=nullptr) new (p) T(value);}
	_INLINE  static void destroy(T* p) {if (p != nullptr) p->~T();}
	template<class... Args>static void construct(T* p, Args&&... args) {
		if (p != nullptr) new (p) T(nonstd::forward<Args>(args)...);
	}
	_INLINE static int  max_size() { return 0x7fffffff; }
	_INLINE static int compare(const T* first, const T* last,size_t nSize) {
		return ::memcmp((void*)first,(void*)last, nSize);
	}
};
/*

	��δ��붨����һ����Ϊ allocator ���࣬����ʵ����һ���ڴ�������������� Windows �ں��з�����ͷ��ڴ档

	���ඨ����������Ա������alloc �� free��alloc �������ڷ����ڴ棬����һ�� size_t ���͵Ĳ�������ʾҪ������ڴ��С��

	��ʹ���� Windows �ں˺��� ExAllocatePoolWithTag �������ڴ棬��ʹ�� RtlZeroMemory ������������ڴ����㡣�������ɹ���

	alloc �������ط�����ڴ��ָ�룻���򣬷��� nullptr��

	deallocate ���������ͷ��ڴ棬����һ��ָ��Ҫ�ͷŵ��ڴ���ָ�롣������ʹ�� MmIsAddressValid �����ж�ָ��ָ����ڴ��ַ�Ƿ���Ч��

	Ȼ��ʹ�� ExFreePoolWithTag �����ͷ��ڴ档

	���໹������һ�� value_type ��������ʾ���ڴ������������ڴ����������͡�

*/