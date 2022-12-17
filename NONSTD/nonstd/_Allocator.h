#pragma once
namespace nonstd {
#if defined (_WIN64)
	constexpr auto USERADDR_MAX = 0x7fffffff0000;
#else
	constexpr auto USERADDR_MAX = 0xBFFE'FFFF;
#endif
	static auto USERADDR_MIN = 0x10000;
	constexpr auto MemReadableProtectMask = PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE;

	bool MmIsAddressValid(void* addr) {
		if ((size_t)addr < USERADDR_MIN || (size_t)addr > USERADDR_MAX) return false;
		MEMORY_BASIC_INFORMATION mbi{};
		if (!VirtualQuery(addr, &mbi, sizeof(mbi))) return false;
		if (mbi.State != MEM_COMMIT) return false;
		if ((mbi.Protect & MemReadableProtectMask) == 0) return false;
		return true;
	}
	
	template<class T>
	class allocator {//内存分配器
		using __byte = unsigned char;
	public:
		using allocator_type = T;
		_INLINE static T* Alloc(size_t nSize) {//分配内存
			size_t AllocateSize = nSize * sizeof(T);
			if (AllocateSize == 0) return nullptr;
			T* p = (T*)new __byte[AllocateSize];
			return p;
		}
		
		_INLINE static void Free(T* ptr) {
			if (CheckAddr(ptr)) {
				delete ptr;
			}
		}
		_INLINE static void Free(T* ptr, size_t nSize) {
			if (CheckAddr(ptr)) {
				delete[] ptr;
			}
		}
		_INLINE static void copy(T* dest, T* src, size_t nSize) {//注意 :内存复制不会调用拷贝构造函数
			if (!CheckAddr(dest) || !CheckAddr(src))return;
			::memcpy(dest, src, nSize * sizeof(T));
		}
		
		_INLINE static void move(T* dest, T* src, size_t nSize) {//注意 :内存移动不会调用拷贝构造函数
			if (!CheckAddr(dest) || !CheckAddr(src))return;
			::memmove(dest, src, nSize * sizeof(T));
		}
		_INLINE  static void fill(T* first, T* last, const T& value) {
			if (!CheckAddr(first) || !CheckAddr(last))return;
			for (auto p = first; p != last; ++p) *p = value;
		}
		
		_INLINE static void fill_n(T* first, size_t n, const T& value) {
			if (!CheckAddr(first))return;
			for (size_t i = 0; i < n; ++i) first[i] = value;
		}
		_INLINE  static void construct(T* p, const T& value) { if (CheckAddr(p)) new (p) T(value); }
		
		
		_INLINE  static void destroy(T* p) {
			if (CheckAddr(p)) p->~T(); 
		}
		
		template<class... Args>static void construct(T* p, Args&&... args) {
			if (CheckAddr(p)) new (p) T(nonstd::forward<Args>(args)...);
		}
		_INLINE static int  max_size() { 
			return 0x7fffffff; 
		}
		_INLINE static int compare(const T* first, const T* last, size_t nSize) {
			if (!CheckAddr(first) || !CheckAddr(last))return 0;
			return ::memcmp((void*)first, (void*)last, nSize);
		}
		_INLINE static bool CheckAddr(T* addr) { return addr != nullptr && MmIsAddressValid((PVOID)addr); }
	};
}

/*
* template<class T>
	class allocator {//内存分配器
		using __byte = unsigned char;
	public:
		using allocator_type = T;
		_INLINE static T* Alloc(size_t nSize);
		_INLINE static void Free(T* ptr) ;
		_INLINE static void Free(T* ptr, size_t nSize);
		_INLINE static void copy(T* dest, T* src, size_t nSize) ;
		_INLINE static void move(T* dest, T* src, size_t nSize);
		_INLINE  static void fill(T* first, T* last, const T& value);
		_INLINE static void fill_n(T* first, size_t n, const T& value) ;
		_INLINE  static void construct(T* p, const T& value) ;
		_INLINE  static void destroy(T* p) ;
		template<class... Args>static void construct(T* p, Args&&... args)
		_INLINE static int  max_size() ;
		_INLINE static int compare(const T* first, const T* last, size_t nSize);
		_INLINE static bool CheckAddr(T* addr);
	};
*/