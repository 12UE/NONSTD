#pragma once
namespace mystd {
	template <typename First, typename Second, typename ... Next>
	struct is_same {
		template <typename A, typename B>
		struct is_same_min {
			enum { value = false };
		};

		template <typename A>
		struct is_same_min<A, A> {
			enum { value = true };
		};

		template <typename X, typename Y>
		constexpr static bool check() {
			return is_same_min<X, Y>::value;
		};

		template <typename X, typename Y, typename Z, typename ... K>
		constexpr static bool check() {
			return is_same_min<X, Y>::value and check<Y, Z, K...>();
		};

		enum { value = check<First, Second, Next...>() };
	};
	template<typename T, typename...Args>
	constexpr bool is_same_v = is_same<T, Args...>::value;


	template<class T1, class ...Args>
	struct has_type {
		static constexpr bool value = false;
	};
	template<class T1, class T2, class ...Args>
	struct has_type<T1, T2, Args...> {
		static constexpr bool value = has_type<T1, T2>::value || has_type<T1, Args...>::value;
	};
	template<class T1, class T2>
	struct has_type<T1, T2>
	{
		static constexpr bool value = false;
	};
	template<class T>
	struct has_type<T, T> {
		static constexpr bool value = true;
	};

	template<class T1, class ...Args>
	constexpr bool has_type_v = has_type<T1, Args...>::value;

	template<bool B, class T = void>
	struct enable_if {};

	template<class T>
	struct enable_if<true, T> { typedef T type; };

	template<bool B, class T = void>
	using enable_if_t = typename enable_if<B, T>::type;

	template<class T>
	struct remove_reference { typedef T type; };

	template<class T>
	struct remove_reference<T&> { typedef T type; };

	//去除右值引用
	template<class T>
	struct remove_reference<T&&> { typedef T type; };

	template<class T>
	using remove_reference_t = typename remove_reference<T>::type;

	template <class T>
	inline T* AllocateMemory(size_t size) {
		if (size == 0)return nullptr;
		T* p = (T*)ExAllocatePoolWithTag(NonPagedPool, size*sizeof(T), 'KDM');
		if (p != nullptr)RtlZeroMemory(p, size);
		return p;
	}
	template <class T>
	inline void FreeMemory(T* p) {
		if (MmIsAddressValid((PVOID)p)) ExFreePoolWithTag((PVOID)p, 'KDM');
	}
	template <class T>
	T* ReAllocateMemory(T* p, size_t size) {
		T* pNew = AllocateMemory<T>(size);
		if (pNew != nullptr) {
			RtlZeroMemory(pNew, size);
			memcpy(pNew, p, size);
		}
		FreeMemory(p);
		return pNew;
	}
}