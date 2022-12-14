#pragma once
template<class T>
class allcator {//内存分配器
public:
	using allocator_type = T;
	_INLINE static T* allocate(size_t size) {//分配内存
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
	_INLINE static void copy(T* dest, T* src, size_t size) {//复制内存
		if (dest == nullptr || src == nullptr)return;
		::memcpy(dest, src, size * sizeof(T));
	}
	_INLINE static void move(T* dest, T* src, size_t size) {//移动内存
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

	这段代码定义了一个名为 allocator 的类，该类实现了一个内存分配器，用于在 Windows 内核中分配和释放内存。

	该类定义了两个成员函数：alloc 和 free。alloc 函数用于分配内存，接收一个 size_t 类型的参数，表示要分配的内存大小。

	它使用了 Windows 内核函数 ExAllocatePoolWithTag 来分配内存，并使用 RtlZeroMemory 函数将分配的内存清零。如果分配成功，

	alloc 函数返回分配的内存的指针；否则，返回 nullptr。

	deallocate 函数用于释放内存，接收一个指向要释放的内存块的指针。它首先使用 MmIsAddressValid 函数判断指针指向的内存地址是否有效，

	然后使用 ExFreePoolWithTag 函数释放内存。

	该类还定义了一个 value_type 别名，表示该内存分配器分配的内存块的数据类型。

*/