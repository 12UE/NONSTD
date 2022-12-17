#pragma once
namespace nonstd {
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

	template<class T1, class ...Args>struct has_type { static constexpr bool value = false; };
	template<class T1, class T2, class ...Args>//这个是用来判断类型是否存在的
	struct has_type<T1, T2, Args...> { static constexpr bool value = has_type<T1, T2>::value || has_type<T1, Args...>::value; };
	template<class T1, class T2> struct has_type<T1, T2> { static constexpr bool value = false; };
	template<class T>struct has_type<T, T> { static constexpr bool value = true; };
	template<class T1, class...Args>constexpr bool has_type_v = has_type<T1, Args...>::value;
	
	template<bool B, class T = void>struct enable_if {};
	template<class T> struct enable_if<true, T> { typedef T type; };//类似于std::enable_if
	
	template<bool B, class T = void>using enable_if_t = typename enable_if<B, T>::type;
	
	template<class T> struct remove_reference { typedef T type; };
	template<class T> struct remove_reference<T&> { typedef T type; };
	template<class T> struct remove_reference<T&&> { typedef T type; };
	template<class T> using remove_reference_t = typename remove_reference<T>::type;
	
	//template <typename T>
	//T&& forward(nonstd::remove_reference_t<T>& t){
	//	return static_cast<T&&>(t);
	//}
	//
	//template <typename T>
	//T&& forward(nonstd::remove_reference_t<T>&& t){
	//	return static_cast<T&&>(t);
	//}

	template <bool _Test, class _Ty1, class _Ty2>
	struct conditional { // Choose _Ty1 if _Test is true, and _Ty2 otherwise
		using type = _Ty1;
	};
	template <class _Ty1, class _Ty2>
	struct conditional<false, _Ty1, _Ty2> {
		using type = _Ty2;
	};
	template <bool _Test, class _Ty1, class _Ty2>
	using conditional_t = typename conditional<_Test, _Ty1, _Ty2>::type;

	template <class... _Traits>
	_INLINE_VAR constexpr bool disjunction_v = disjunction<_Traits...>::value;

	template <class _Ty, class... _Types>
	_INLINE_VAR constexpr bool _Is_any_of_v = // true if and only if _Ty is in _Types
		disjunction_v<is_same<_Ty, _Types>...>;

	_NODISCARD constexpr bool _Is_constant_evaluated() noexcept { // Internal function for any standard mode
		return __builtin_is_constant_evaluated();
	}

	
	
	template<class T>struct remove_pointer { typedef T type; };
	template<class T>struct remove_pointer<T*> { typedef T type; };
	template<class T>struct remove_pointer<T* const> { typedef T type; };
	template<class T>struct remove_pointer<T* volatile> { typedef T type; };
	template<class T>struct remove_pointer<T* const volatile> { typedef T type; };
	template<class T>using remove_pointer_t = typename remove_pointer<T>::type;	
	
	template<class T>
	void swap(T& a, T& b) {
		T c = a;
		a = b;
		b = c;
	}
	template <typename T> 
	T& static_cast_(void* variable) { return *static_cast<T*>(variable); }
	//
	template<typename T>
	struct remove_cv {
		typedef T type;
	};

	template<typename T>
	struct remove_cv<const T> {
		typedef T type;
	};

	template<typename T>
	struct remove_cv<volatile T> {
		typedef T type;
	};

	template<typename T>
	struct remove_cv<const volatile T> {
		typedef T type;
	};
	template<typename T>
	using remove_cv_t = typename remove_cv<T>::type;
	
	template <class _Ty>
	using _Const_thru_ref = typename remove_reference<_Ty>::_Const_thru_ref_type;

	template <class _Ty>
	using _Remove_cvref_t [[msvc::known_semantics]] = remove_cv_t<remove_reference_t<_Ty>>;


	template <class _Ty>
	using remove_cvref_t = _Remove_cvref_t<_Ty>;

	template <class _Ty>
	struct remove_cvref {
		using type = remove_cvref_t<_Ty>;
	};

	
	template <class _Ty>
	inline constexpr bool is_integral_v = nonstd::has_type_v<remove_cv_t<_Ty>, bool, char, signed char, unsigned char,
		wchar_t,char8_t,char16_t, char32_t, short, unsigned short, int, unsigned int, long, unsigned long, long long, unsigned long long>;

	
	template<typename T>
	struct is_floating_point {
		static constexpr bool value = nonstd::is_same<float, T>::value ||
			nonstd::is_same<double, T>::value ||
			nonstd::is_same<long double, T>::value;
	};
	template<typename T>
	constexpr bool is_floating_point_v = is_floating_point<T>::value;
	
	template<typename T>
	struct is_arithmetic {
		static constexpr bool value = nonstd::is_integral_v<T> ||
			nonstd::is_floating_point<T>::value;
	};
	
	template <class _Ty>
	constexpr bool is_arithmetic_v = nonstd::is_integral_v<_Ty> || nonstd::is_floating_point_v<_Ty>;
	template <class... _Types>
	using void_t = void;

	template <class _Ty>
	struct _Identity {
		using type = _Ty;
	};
	template <class _Ty>
	using _Identity_t [[msvc::known_semantics]] = typename _Identity<_Ty>::type;
	
	template<typename T>
	struct is_void {
		static constexpr bool value = nonstd::is_same<void_t, T>::value;
	};
	
	
	template<typename T>
	constexpr bool is_void_v = is_void<T>::value;
	template<typename T>
	struct is_null_pointer {
		static constexpr bool value = nonstd::is_same<std::nullptr_t, T>::value;
	};
	template<typename T>
	constexpr bool is_null_pointer_v = is_null_pointer<T>::value;
	template<typename T>
	struct is_fundamental {
		static constexpr bool value = nonstd::is_arithmetic<T>::value ||
			nonstd::is_void<T>::value ||
			nonstd::is_null_pointer<T>::value;
	};
	template<typename T>
	constexpr bool is_fundamental_v = is_fundamental<T>::value;
	template<typename T>
	struct is_function {
	private:
		typedef char yes[1];
		typedef char no[2];
	
		template<typename U>
		static yes& test(U(*)[1]);
	
		template<typename U>
		static no& test(...);
	
	public:
		static constexpr bool value = sizeof(test<T>(0)) == sizeof(yes);
	};
	template<typename T>
	constexpr bool is_function_v = is_function<T>::value;
	
	template<typename T>
	struct is_member_pointer {
	private:
		typedef char yes[1];
		typedef char no[2];
	
		template<typename U>
		static yes& test(U*);
	
		template<typename U>
		static no& test(...);
	
	public:
		static constexpr bool value = sizeof(test<T>(0)) == sizeof(yes);
	};
	template<class T>
	constexpr bool is_member_pointer_v = is_member_pointer<T>::value;
	
	template<typename T>
	struct is_member_function_pointer {
		static constexpr bool value = nonstd::is_function<typename nonstd::remove_pointer<T>::type>::value &&
			nonstd::is_member_pointer<typename nonstd::remove_reference<T>::type>::value;
	};
	template<typename T>
	constexpr bool is_member_function_pointer_v = is_member_function_pointer<T>::value;
	
	template<typename T>
	struct is_pointer {
	private:
		typedef char yes[1];
		typedef char no[2];
	
		template<typename U>
		static yes& test(U*);
	
		template<typename U>
		static no& test(...);
	
	public:
		static constexpr bool value = sizeof(test<T>(0)) == sizeof(yes);
	};
	template<class T>
	constexpr bool is_pointer_v = is_pointer<T>::value;
	
	
	template<typename T>
	struct is_reference {
		static constexpr bool value = false;
	};
	template<class T>
	constexpr bool is_reference_v = is_reference<T>::value;
	
	template<typename T>
	struct is_reference<T&> {
		static constexpr bool value = true;
	};
	template<class T>
	constexpr bool is_reference_v<T&> = is_reference<T&>::value;
	
	template<typename T>
	struct is_reference<T&&> {
		static constexpr bool value = true;
	};
	template<class T>
	constexpr bool is_reference_v<T&&> = is_reference<T&&>::value;
	
	template <class _Ty>
	constexpr bool is_enum = bool_constant<__is_enum(_Ty)>::value; // determine whether _Ty is an enumerated type

	template <class _Ty>
	_INLINE_VAR constexpr bool is_enum_v = __is_enum(_Ty);
	
	template <class _Ty, _Ty _Val>
	struct integral_constant {
		static constexpr _Ty value = _Val;

		using value_type = _Ty;
		using type = integral_constant;

		constexpr operator value_type() const noexcept {
			return value;
		}

		_NODISCARD constexpr value_type operator()() const noexcept {
			return value;
		}
	};


	template <bool _Val>
	using bool_constant = integral_constant<bool, _Val>;
	
	template<class T>
	const bool bool_constant_v = bool_constant<T>::value;

	//true_type
	typedef bool_constant<true> true_type;
	//false_type
	typedef bool_constant<false> false_type;
	

	template <class _Ty>
	struct is_pod :nonstd::bool_constant<__is_pod(_Ty)> {}; // determine whether _Ty is a POD type

	template <class _Ty>
	constexpr bool is_pod_v = is_pod<_Ty>::value;

	template<typename T>
	struct is_unsigned {
		static constexpr bool value = false;
	};
	

	template<>
	struct is_unsigned<unsigned char> {
		static constexpr bool value = true;
	};

	template<>
	struct is_unsigned<unsigned short> {
		static constexpr bool value = true;
	};

	template<>
	struct is_unsigned<unsigned int> {
		static constexpr bool value = true;
	};

	template<>
	struct is_unsigned<unsigned long> {
		static constexpr bool value = true;
	};

	template<>
	struct is_unsigned<unsigned long long> {
		static constexpr bool value = true;
	};
	
	template<class T>
	inline constexpr bool is_unsigned_v = nonstd::is_unsigned<T>::value;


	template <typename T>
	struct make_unsigned {
		using type = T;
	};

	template <>
	struct make_unsigned<char> {
		using type = unsigned char;
	};

	template <>
	struct make_unsigned<short> {
		using type = unsigned short;
	};

	template <>
	struct make_unsigned<int> {
		using type = unsigned int;
	};

	template <>
	struct make_unsigned<long> {
		using type = unsigned long;
	};

	template <>
	struct make_unsigned<long long> {
		using type = unsigned long long;
	};
	
	template <typename T>
	using make_unsigned_t = typename make_unsigned<T>::type;

	template <typename T>
	typename T::iterator end(T& container)
	{
		return container.end();
	}
	template<typename T>
	typename T::iterator begin(T& container)
	{
		return container.begin();
	}
	template<typename T>
	typename T::const_iterator end(const T& container)
	{
		return container.end();
	}
	template<typename T>
	typename T::const_iterator begin(const T& container)
	{
		return container.begin();
	}
	
	template <class _Ty, _Ty... _Vals>
	struct integer_sequence { // sequence of integer parameters
		static_assert(is_integral_v<_Ty>, "integer_sequence<T, I...> requires T to be an integral type.");

		using value_type = _Ty;

		_NODISCARD static constexpr size_t size() noexcept {
			return sizeof...(_Vals);
		}
	};

	template <class _Ty, _Ty _Size>
	using make_integer_sequence = __make_integer_seq<integer_sequence, _Ty, _Size>;

	template <size_t... _Vals>
	using index_sequence = integer_sequence<size_t,_Vals...>;

	template <size_t _Size>
	using make_index_sequence = make_integer_sequence<size_t, _Size>;

	template <class... _Types>
	using index_sequence_for = make_index_sequence<sizeof...(_Types)>;


	template <bool _First_value, class _First, class... _Rest>
	struct _Disjunction { // handle true trait or last trait
		using type = _First;
	};

	template <class _False, class _Next, class... _Rest>
	struct _Disjunction<false, _False, _Next, _Rest...> { // first trait is false, try the next trait
		using type = typename _Disjunction<_Next::value, _Next, _Rest...>::type;
	};

	template <class... _Traits>
	struct disjunction : false_type {}; // If _Traits is empty, false_type

	template <class _First, class... _Rest>
	struct disjunction<_First, _Rest...> : _Disjunction<_First::value, _First, _Rest...>::type {
		// the first true trait in _Traits, or the last trait if none are true
	};

	//template <class... _Traits>
	//_INLINE_VAR constexpr bool disjunction_v = disjunction<_Traits...>::value;
	//
	//template <class _Ty, class... _Types>
	//_INLINE_VAR constexpr bool _Is_any_of_v = // true if and only if _Ty is in _Types
	//	disjunction_v<is_same<_Ty, _Types>...>;

	
	template <bool _First_value, class _First, class... _Rest>
	struct _Conjunction { // handle false trait or last trait
		using type = _First;
	};

	

	template <class _True, class _Next, class... _Rest>
	struct _Conjunction<true, _True, _Next, _Rest...> { // the first trait is true, try the next one
		using type = typename _Conjunction<_Next::value, _Next, _Rest...>::type;
	};

	template <class... _Traits>
	struct conjunction : nonstd::true_type{}; // If _Traits is empty, true_type

	template <class _First, class... _Rest>
	struct conjunction<_First, _Rest...> : _Conjunction<_First::value, _First, _Rest...>::type {
		// the first false trait in _Traits, or the last trait if none are false
	};

	template <class... _Traits>
	_INLINE_VAR constexpr bool conjunction_v = conjunction<_Traits...>::value;
	
	template <class _Trait>
	struct negation : bool_constant<!static_cast<bool>(_Trait::value)> {}; // The negated result of _Trait

	template <class _Trait>
	_INLINE_VAR constexpr bool negation_v = negation<_Trait>::value;
	// Type modifiers
	template <class _Ty>
	struct add_const { // add top-level const qualifier
		using type = const _Ty;
	};

	template <class _Ty>
	using add_const_t = typename add_const<_Ty>::type;

	template <class _Ty>
	struct add_volatile { // add top-level volatile qualifier
		using type = volatile _Ty;
	};

	template <class _Ty>
	using add_volatile_t = typename add_volatile<_Ty>::type;

	template <class _Ty>
	struct add_cv { // add top-level const and volatile qualifiers
		using type = const volatile _Ty;
	};

	template <class _Ty>
	using add_cv_t = typename add_cv<_Ty>::type;

	template <class _Ty, class = void>
	struct _Add_reference { // add reference (non-referenceable type)
		using _Lvalue = _Ty;
		using _Rvalue = _Ty;
	};

	template <class _Ty>
	struct _Add_reference<_Ty, void_t<_Ty&>> { // (referenceable type)
		using _Lvalue = _Ty&;
		using _Rvalue = _Ty&&;
	};

	template <class _Ty>
	struct add_lvalue_reference {
		using type = typename _Add_reference<_Ty>::_Lvalue;
	};

	template <class _Ty>
	using add_lvalue_reference_t = typename _Add_reference<_Ty>::_Lvalue;

	template <class _Ty>
	struct add_rvalue_reference {
		using type = typename _Add_reference<_Ty>::_Rvalue;
	};

	template <class _Ty>
	using add_rvalue_reference_t = typename _Add_reference<_Ty>::_Rvalue;
	
	template <class>
	_INLINE_VAR constexpr bool _Always_false = false;

	template <class _Ty>
	add_rvalue_reference_t<_Ty> declval() noexcept {
		static_assert(_Always_false<_Ty>, "Calling declval is ill-formed, see N4892 [declval]/2.");
	}
	template <class _Ty>
	struct remove_extent { // remove array extent
		using type = _Ty;
	};

	template <class _Ty, size_t _Ix>
	struct remove_extent<_Ty[_Ix]> {
		using type = _Ty;
	};

	template <class _Ty>
	struct remove_extent<_Ty[]> {
		using type = _Ty;
	};

	template <class _Ty>
	using remove_extent_t = typename remove_extent<_Ty>::type;

	template <class _Ty>
	struct remove_all_extents { // remove all array extents
		using type = _Ty;
	};

	template <class _Ty, size_t _Ix>
	struct remove_all_extents<_Ty[_Ix]> {
		using type = typename remove_all_extents<_Ty>::type;
	};

	template <class _Ty>
	struct remove_all_extents<_Ty[]> {
		using type = typename remove_all_extents<_Ty>::type;
	};

	template <class _Ty>
	using remove_all_extents_t = typename remove_all_extents<_Ty>::type;

	template <class _Ty, class = void>
	struct _Add_pointer { // add pointer (pointer type cannot be formed)
		using type = _Ty;
	};

	template <class _Ty>
	struct _Add_pointer<_Ty, void_t<remove_reference_t<_Ty>*>> { // (pointer type can be formed)
		using type = remove_reference_t<_Ty>*;
	};


	template <class _Ty>
	struct add_pointer {
		using type = typename _Add_pointer<_Ty>::type;
	};

	template <class _Ty>
	using add_pointer_t = typename _Add_pointer<_Ty>::type;

	template <class>
	_INLINE_VAR constexpr bool is_array_v = false; // determine whether type argument is an array

	template <class _Ty, size_t _Nx>
	_INLINE_VAR constexpr bool is_array_v<_Ty[_Nx]> = true;

	template <class _Ty>
	_INLINE_VAR constexpr bool is_array_v<_Ty[]> = true;

	template <class _Ty>
	struct is_array : bool_constant<is_array_v<_Ty>> {};
	
	template <class>
	inline constexpr bool is_bounded_array_v = false;

	template <class _Ty, size_t _Nx>
	inline constexpr bool is_bounded_array_v<_Ty[_Nx]> = true;

	template <class _Ty>
	struct is_bounded_array : bool_constant<is_bounded_array_v<_Ty>> {};

	template <class>
	inline constexpr bool is_unbounded_array_v = false;

	template <class _Ty>
	inline constexpr bool is_unbounded_array_v<_Ty[]> = true;

	template <class _Ty>
	struct is_unbounded_array : bool_constant<is_unbounded_array_v<_Ty>> {};

	template <class>
	_INLINE_VAR constexpr bool is_lvalue_reference_v = false; // determine whether type argument is an lvalue reference

	template <class _Ty>
	_INLINE_VAR constexpr bool is_lvalue_reference_v<_Ty&> = true;

	template <class _Ty>
	struct is_lvalue_reference : bool_constant<is_lvalue_reference_v<_Ty>> {};

	template <class>
	_INLINE_VAR constexpr bool is_rvalue_reference_v = false; // determine whether type argument is an rvalue reference

	template <class _Ty>
	_INLINE_VAR constexpr bool is_rvalue_reference_v<_Ty&&> = true;

	template <class _Ty>
	struct is_rvalue_reference : bool_constant<is_rvalue_reference_v<_Ty>> {};



	template <class _Ty>
	struct is_union : bool_constant<__is_union(_Ty)> {}; // determine whether _Ty is a union

	template <class _Ty>
	_INLINE_VAR constexpr bool is_union_v = __is_union(_Ty);

	template <class _Ty>
	struct is_class : bool_constant<__is_class(_Ty)> {}; // determine whether _Ty is a class

	template <class _Ty>
	_INLINE_VAR constexpr bool is_class_v = __is_class(_Ty);

	template <class _From, class _To>
	struct is_convertible : bool_constant<__is_convertible_to(_From, _To)> {
		// determine whether _From is convertible to _To
	};
	template <class _From, class _To>
	_INLINE_VAR constexpr bool is_convertible_v = __is_convertible_to(_From, _To);
	
	

	template <class _Ty>
	struct is_compound : bool_constant<!is_fundamental_v<_Ty>> {}; // determine whether _Ty is a compound type

	template <class _Ty>
	_INLINE_VAR constexpr bool is_compound_v = !is_fundamental_v<_Ty>;
	
	template <class _Ty>
	struct _Is_memfunptr { // base class for member function pointer predicates
		using _Bool_type = false_type; // NB: members are user-visible via _Weak_types
	};

	template <class>
	_INLINE_VAR constexpr bool is_const_v = false; // determine whether type argument is const qualified

	template <class _Ty>
	_INLINE_VAR constexpr bool is_const_v<const _Ty> = true;

	template <class _Ty>
	struct is_const : bool_constant<is_const_v<_Ty>> {};

	template <class>
	_INLINE_VAR constexpr bool is_volatile_v = false; // determine whether type argument is volatile qualified

	template <class _Ty>
	_INLINE_VAR constexpr bool is_volatile_v<volatile _Ty> = true;

	template <class _Ty>
	struct is_volatile : bool_constant<is_volatile_v<_Ty>> {};

	template <class _Ty>
	_INLINE_VAR constexpr bool is_object_v = // only function types and reference types can't be const qualified
		is_const_v<const _Ty> && !is_void_v<_Ty>;

	template <class _Ty>
	struct is_object : bool_constant<is_object_v<_Ty>> {};

	template <class>
	struct _Is_member_object_pointer {
		static constexpr bool value = false;
	};

	template <class _Ty1, class _Ty2>
	struct _Is_member_object_pointer<_Ty1 _Ty2::*> {
		static constexpr bool value = !is_function_v<_Ty1>;
		using _Class_type = _Ty2;
	};

	template <class _Ty>
	_INLINE_VAR constexpr bool is_member_object_pointer_v = _Is_member_object_pointer<remove_cv_t<_Ty>>::value;

	template <class _Ty>
	struct is_member_object_pointer : bool_constant<is_member_object_pointer_v<_Ty>> {};

	template <class _Ty>
	_INLINE_VAR constexpr bool is_scalar_v = // determine whether _Ty is a scalar type
		is_arithmetic_v<_Ty> || is_enum_v<_Ty> || is_pointer_v<_Ty> || is_member_pointer_v<_Ty> || is_null_pointer_v<_Ty>;

	template <class _Ty>
	struct is_scalar : bool_constant<is_scalar_v<_Ty>> {};

	template <class _Ty>
	struct is_empty : bool_constant<__is_empty(_Ty)> {}; // determine whether _Ty is an empty class

	template <class _Ty>
	_INLINE_VAR constexpr bool is_empty_v = __is_empty(_Ty);

	template <class _Ty>
	struct is_polymorphic : bool_constant<__is_polymorphic(_Ty)> {}; // determine whether _Ty is a polymorphic type

	template <class _Ty>
	_INLINE_VAR constexpr bool is_polymorphic_v = __is_polymorphic(_Ty);

	template <class _Ty>
	struct is_abstract : bool_constant<__is_abstract(_Ty)> {}; // determine whether _Ty is an abstract class

	template <class _Ty>
	_INLINE_VAR constexpr bool is_abstract_v = __is_abstract(_Ty);

	template <class _Ty>
	struct is_final : bool_constant<__is_final(_Ty)> {}; // determine whether _Ty is a final class

	template <class _Ty>
	_INLINE_VAR constexpr bool is_final_v = __is_final(_Ty);

	template <class _Ty>
	struct is_standard_layout : bool_constant<__is_standard_layout(_Ty)> {}; // determine whether _Ty is standard layout

	template <class _Ty>
	_INLINE_VAR constexpr bool is_standard_layout_v = __is_standard_layout(_Ty);
	
	template <class _Ty>
	struct is_literal_type : bool_constant<__is_literal_type(_Ty)> {
		// determine whether _Ty is a literal type
	};
	template <class _Ty>
	_INLINE_VAR constexpr bool is_literal_type_v = __is_literal_type(_Ty);

	template <class _Ty>
	struct is_trivial : bool_constant<__is_trivially_constructible(_Ty) && __is_trivially_copyable(_Ty)> {
		// determine whether _Ty is a trivial type
	};

	template <class _Ty>
	_INLINE_VAR constexpr bool is_trivial_v = __is_trivially_constructible(_Ty) && __is_trivially_copyable(_Ty);
	
	template <class _Ty>
	struct is_trivially_copyable : bool_constant<__is_trivially_copyable(_Ty)> {
		// determine whether _Ty is a trivially copyable type
	};
	
	

	template <class _Ty>
	_INLINE_VAR constexpr bool is_trivially_copyable_v = __is_trivially_copyable(_Ty);

	template <class _Ty>
	struct has_virtual_destructor : bool_constant<__has_virtual_destructor(_Ty)> {
		// determine whether _Ty has a virtual destructor
	};

	template <class _Ty>
	_INLINE_VAR constexpr bool has_virtual_destructor_v = __has_virtual_destructor(_Ty);
	template <class _Ty, class... _Args>
	struct is_constructible : bool_constant<__is_constructible(_Ty, _Args...)> {
		// determine whether _Ty can be direct-initialized with _Args...
	};

	template <class _Ty, class... _Args>
	_INLINE_VAR constexpr bool is_constructible_v = __is_constructible(_Ty, _Args...);

	template <class _Ty>
	struct is_copy_constructible : bool_constant<__is_constructible(_Ty, add_lvalue_reference_t<const _Ty>)> {
		// determine whether _Ty can be direct-initialized with an lvalue const _Ty
	};

	template <class _Ty>
	_INLINE_VAR constexpr bool is_copy_constructible_v = __is_constructible(_Ty, add_lvalue_reference_t<const _Ty>);

	template <class _Ty>
	struct is_default_constructible : bool_constant<__is_constructible(_Ty)> {
		// determine whether _Ty can be value-initialized
	};

	template <class _Ty>
	_INLINE_VAR constexpr bool is_default_constructible_v = __is_constructible(_Ty);

	template <class _Ty, class = void>
	struct _Is_implicitly_default_constructible : false_type {
		// determine whether _Ty can be copy-initialized with {}
	};

	template <class _Ty>
	void _Implicitly_default_construct(const _Ty&);

	template <class _Ty>
	struct _Is_implicitly_default_constructible < _Ty, void_t<decltype(_Implicitly_default_construct<_Ty>({})) >> : true_type {
	};

	template <class _Ty>
	struct is_move_constructible : bool_constant<__is_constructible(_Ty, _Ty)> {
		// determine whether _Ty can be direct-initialized from an rvalue _Ty
	};

	template <class _Ty>
	_INLINE_VAR constexpr bool is_move_constructible_v = __is_constructible(_Ty, _Ty);

	template <class _To, class _From>
	struct is_assignable : bool_constant<__is_assignable(_To, _From)> {}; // determine whether _From can be assigned to _To

	template <class _To, class _From>
	_INLINE_VAR constexpr bool is_assignable_v = __is_assignable(_To, _From);
	
	template <class _To, class _From>
	using _Is_assignable_no_precondition_check = is_assignable<_To, _From>;

	template <class _Ty>
	struct is_copy_assignable
		: bool_constant<__is_assignable(add_lvalue_reference_t<_Ty>, add_lvalue_reference_t<const _Ty>)> {
		// determine whether an lvalue const _Ty can be assigned to an lvalue _Ty
	};
	
	template <class _Ty>
	_INLINE_VAR constexpr bool is_copy_assignable_v = __is_assignable(
		add_lvalue_reference_t<_Ty>, add_lvalue_reference_t<const _Ty>);

	template <class _Ty>
	using _Is_copy_assignable_no_precondition_check = is_copy_assignable<_Ty>;

	template <class _Ty>
	_INLINE_VAR constexpr bool _Is_copy_assignable_unchecked_v = is_copy_assignable_v<_Ty>;

	template <class _Ty>
	struct is_move_assignable : bool_constant<__is_assignable(add_lvalue_reference_t<_Ty>, _Ty)> {
		// determine whether an rvalue _Ty can be assigned to an lvalue _Ty
	};

	template <class _Ty>
	_INLINE_VAR constexpr bool is_move_assignable_v = __is_assignable(add_lvalue_reference_t<_Ty>, _Ty);
	
	template <class _Ty>
	using _Is_move_assignable_no_precondition_check = is_move_assignable<_Ty>;

	template <class _Ty>
	_INLINE_VAR constexpr bool _Is_move_assignable_unchecked_v = is_move_assignable_v<_Ty>;
	template <class _Ty>
	struct is_destructible : bool_constant<__is_destructible(_Ty)> {
		// true iff remove_all_extents_t<_Ty> is a reference type, or can be explicitly destroyed
	};

	template <class _Ty>
	_INLINE_VAR constexpr bool is_destructible_v = __is_destructible(_Ty);

	template <class _Ty, class... _Args>
	struct is_trivially_constructible : bool_constant<__is_trivially_constructible(_Ty, _Args...)> {
		// determine whether direct-initialization of _Ty with _Args... is trivial
	};

	template <class _Ty, class... _Args>
	_INLINE_VAR constexpr bool is_trivially_constructible_v = __is_trivially_constructible(_Ty, _Args...);

	template <class _Ty>
	struct is_trivially_copy_constructible
		: bool_constant<__is_trivially_constructible(_Ty, add_lvalue_reference_t<const _Ty>)> {
		// determine whether direct-initialization of _Ty with an lvalue const _Ty is trivial
	};

	template <class _Ty>
	_INLINE_VAR constexpr bool is_trivially_copy_constructible_v = __is_trivially_constructible(
		_Ty, add_lvalue_reference_t<const _Ty>);

	template <class _Ty>
	struct is_trivially_default_constructible : bool_constant<__is_trivially_constructible(_Ty)> {
		// determine whether value-initialization of _Ty is trivial
	};

	template <class _Ty>
	_INLINE_VAR constexpr bool is_trivially_default_constructible_v = __is_trivially_constructible(_Ty);

	template <class _Ty>
	struct is_trivially_move_constructible : bool_constant<__is_trivially_constructible(_Ty, _Ty)> {
		// determine whether direct-initialization of _Ty with an rvalue _Ty is trivial
	};

	template <class _Ty>
	_INLINE_VAR constexpr bool is_trivially_move_constructible_v = __is_trivially_constructible(_Ty, _Ty);

	template <class _To, class _From>
	struct is_trivially_assignable : bool_constant<__is_trivially_assignable(_To, _From)> {
		// determine whether _From can be trivially assigned to _To
	};

	template <class _To, class _From>
	_INLINE_VAR constexpr bool is_trivially_assignable_v = __is_trivially_assignable(_To, _From);

	template <class _Ty>
	struct is_trivially_copy_assignable
		: bool_constant<__is_trivially_assignable(add_lvalue_reference_t<_Ty>, add_lvalue_reference_t<const _Ty>)> {
		// determine whether an lvalue const _Ty can be trivially assigned to an lvalue _Ty
	};

	template <class _Ty>
	_INLINE_VAR constexpr bool is_trivially_copy_assignable_v = __is_trivially_assignable(
		add_lvalue_reference_t<_Ty>, add_lvalue_reference_t<const _Ty>);

	template <class _Ty>
	struct is_trivially_move_assignable : bool_constant<__is_trivially_assignable(add_lvalue_reference_t<_Ty>, _Ty)> {
		// determine whether an rvalue _Ty can be trivially assigned to an lvalue _Ty
	};

	template <class _Ty>
	_INLINE_VAR constexpr bool is_trivially_move_assignable_v = __is_trivially_assignable(add_lvalue_reference_t<_Ty>, _Ty);

	template <class _Ty>
	struct is_trivially_destructible : bool_constant<__is_trivially_destructible(_Ty)> {
		// determine whether remove_all_extents_t<_Ty> is a reference type or can trivially be explicitly destroyed
	};

	template <class _Ty>
	_INLINE_VAR constexpr bool is_trivially_destructible_v = __is_trivially_destructible(_Ty);

	template <class _Ty, class... _Args>
	struct is_nothrow_constructible : bool_constant<__is_nothrow_constructible(_Ty, _Args...)> {
		// determine whether direct-initialization of _Ty from _Args... is both valid and not potentially-throwing
	};

	template <class _Ty, class... _Args>
	_INLINE_VAR constexpr bool is_nothrow_constructible_v = __is_nothrow_constructible(_Ty, _Args...);

	template <class _Ty>
	struct is_nothrow_copy_constructible
		: bool_constant<__is_nothrow_constructible(_Ty, add_lvalue_reference_t<const _Ty>)> {
		// determine whether direct-initialization of _Ty from an lvalue const _Ty is both valid
		// and not potentially-throwing
	};

	template <class _Ty>
	_INLINE_VAR constexpr bool is_nothrow_copy_constructible_v = __is_nothrow_constructible(
		_Ty, add_lvalue_reference_t<const _Ty>);

	template <class _Ty>
	struct is_nothrow_default_constructible : bool_constant<__is_nothrow_constructible(_Ty)> {
		// determine whether value-initialization of _Ty is both valid and not potentially-throwing
	};

	template <class _Ty>
	_INLINE_VAR constexpr bool is_nothrow_default_constructible_v = __is_nothrow_constructible(_Ty);

	template <class _Ty>
	struct is_nothrow_move_constructible : bool_constant<__is_nothrow_constructible(_Ty, _Ty)> {
		// determine whether direct-initialization of _Ty from an rvalue _Ty is both valid and not potentially-throwing
	};

	template <class _Ty>
	_INLINE_VAR constexpr bool is_nothrow_move_constructible_v = __is_nothrow_constructible(_Ty, _Ty);

	template <class _To, class _From>
	struct is_nothrow_assignable : bool_constant<__is_nothrow_assignable(_To, _From)> {
		// determine whether assignment of _From to _To is both valid and not potentially-throwing
	};

	template <class _To, class _From>
	_INLINE_VAR constexpr bool is_nothrow_assignable_v = __is_nothrow_assignable(_To, _From);

	template <class _Ty>
	struct is_nothrow_copy_assignable
		: bool_constant<__is_nothrow_assignable(add_lvalue_reference_t<_Ty>, add_lvalue_reference_t<const _Ty>)> {
		// determine whether assignment of an lvalue const _Ty to an lvalue _Ty is both valid and not potentially-throwing
	};

	template <class _Ty>
	_INLINE_VAR constexpr bool is_nothrow_copy_assignable_v = __is_nothrow_assignable(
		add_lvalue_reference_t<_Ty>, add_lvalue_reference_t<const _Ty>);

	template <class _Ty>
	struct is_nothrow_move_assignable : bool_constant<__is_nothrow_assignable(add_lvalue_reference_t<_Ty>, _Ty)> {
		// determine whether assignment of an rvalue _Ty to an lvalue _Ty is both valid and not potentially-throwing
	};

	template <class _Ty>
	_INLINE_VAR constexpr bool is_nothrow_move_assignable_v = __is_nothrow_assignable(add_lvalue_reference_t<_Ty>, _Ty);

	template <class _Ty>
	struct is_nothrow_destructible : bool_constant<__is_nothrow_destructible(_Ty)> {
		// determine whether remove_all_extents_t<_Ty> is a reference type or has
		// non-potentially-throwing explicit destruction
	};

	template <class _Ty>
	_INLINE_VAR constexpr bool is_nothrow_destructible_v = __is_nothrow_destructible(_Ty);

	template <class _Ty, bool = is_integral_v<_Ty>>
	struct _Sign_base { // determine whether integral type _Ty is signed or unsigned
		using _Uty = remove_cv_t<_Ty>;

		static constexpr bool _Signed = static_cast<_Uty>(-1) < static_cast<_Uty>(0);
		static constexpr bool _Unsigned = !_Signed;
	};

	template <class _Ty>
	struct _Sign_base<_Ty, false> { // floating-point _Ty is signed
		// non-arithmetic _Ty is neither signed nor unsigned
		static constexpr bool _Signed = is_floating_point_v<_Ty>;
		static constexpr bool _Unsigned = false;
	};

	template <class _Ty>
	struct is_signed : bool_constant<_Sign_base<_Ty>::_Signed> {}; // determine whether _Ty is a signed type

	template <class _Ty>
	_INLINE_VAR constexpr bool is_signed_v = _Sign_base<_Ty>::_Signed;


	template <class _Ty>
	_INLINE_VAR constexpr bool _Is_nonbool_integral = is_integral_v<_Ty> && !is_same_v<remove_cv_t<_Ty>, bool>;

	template <bool>
	struct _Select { // Select between aliases that extract either their first or second parameter
		template <class _Ty1, class>
		using _Apply = _Ty1;
	};

	template <>
	struct _Select<false> {
		template <class, class _Ty2>
		using _Apply = _Ty2;
	};

	template <size_t>
	struct _Make_signed2; // Choose make_signed strategy by type size

	template <>
	struct _Make_signed2<1> {
		template <class>
		using _Apply = signed char;
	};

	template <>
	struct _Make_signed2<2> {
		template <class>
		using _Apply = short;
	};

	template <>
	struct _Make_signed2<4> {
		template <class _Ty>
		using _Apply = // assumes LLP64
			typename _Select<is_same_v<_Ty, long> || is_same_v<_Ty, unsigned long>>::template _Apply<long, int>;
	};

	template <>
	struct _Make_signed2<8> {
		template <class>
		using _Apply = long long;
	};

	template <class _Ty>
	using _Make_signed1 = // signed partner to cv-unqualified _Ty
		typename _Make_signed2<sizeof(_Ty)>::template _Apply<_Ty>;

	template <class _Ty>
	struct make_signed { // signed partner to _Ty
		static_assert(_Is_nonbool_integral<_Ty> || is_enum_v<_Ty>,
			"make_signed<T> requires that T shall be a (possibly cv-qualified) "
			"integral type or enumeration but not a bool type.");

		using type = typename remove_cv<_Ty>::template _Apply<_Make_signed1>;
	};

	template <class _Ty>
	using make_signed_t = typename make_signed<_Ty>::type;

	template <size_t>
	struct _Make_unsigned2; // Choose make_unsigned strategy by type size

	template <>
	struct _Make_unsigned2<1> {
		template <class>
		using _Apply = unsigned char;
	};

	template <>
	struct _Make_unsigned2<2> {
		template <class>
		using _Apply = unsigned short;
	};

	template <>
	struct _Make_unsigned2<4> {
		template <class _Ty>
		using _Apply = // assumes LLP64
			typename _Select<is_same_v<_Ty, long> || is_same_v<_Ty, unsigned long>>::template _Apply<unsigned long,
			unsigned int>;
	};

	template <>
	struct _Make_unsigned2<8> {
		template <class>
		using _Apply = unsigned long long;
	};

	template <class _Ty>
	using _Make_unsigned1 = // unsigned partner to cv-unqualified _Ty
		typename _Make_unsigned2<sizeof(_Ty)>::template _Apply<_Ty>;
	


	template <class _Ty>
	using make_unsigned_t = typename make_unsigned<_Ty>::type;

	template <class _Rep>
	constexpr make_unsigned_t<_Rep> _Unsigned_value(_Rep _Val) { // makes _Val unsigned
		return static_cast<make_unsigned_t<_Rep>>(_Val);
	}

	template <class _Ty>
	struct alignment_of : integral_constant<size_t, alignof(_Ty)> {}; // determine alignment of _Ty

	template <class _Ty>
	_INLINE_VAR constexpr size_t alignment_of_v = alignof(_Ty);

	template <class _Ty, size_t _Len>
	union _Align_type { // union with size _Len bytes and alignment of _Ty
		_Ty _Val;
		char _Pad[_Len];
	};

	template <size_t _Len, size_t _Align, class _Ty, bool _Ok>
	struct _Aligned; // define type with size _Len and alignment _Ty

	template <size_t _Len, size_t _Align, class _Ty>
	struct _Aligned<_Len, _Align, _Ty, true> {
		using type = _Align_type<_Ty, _Len>;
	};

	template <size_t _Len, size_t _Align>
	struct _Aligned<_Len, _Align, double, false> {
		static_assert(_Always_false<_Aligned>,
			"You've instantiated std::aligned_storage<Len, Align> with an extended alignment (in other "
			"words, Align > alignof(max_align_t)). Before VS 2017 15.8, the member \"type\" would "
			"non-conformingly have an alignment of only alignof(max_align_t). VS 2017 15.8 was fixed to "
			"handle this correctly, but the fix inherently changes layout and breaks binary compatibility "
			"(*only* for uses of aligned_storage with extended alignments). "
			"To suppress this error, please define either "
			"(1) _ENABLE_EXTENDED_ALIGNED_STORAGE to confirm that you want a type with an extended alignment, or "
			"(2) _DISABLE_EXTENDED_ALIGNED_STORAGE to get the old non-conforming behavior.");
		using type = _Align_type<max_align_t, _Len>;

	};

	template <size_t _Len, size_t _Align>
	struct _Aligned<_Len, _Align, int, false> {
		using _Next = double;
		static constexpr bool _Fits = _Align <= alignof(_Next);
		using type = typename _Aligned<_Len, _Align, _Next, _Fits>::type;
	};

	template <size_t _Len, size_t _Align>
	struct _Aligned<_Len, _Align, short, false> {
		using _Next = int;
		static constexpr bool _Fits = _Align <= alignof(_Next);
		using type = typename _Aligned<_Len, _Align, _Next, _Fits>::type;
	};

	template <size_t _Len, size_t _Align>
	struct _Aligned<_Len, _Align, char, false> {
		using _Next = short;
		static constexpr bool _Fits = _Align <= alignof(_Next);
		using type = typename _Aligned<_Len, _Align, _Next, _Fits>::type;
	};

	// TRANSITION, ABI: Internal non-deprecated version to avoid ABI changes due to deprecation
	template <size_t _Len, size_t _Align = alignof(max_align_t)>
	struct _Aligned_storage { // define type with size _Len and alignment _Align
		using _Next = char;
		static constexpr bool _Fits = _Align <= alignof(_Next);
		using type = typename _Aligned<_Len, _Align, _Next, _Fits>::type;
	};

	template <size_t _Len, size_t _Align = alignof(max_align_t)>
	using _Aligned_storage_t = typename _Aligned_storage<_Len, _Align>::type;

	template <size_t _Len, size_t _Align = alignof(max_align_t)>
	struct aligned_storage { // define type with size _Len and alignment _Align
		using type = _Aligned_storage_t<_Len, _Align>;
	};

	template <size_t _Len, size_t _Align = alignof(max_align_t)>
	using aligned_storage_t  = _Aligned_storage_t<_Len, _Align>;

	template <size_t... _Vals>
	struct _Maximum;
	
	template <>
	struct _Maximum<> : integral_constant<size_t, 0> {}; // maximum of nothing is 0

	template <size_t _Val>
	struct _Maximum<_Val> : integral_constant<size_t, _Val> {}; // maximum of _Val is _Val

	template <size_t _First, size_t _Second, size_t... _Rest>
	struct _Maximum<_First, _Second, _Rest...> : _Maximum<(_First < _Second ? _Second : _First), _Rest...>::type{
		// find maximum value in _First, _Second, _Rest...
	};

	template <size_t _Len, class... _Types>
	struct aligned_union {
		// define type with size at least _Len, for storing anything in _Types
		static constexpr size_t _Max_len = _Maximum<_Len, sizeof(_Types)...>::value; // NOT sizeof...(_Types)
		static constexpr size_t alignment_value = _Maximum<alignof(_Types)...>::value;

		using type = _Aligned_storage_t<_Max_len, alignment_value>;
	};


		template <size_t _Len, class... _Types>
	using aligned_union_t  = typename aligned_union<_Len, _Types...>::type;
	

		template <class _Ty, bool = is_enum_v<_Ty>>
	struct _Underlying_type {
		using type = __underlying_type(_Ty);
	};

	template <class _Ty>
	struct _Underlying_type<_Ty, false> {};

	template <class _Ty>
	struct underlying_type : _Underlying_type<_Ty> {}; // determine underlying type for enum

	template <class _Ty>
	using underlying_type_t = typename _Underlying_type<_Ty>::type;

	template <class _Ty>
	_INLINE_VAR constexpr size_t rank_v = 0; // determine number of dimensions of array _Ty

	template <class _Ty, size_t _Nx>
	_INLINE_VAR constexpr size_t rank_v<_Ty[_Nx]> = rank_v<_Ty> +1;

	template <class _Ty>
	_INLINE_VAR constexpr size_t rank_v<_Ty[]> = rank_v<_Ty> +1;

	template <class _Ty>
	struct rank : integral_constant<size_t, rank_v<_Ty>> {};

	template <class _Ty, unsigned int _Ix = 0>
	_INLINE_VAR constexpr size_t extent_v = 0; // determine extent of dimension _Ix of array _Ty

	template <class _Ty, size_t _Nx>
	_INLINE_VAR constexpr size_t extent_v<_Ty[_Nx], 0> = _Nx;

	template <class _Ty, unsigned int _Ix, size_t _Nx>
	_INLINE_VAR constexpr size_t extent_v<_Ty[_Nx], _Ix> = extent_v<_Ty, _Ix - 1>;

	template <class _Ty, unsigned int _Ix>
	_INLINE_VAR constexpr size_t extent_v<_Ty[], _Ix> = extent_v<_Ty, _Ix - 1>;

	template <class _Ty, unsigned int _Ix = 0>
	struct extent : integral_constant<size_t, extent_v<_Ty, _Ix>> {};

	template <class _Base, class _Derived>
	struct is_base_of : bool_constant<__is_base_of(_Base, _Derived)> {
		// determine whether _Base is a base of or the same as _Derived
	};

	template <class _Base, class _Derived>
	_INLINE_VAR constexpr bool is_base_of_v = __is_base_of(_Base, _Derived);

	template <class _Ty>
	struct decay { // determines decayed version of _Ty
		using _Ty1 = remove_reference_t<_Ty>;
		using _Ty2 = typename _Select<is_function_v<_Ty1>>::template _Apply<add_pointer<_Ty1>, remove_cv<_Ty1>>;
		using type = typename _Select<is_array_v<_Ty1>>::template _Apply<add_pointer<remove_extent_t<_Ty1>>, _Ty2>::type;
	};

	template <class _Ty>
	using decay_t = typename nonstd::decay<_Ty>::type;

	template <class _Ty1, class _Ty2>
	using _Conditional_type = decltype(false ? nonstd:: declval<_Ty1>() : nonstd:: declval<_Ty2>());

	template <class _Ty1, class _Ty2, class = void>
	struct _Const_lvalue_cond_oper {};

	// N4810 [meta.trans.other]/3.3.4 (per the proposed resolution of LWG-3205): "Otherwise, if remove_cvref_t</**/> denotes
	// a type..."
	template <class _Ty1, class _Ty2>
	struct _Const_lvalue_cond_oper<_Ty1, _Ty2, nonstd::void_t<_Conditional_type<const _Ty1&, const _Ty2&>>> {
		using type = remove_cvref_t<_Conditional_type<const _Ty1&, const _Ty2&>>;
	};

	template <class _Ty1, class _Ty2, class = void>
	struct _Decayed_cond_oper : _Const_lvalue_cond_oper<_Ty1, _Ty2> {};


	template <class _Ty1, class _Ty2>
	struct _Decayed_cond_oper<_Ty1, _Ty2, nonstd::void_t<_Conditional_type<_Ty1, _Ty2>>> {
		using type = decay_t<_Conditional_type<_Ty1, _Ty2>>;
	};

	template <class... _Ty>
	struct common_type;

	template <class... _Ty>
	using common_type_t = typename common_type<_Ty...>::type;

	template <>
	struct common_type<> {};

	template <class _Ty1>
	struct common_type<_Ty1> : common_type<_Ty1, _Ty1> {};

	template <class _Ty1, class _Ty2, class _Decayed1 = decay_t<_Ty1>, class _Decayed2 = decay_t<_Ty2>>
	struct _Common_type2 : common_type<_Decayed1, _Decayed2> {};

	template <class _Ty1, class _Ty2>
	struct _Common_type2<_Ty1, _Ty2, _Ty1, _Ty2> : _Decayed_cond_oper<_Ty1, _Ty2> {};

	template <class _Ty1, class _Ty2>
	struct common_type<_Ty1, _Ty2> : _Common_type2<_Ty1, _Ty2> {};

	template <class _Void, class _Ty1, class _Ty2, class... _Rest>
	struct _Common_type3 {};

	template <class _Ty1, class _Ty2, class... _Rest>
	struct _Common_type3<void_t<common_type_t<_Ty1, _Ty2>>, _Ty1, _Ty2, _Rest...>
		: common_type<common_type_t<_Ty1, _Ty2>, _Rest...> {};

	template <class _Ty1, class _Ty2, class... _Rest>
	struct common_type<_Ty1, _Ty2, _Rest...> : _Common_type3<void, _Ty1, _Ty2, _Rest...> {};

	template <class _Ty>
	_Ty _Returns_exactly() noexcept; // not defined

	template <class, class, template <class> class, template <class> class>
	struct basic_common_reference {};

	template <class _From>
	struct _Copy_cv_impl {
		template <class _To>
		using _Apply = _To;
	};
	template <class _From>
	struct _Copy_cv_impl<const _From> {
		template <class _To>
		using _Apply = const _To;
	};
	template <class _From>
	struct _Copy_cv_impl<volatile _From> {
		template <class _To>
		using _Apply = volatile _To;
	};
	template <class _From>
	struct _Copy_cv_impl<const volatile _From> {
		template <class _To>
		using _Apply = const volatile _To;
	};
	template <class _From, class _To>
	using _Copy_cv = // N4810 [meta.trans.other]/2.3
		typename _Copy_cv_impl<_From>::template _Apply<_To>;

	template <class _Ty1>
	struct _Add_qualifiers { // _Add_qualifiers<A>::template _Apply is XREF(A) from N4810 [meta.trans.other]/2.1
		template <class _Ty2>
		using _Apply = _Copy_cv<_Ty1, _Ty2>;
	};
	template <class _Ty1>
	struct _Add_qualifiers<_Ty1&> {
		template <class _Ty2>
		using _Apply = add_lvalue_reference_t<_Copy_cv<_Ty1, _Ty2>>;
	};
	template <class _Ty1>
	struct _Add_qualifiers<_Ty1&&> {
		template <class _Ty2>
		using _Apply = add_rvalue_reference_t<_Copy_cv<_Ty1, _Ty2>>;
	};
	template <class _Ty1, class _Ty2>
	using _Cond_res = // N4810 [meta.trans.other]/2.4
		decltype(false ? _Returns_exactly<_Ty1>() : _Returns_exactly<_Ty2>());


	template <class...>
	struct common_reference;

	template <class... _Types>
	using common_reference_t = typename common_reference<_Types...>::type;

	// N4810 [meta.trans.other]/5.1: "If sizeof...(T) is zero ..."
	template <>
	struct common_reference<> {};

	// N4810 [meta.trans.other]/5.2: "...if sizeof...(T) is one ..."
	template <class _Ty>
	struct common_reference<_Ty> {
		using type = _Ty;
	};

	// N4810 [meta.trans.other]/5.3: "...if sizeof...(T) is two..."

	// N4810 [meta.trans.other]/5.3.4: "if common_type_t<T1, T2> is well-formed..."
	// N4810 [meta.trans.other]/5.3.5: "Otherwise, there shall be no member type."
	template <class _Ty1, class _Ty2, class = void>
	struct _Common_reference2C : common_type<_Ty1, _Ty2> {};

	// N4810 [meta.trans.other]/5.3.3: "if COND_RES(T1, T2) is well-formed..."
	template <class _Ty1, class _Ty2>
	struct _Common_reference2C<_Ty1, _Ty2, void_t<_Cond_res<_Ty1, _Ty2>>> {
		using type = _Cond_res<_Ty1, _Ty2>;
	};

	// N4810 [meta.trans.other]/5.3.2: "if basic_common_reference<[...]>::type is well-formed..."
	template <class _Ty1, class _Ty2>
	using _Basic_specialization = typename basic_common_reference<remove_cvref_t<_Ty1>, remove_cvref_t<_Ty2>,
		_Add_qualifiers<_Ty1>::template _Apply, _Add_qualifiers<_Ty2>::template _Apply>::type;

	template <class _Ty1, class _Ty2, class = void>
	struct _Common_reference2B : _Common_reference2C<_Ty1, _Ty2> {};

	template <class _Ty1, class _Ty2>
	struct _Common_reference2B<_Ty1, _Ty2, void_t<_Basic_specialization<_Ty1, _Ty2>>> {
		using type = _Basic_specialization<_Ty1, _Ty2>;
	};

	// N4810 [meta.trans.other]/5.3.1: "If T1 and T2 are reference types and COMMON_REF(T1, T2) is well-formed..."
	template <class _Ty1, class _Ty2, class = void>
	struct _Common_reference2A : _Common_reference2B<_Ty1, _Ty2> {};

	template <class _Ty1, class _Ty2, class _Result = _Cond_res<_Copy_cv<_Ty1, _Ty2>&, _Copy_cv<_Ty2, _Ty1>&>,
		enable_if_t<is_lvalue_reference_v<_Result>, int> = 0>
	using _LL_common_ref = _Result;

	template <class _Ty1, class _Ty2>
	struct _Common_reference2A<_Ty1&, _Ty2&, void_t<_LL_common_ref<_Ty1, _Ty2>>> {
		using type = _LL_common_ref<_Ty1, _Ty2>; // "both lvalues" case from N4810 [meta.trans.other]/2.5
	};

	template <class _Ty1, class _Ty2>
	struct _Common_reference2A<_Ty1&&, _Ty2&, enable_if_t<is_convertible_v<_Ty1&&, _LL_common_ref<const _Ty1, _Ty2>>>> {
		using type = _LL_common_ref<const _Ty1, _Ty2>; // "rvalue and lvalue" case from N4810 [meta.trans.other]/2.7
	};

	template <class _Ty1, class _Ty2>
	struct _Common_reference2A<_Ty1&, _Ty2&&, enable_if_t<is_convertible_v<_Ty2&&, _LL_common_ref<const _Ty2, _Ty1>>>> {
		using type = _LL_common_ref<const _Ty2, _Ty1>; // "lvalue and rvalue" case from N4810 [meta.trans.other]/2.8
	};

	template <class _Ty1, class _Ty2>
	using _RR_common_ref = remove_reference_t<_LL_common_ref<_Ty1, _Ty2>>&&;

	template <class _Ty1, class _Ty2>
	struct _Common_reference2A<_Ty1&&, _Ty2&&,
		enable_if_t<
		is_convertible_v<_Ty1&&, _RR_common_ref<_Ty1, _Ty2>>&& is_convertible_v<_Ty2&&, _RR_common_ref<_Ty1, _Ty2>>>> {
		using type = _RR_common_ref<_Ty1, _Ty2>; // "both rvalues" case from N4810 [meta.trans.other]/2.6
	};

	template <class _Ty1, class _Ty2>
	struct common_reference<_Ty1, _Ty2> : _Common_reference2A<_Ty1, _Ty2> {};

	// N4810 [meta.trans.other]/5.4: "if sizeof...(T) is greater than two..."
	template <class _Void, class _Ty1, class _Ty2, class... _Types>
	struct _Fold_common_reference {};
	template <class _Ty1, class _Ty2, class... _Types>
	struct _Fold_common_reference<void_t<common_reference_t<_Ty1, _Ty2>>, _Ty1, _Ty2, _Types...>
		: common_reference<common_reference_t<_Ty1, _Ty2>, _Types...> {};

	template <class _Ty1, class _Ty2, class _Ty3, class... _Rest>
	struct common_reference<_Ty1, _Ty2, _Ty3, _Rest...> : _Fold_common_reference<void, _Ty1, _Ty2, _Ty3, _Rest...> {};
#if _HAS_CXX20
	template <class _Ty>
	struct type_identity {
		using type = _Ty;
	};
	template <class _Ty>
	using type_identity_t = typename type_identity<_Ty>::type;
#endif // _HAS_CXX20

	template <class _Type, template <class...> class _Template>
	_INLINE_VAR constexpr bool _Is_specialization_v = false; // true if and only if _Type is a specialization of _Template
	template <template <class...> class _Template, class... _Types>
	_INLINE_VAR constexpr bool _Is_specialization_v<_Template<_Types...>, _Template> = true;

	template <class _Type, template <class...> class _Template>
	struct _Is_specialization : bool_constant<_Is_specialization_v<_Type, _Template>> {};

	template <class _Ty>
	_NODISCARD constexpr _Ty&& forward(
		nonstd::remove_reference_t<_Ty>&_Arg) noexcept { // forward an lvalue as either an lvalue or an rvalue
		return static_cast<_Ty&&>(_Arg);
	}

	template <class _Ty>
	_NODISCARD constexpr _Ty&& forward(nonstd::remove_reference_t<_Ty>&& _Arg) noexcept { // forward an rvalue as an rvalue
		static_assert(!is_lvalue_reference_v<_Ty>, "bad forward call");
		return static_cast<_Ty&&>(_Arg);
	}


	template <class _Ty>
	_NODISCARD constexpr remove_reference_t<_Ty>&& move(_Ty&& _Arg) noexcept { // forward _Arg as movable
		return static_cast<remove_reference_t<_Ty>&&>(_Arg);
	}

	template <class _Ty>
	_NODISCARD constexpr conditional_t<!is_nothrow_move_constructible_v<_Ty>&& is_copy_constructible_v<_Ty>, const _Ty&,
		_Ty&&>
		move_if_noexcept(_Ty& _Arg) noexcept { // forward _Arg as movable, sometimes
		return nonstd:: move(_Arg);
	}

	template <class _Ty>
	class reference_wrapper;

	// std::invoke isn't constexpr in C++17, and normally implementers are forbidden from "strengthening" constexpr
	// (WG21-N4842 [constexpr.functions]/1), yet both std::apply and std::visit are required to be constexpr and have
	// invoke-like behavior. As a result, we've chosen to apply the part of P1065R2 resolving LWG-2894 as a defect report.

	enum class _Invoker_strategy {
		_Functor,
		_Pmf_object,
		_Pmf_refwrap,
		_Pmf_pointer,
		_Pmd_object,
		_Pmd_refwrap,
		_Pmd_pointer
	};

	struct _Invoker_functor {
		static constexpr _Invoker_strategy _Strategy = _Invoker_strategy::_Functor;

		template <class _Callable, class... _Types>
		static constexpr auto _Call(_Callable&& _Obj, _Types&&... _Args) noexcept(
			noexcept(static_cast<_Callable&&>(_Obj)(static_cast<_Types&&>(_Args)...)))
			-> decltype(static_cast<_Callable&&>(_Obj)(static_cast<_Types&&>(_Args)...)) {
			return static_cast<_Callable&&>(_Obj)(static_cast<_Types&&>(_Args)...);
		}
	};

	struct _Invoker_pmf_object {
		static constexpr _Invoker_strategy _Strategy = _Invoker_strategy::_Pmf_object;

		template <class _Decayed, class _Ty1, class... _Types2>
		static constexpr auto _Call(_Decayed _Pmf, _Ty1&& _Arg1, _Types2&&... _Args2) noexcept(
			noexcept((static_cast<_Ty1&&>(_Arg1).*_Pmf)(static_cast<_Types2&&>(_Args2)...)))
			-> decltype((static_cast<_Ty1&&>(_Arg1).*_Pmf)(static_cast<_Types2&&>(_Args2)...)) {
			return (static_cast<_Ty1&&>(_Arg1).*_Pmf)(static_cast<_Types2&&>(_Args2)...);
		}
	};

	struct _Invoker_pmf_refwrap {
		static constexpr _Invoker_strategy _Strategy = _Invoker_strategy::_Pmf_refwrap;

		template <class _Decayed, class _Refwrap, class... _Types2>
		static constexpr auto _Call(_Decayed _Pmf, _Refwrap _Rw, _Types2&&... _Args2) noexcept(
			noexcept((_Rw.get().*_Pmf)(static_cast<_Types2&&>(_Args2)...)))
			-> decltype((_Rw.get().*_Pmf)(static_cast<_Types2&&>(_Args2)...)) {
			return (_Rw.get().*_Pmf)(static_cast<_Types2&&>(_Args2)...);
		}
	};

	struct _Invoker_pmf_pointer {
		static constexpr _Invoker_strategy _Strategy = _Invoker_strategy::_Pmf_pointer;

		template <class _Decayed, class _Ty1, class... _Types2>
		static constexpr auto _Call(_Decayed _Pmf, _Ty1&& _Arg1, _Types2&&... _Args2) noexcept(
			noexcept(((*static_cast<_Ty1&&>(_Arg1)).*_Pmf)(static_cast<_Types2&&>(_Args2)...)))
			-> decltype(((*static_cast<_Ty1&&>(_Arg1)).*_Pmf)(static_cast<_Types2&&>(_Args2)...)) {
			return ((*static_cast<_Ty1&&>(_Arg1)).*_Pmf)(static_cast<_Types2&&>(_Args2)...);
		}
	};

	struct _Invoker_pmd_object {
		static constexpr _Invoker_strategy _Strategy = _Invoker_strategy::_Pmd_object;

		template <class _Decayed, class _Ty1>
		static constexpr auto _Call(_Decayed _Pmd, _Ty1&& _Arg1) noexcept -> decltype(static_cast<_Ty1&&>(_Arg1).*_Pmd) {
			return static_cast<_Ty1&&>(_Arg1).*_Pmd;
		}
	};

	struct _Invoker_pmd_refwrap {
		static constexpr _Invoker_strategy _Strategy = _Invoker_strategy::_Pmd_refwrap;

		template <class _Decayed, class _Refwrap>
		static constexpr auto _Call(_Decayed _Pmd, _Refwrap _Rw) noexcept -> decltype(_Rw.get().*_Pmd) {
			return _Rw.get().*_Pmd;
		}
	};

	struct _Invoker_pmd_pointer {
		static constexpr _Invoker_strategy _Strategy = _Invoker_strategy::_Pmd_pointer;

		template <class _Decayed, class _Ty1>
		static constexpr auto _Call(_Decayed _Pmd, _Ty1&& _Arg1) noexcept(noexcept((*static_cast<_Ty1&&>(_Arg1)).*_Pmd))
			-> decltype((*static_cast<_Ty1&&>(_Arg1)).*_Pmd) {
			return (*static_cast<_Ty1&&>(_Arg1)).*_Pmd;
		}
	};

	template <class _Callable, class _Ty1, class _Removed_cvref = _Remove_cvref_t<_Callable>,
		bool _Is_pmf = is_member_function_pointer_v<_Removed_cvref>,
		bool _Is_pmd = is_member_object_pointer_v<_Removed_cvref>>
		struct _Invoker1;

	template <class _Callable, class _Ty1, class _Removed_cvref>
	struct _Invoker1<_Callable, _Ty1, _Removed_cvref, true, false>
		: conditional_t<is_base_of_v<typename _Is_memfunptr<_Removed_cvref>::_Class_type, remove_reference_t<_Ty1>>,
		_Invoker_pmf_object,
		conditional_t<_Is_specialization_v<_Remove_cvref_t<_Ty1>, reference_wrapper>, _Invoker_pmf_refwrap,
		_Invoker_pmf_pointer>> {}; // pointer to member function

	template <class _Callable, class _Ty1, class _Removed_cvref>
	struct _Invoker1<_Callable, _Ty1, _Removed_cvref, false, true>
		: conditional_t<
		is_base_of_v<typename _Is_member_object_pointer<_Removed_cvref>::_Class_type, remove_reference_t<_Ty1>>,
		_Invoker_pmd_object,
		conditional_t<_Is_specialization_v<_Remove_cvref_t<_Ty1>, reference_wrapper>, _Invoker_pmd_refwrap,
		_Invoker_pmd_pointer>> {}; // pointer to member data

	template <class _Callable, class _Ty1, class _Removed_cvref>
	struct _Invoker1<_Callable, _Ty1, _Removed_cvref, false, false> : _Invoker_functor {};

	template <class _Callable>
	constexpr auto invoke(_Callable&& _Obj) noexcept(noexcept(static_cast<_Callable&&>(_Obj)()))
		-> decltype(static_cast<_Callable&&>(_Obj)()) {
		return static_cast<_Callable&&>(_Obj)();
	}
	
	template <class _Callable, class _Ty1, class... _Types2>
	constexpr auto invoke(_Callable&& _Obj, _Ty1&& _Arg1, _Types2&&... _Args2) noexcept(
		noexcept(_Invoker1<_Callable, _Ty1>::_Call(
			static_cast<_Callable&&>(_Obj), static_cast<_Ty1&&>(_Arg1), static_cast<_Types2&&>(_Args2)...)))
		-> decltype(_Invoker1<_Callable, _Ty1>::_Call(
			static_cast<_Callable&&>(_Obj), static_cast<_Ty1&&>(_Arg1), static_cast<_Types2&&>(_Args2)...)) {
		if constexpr (_Invoker1<_Callable, _Ty1>::_Strategy == _Invoker_strategy::_Functor) {
			return static_cast<_Callable&&>(_Obj)(static_cast<_Ty1&&>(_Arg1), static_cast<_Types2&&>(_Args2)...);
		}
		else if constexpr (_Invoker1<_Callable, _Ty1>::_Strategy == _Invoker_strategy::_Pmf_object) {
			return (static_cast<_Ty1&&>(_Arg1).*_Obj)(static_cast<_Types2&&>(_Args2)...);
		}
		else if constexpr (_Invoker1<_Callable, _Ty1>::_Strategy == _Invoker_strategy::_Pmf_refwrap) {
			return (_Arg1.get().*_Obj)(static_cast<_Types2&&>(_Args2)...);
		}
		else if constexpr (_Invoker1<_Callable, _Ty1>::_Strategy == _Invoker_strategy::_Pmf_pointer) {
			return ((*static_cast<_Ty1&&>(_Arg1)).*_Obj)(static_cast<_Types2&&>(_Args2)...);
		}
		else if constexpr (_Invoker1<_Callable, _Ty1>::_Strategy == _Invoker_strategy::_Pmd_object) {
			return static_cast<_Ty1&&>(_Arg1).*_Obj;
		}
		else if constexpr (_Invoker1<_Callable, _Ty1>::_Strategy == _Invoker_strategy::_Pmd_refwrap) {
			return _Arg1.get().*_Obj;
		}
		else {
			static_assert(_Invoker1<_Callable, _Ty1>::_Strategy == _Invoker_strategy::_Pmd_pointer, "bug in invoke");
			return (*static_cast<_Ty1&&>(_Arg1)).*_Obj;
		}
	}

#pragma warning(push)
#pragma warning(disable : 4242) // '%s': conversion from '%s' to '%s', possible loss of data (/Wall)
#pragma warning(disable : 4244) // '%s': conversion from '%s' to '%s', possible loss of data (Yes, duplicated message.)
#pragma warning(disable : 4365) // '%s': conversion from '%s' to '%s', signed/unsigned mismatch (/Wall)

	template <class _From, class _To, bool = is_convertible_v<_From, _To>, bool = is_void_v<_To>>
	_INLINE_VAR constexpr bool _Is_nothrow_convertible_v = noexcept(_Fake_copy_init<_To>(nonstd:: declval<_From>()));

#pragma warning(pop)

	template <class _From, class _To, bool _IsVoid>
	_INLINE_VAR constexpr bool _Is_nothrow_convertible_v<_From, _To, false, _IsVoid> = false;

	template <class _From, class _To>
	_INLINE_VAR constexpr bool _Is_nothrow_convertible_v<_From, _To, true, true> = true;

	template <class _From, class _To>
	struct _Is_nothrow_convertible : bool_constant<_Is_nothrow_convertible_v<_From, _To>> {
		// determine whether _From is nothrow-convertible to _To
	};

#if _HAS_CXX20
	template <class _From, class _To>
	inline constexpr bool is_nothrow_convertible_v = _Is_nothrow_convertible_v<_From, _To>;

	template <class _From, class _To>
	using is_nothrow_convertible = _Is_nothrow_convertible<_From, _To>;
#endif // _HAS_CXX20

	template <class _From, class _To, class = void>
	struct _Invoke_convertible : false_type {};

	template <class _From, class _To>
	struct _Invoke_convertible<_From, _To, void_t<decltype(_Fake_copy_init<_To>(_Returns_exactly<_From>()))>> : true_type {
	};

	template <class _From, class _To>
	struct _Invoke_nothrow_convertible : bool_constant<noexcept(_Fake_copy_init<_To>(_Returns_exactly<_From>()))> {};

	template <class _Result, bool _Nothrow>
	struct _Invoke_traits_common {
		using type = _Result;
		using _Is_invocable = true_type;
		using _Is_nothrow_invocable = bool_constant<_Nothrow>;
		template <class _Rx>
		using _Is_invocable_r = bool_constant<nonstd::disjunction_v<is_void<_Rx>, _Invoke_convertible<type, _Rx>>>;
		template <class _Rx>
		using _Is_nothrow_invocable_r = bool_constant<conjunction_v<_Is_nothrow_invocable,
			disjunction<is_void<_Rx>,
			conjunction<_Invoke_convertible<type, _Rx>, _Invoke_nothrow_convertible<type, _Rx>>>>>;
	};

	template <class _Void, class _Callable>
	struct _Invoke_traits_zero {
		// selected when _Callable isn't callable with zero _Args
		using _Is_invocable = false_type;
		using _Is_nothrow_invocable = false_type;
		template <class _Rx>
		using _Is_invocable_r = false_type;
		template <class _Rx>
		using _Is_nothrow_invocable_r = false_type;
	};

	template <class _Callable>
	using _Decltype_invoke_zero = decltype(nonstd::declval<_Callable>()());

	template <class _Callable>
	struct _Invoke_traits_zero<void_t<_Decltype_invoke_zero<_Callable>>, _Callable>
		: _Invoke_traits_common<_Decltype_invoke_zero<_Callable>, noexcept(nonstd::declval<_Callable>()())> {};

	template <class _Void, class... _Types>
	struct _Invoke_traits_nonzero {
		// selected when _Callable isn't callable with nonzero _Args
		using _Is_invocable = false_type;
		using _Is_nothrow_invocable = false_type;
		template <class _Rx>
		using _Is_invocable_r = false_type;
		template <class _Rx>
		using _Is_nothrow_invocable_r = false_type;
	};

	template <class _Callable, class _Ty1, class... _Types2>
	using _Decltype_invoke_nonzero = decltype(_Invoker1<_Callable, _Ty1>::_Call(
		nonstd::declval<_Callable>(), nonstd::declval<_Ty1>(), nonstd::declval<_Types2>()...));

	template <class _Callable, class _Ty1, class... _Types2>
	struct _Invoke_traits_nonzero<void_t<_Decltype_invoke_nonzero<_Callable, _Ty1, _Types2...>>, _Callable, _Ty1,
		_Types2...> : _Invoke_traits_common<_Decltype_invoke_nonzero<_Callable, _Ty1, _Types2...>,
		noexcept(_Invoker1<_Callable, _Ty1>::_Call(
			nonstd::declval<_Callable>(), nonstd::declval<_Ty1>(), nonstd::declval<_Types2>()...))> {};

	template <class _Callable, class... _Args>
	using _Select_invoke_traits = conditional_t<sizeof...(_Args) == 0, _Invoke_traits_zero<void, _Callable>,
		_Invoke_traits_nonzero<void, _Callable, _Args...>>;

	template <class _Callable, class... _Args>
	using _Invoke_result_t = typename _Select_invoke_traits<_Callable, _Args...>::type;

	template <class _Rx, class _Callable, class... _Args>
	using _Is_invocable_r_ = typename _Select_invoke_traits<_Callable, _Args...>::template _Is_invocable_r<_Rx>;

	template <class _Rx, class _Callable, class... _Args>
	struct _Is_invocable_r : _Is_invocable_r_<_Rx, _Callable, _Args...> {
		// determines whether _Callable is callable with _Args and return type _Rx
	};

#if _HAS_CXX17
	template <class _Callable, class... _Args>
	struct invoke_result : _Select_invoke_traits<_Callable, _Args...> {
		// determine the result type of invoking _Callable with _Args
	};

	template <class _Callable, class... _Args>
	using invoke_result_t = typename _Select_invoke_traits<_Callable, _Args...>::type;

	template <class _Callable, class... _Args>
	struct is_invocable : _Select_invoke_traits<_Callable, _Args...>::_Is_invocable {
		// determines whether _Callable is callable with _Args
	};

	template <class _Callable, class... _Args>
	inline constexpr bool is_invocable_v = _Select_invoke_traits<_Callable, _Args...>::_Is_invocable::value;

	template <class _Callable, class... _Args>
	struct is_nothrow_invocable : _Select_invoke_traits<_Callable, _Args...>::_Is_nothrow_invocable {
		// determines whether _Callable is nothrow-callable with _Args
	};

	template <class _Callable, class... _Args>
	inline constexpr bool is_nothrow_invocable_v = _Select_invoke_traits<_Callable, _Args...>::_Is_nothrow_invocable::value;

	template <class _Rx, class _Callable, class... _Args>
	struct is_invocable_r : _Is_invocable_r_<_Rx, _Callable, _Args...> {
		// determines whether _Callable is callable with _Args and return type _Rx
	};

	template <class _Rx, class _Callable, class... _Args>
	inline constexpr bool is_invocable_r_v = _Is_invocable_r_<_Rx, _Callable, _Args...>::value;

	template <class _Rx, class _Callable, class... _Args>
	struct is_nothrow_invocable_r : _Select_invoke_traits<_Callable, _Args...>::template _Is_nothrow_invocable_r<_Rx> {
		// determines whether _Callable is nothrow-callable with _Args and return type _Rx
	};

	template <class _Rx, class _Callable, class... _Args>
	inline constexpr bool is_nothrow_invocable_r_v =
		_Select_invoke_traits<_Callable, _Args...>::template _Is_nothrow_invocable_r<_Rx>::value;
#endif // _HAS_CXX17
	template <class _Ty>
	struct _Function_args {}; // determine whether _Ty is a function

#define _FUNCTION_ARGS(CALL_OPT, CV_OPT, REF_OPT, NOEXCEPT_OPT)                                           \
    template <class _Ret, class... _Types>                                                                \
    struct _Function_args<_Ret CALL_OPT(_Types...) CV_OPT REF_OPT NOEXCEPT_OPT> : _Arg_types<_Types...> { \
        using _Unnameable_result  = _Ret;                                 \
    };

	//_NON_MEMBER_CALL_CV_REF_NOEXCEPT(_FUNCTION_ARGS)
#undef _FUNCTION_ARGS

#define _FUNCTION_ARGS_ELLIPSIS(CV_REF_NOEXCEPT_OPT)                                                            \
    template <class _Ret, class... _Types>                                                                      \
    struct _Function_args<_Ret(_Types..., ...) CV_REF_NOEXCEPT_OPT> { /* no calling conventions for ellipsis */ \
        using _Unnameable_result  = _Ret;                                       \
    };

		_CLASS_DEFINE_CV_REF_NOEXCEPT(_FUNCTION_ARGS_ELLIPSIS)
#undef _FUNCTION_ARGS_ELLIPSIS

		template <class _Ty, class = void>
	struct _Weak_result_type {}; // default definition

	
		template <class _Ty>
	struct _Weak_result_type<_Ty, void_t<typename _Ty::result_type>> { // defined if _Ty::result_type exists
		using _Unnameable_result  = typename _Ty::result_type;
	};
	

		template <class _Ty, class = void>
	struct _Weak_argument_type : _Weak_result_type<_Ty> {}; // default definition

	
		template <class _Ty>
	struct _Weak_argument_type<_Ty, void_t<typename _Ty::argument_type>> : _Weak_result_type<_Ty> {
		// defined if _Ty::argument_type exists
		using _Unnameable_argument  = typename _Ty::argument_type;
	};
	

		template <class _Ty, class = void>
	struct _Weak_binary_args : _Weak_argument_type<_Ty> {}; // default definition

	
		template <class _Ty>
	struct _Weak_binary_args<_Ty, void_t<typename _Ty::first_argument_type,
		typename _Ty::second_argument_type>>
		: _Weak_argument_type<_Ty> { // defined if both types exist
		using _FIRST_ARGUMENT_TYPE_NAME  = typename _Ty::first_argument_type;
		using _SECOND_ARGUMENT_TYPE_NAME  = typename _Ty::second_argument_type;
	};
	

		template <class _Ty>
	using _Weak_types = conditional_t<is_function_v<remove_pointer_t<_Ty>>, _Function_args<remove_pointer_t<_Ty>>,
		conditional_t<is_member_function_pointer_v<_Ty>, _Is_memfunptr<remove_cv_t<_Ty>>, _Weak_binary_args<_Ty>>>;

	template <class _Ty>
	void _Refwrap_ctor_fun(_Identity_t<_Ty&>) noexcept; // not defined
	template <class _Ty>
	void _Refwrap_ctor_fun(_Identity_t<_Ty&&>) = delete;

	template <class _Ty, class _Uty, class = void>
	struct _Refwrap_has_ctor_from : false_type {};

	template <class _Ty, class _Uty>
	struct _Refwrap_has_ctor_from<_Ty, _Uty, void_t<decltype(_Refwrap_ctor_fun<_Ty>(nonstd:: declval<_Uty>()))>> : true_type {};
	
	template <class _Ty>
	_NODISCARD _CONSTEXPR _Ty* addressof(_Ty& _Val) noexcept {
		return __builtin_addressof(_Val);
	}

	template <class _Ty>
	const _Ty* addressof(const _Ty&&) = delete;

	template <class _Ptrty>
	_NODISCARD _CONSTEXPR auto _Unfancy(_Ptrty _Ptr) noexcept { // converts from a fancy pointer to a plain pointer
		return nonstd::addressof(*_Ptr);
	}

	template <class _Ty>
	_NODISCARD _CONSTEXPR _Ty* _Unfancy(_Ty* _Ptr) noexcept { // do nothing for plain pointers
		return _Ptr;
	}
	
	template <class _Ty>
	class reference_wrapper
	{
	public:
		static_assert(is_object_v<_Ty> || is_function_v<_Ty>,
			"reference_wrapper<T> requires T to be an object type or a function type.");

		using type = _Ty;

		template <class _Uty, enable_if_t<conjunction_v<negation<is_same<_Remove_cvref_t<_Uty>, reference_wrapper>>,
			_Refwrap_has_ctor_from<_Ty, _Uty>>,
			int> = 0>
		constexpr reference_wrapper(_Uty&& _Val) noexcept(noexcept(_Refwrap_ctor_fun<_Ty>(nonstd:: declval<_Uty>()))) {
			_Ty& _Ref = static_cast<_Uty&&>(_Val);
			_Ptr = nonstd:: addressof(_Ref);
		}

		constexpr operator _Ty& () const noexcept {
			return *_Ptr;
		}

		_NODISCARD constexpr _Ty& get() const noexcept {
			return *_Ptr;
		}

	private:
		_Ty* _Ptr{};

	public:
		template <class... _Types>
		constexpr auto operator()(_Types&&... _Args) const
			noexcept(noexcept(nonstd:: invoke(*_Ptr, static_cast<_Types&&>(_Args)...))) // strengthened
			-> decltype(nonstd:: invoke(*_Ptr, static_cast<_Types&&>(_Args)...)) {
			return nonstd:: invoke(*_Ptr, static_cast<_Types&&>(_Args)...);
		}
	};

#if _HAS_CXX17
	template <class _Ty>
	reference_wrapper(_Ty&) -> reference_wrapper<_Ty>;
#endif // _HAS_CXX17

	template <class _Ty>
	_NODISCARD constexpr reference_wrapper<_Ty> ref(_Ty& _Val) noexcept {
		return reference_wrapper<_Ty>(_Val);
	}

	template <class _Ty>
	void ref(const _Ty&&) = delete;

	template <class _Ty>
	_NODISCARD constexpr reference_wrapper<_Ty> ref(reference_wrapper<_Ty> _Val) noexcept {
		return _Val;
	}

	template <class _Ty>
	_NODISCARD constexpr reference_wrapper<const _Ty> cref(const _Ty& _Val) noexcept {
		return reference_wrapper<const _Ty>(_Val);
	}

	template <class _Ty>
	void cref(const _Ty&&) = delete;

	template <class _Ty>
	_NODISCARD constexpr reference_wrapper<const _Ty> cref(reference_wrapper<_Ty> _Val) noexcept {
		return _Val;
	}

#if _HAS_CXX20
	template <class _Ty>
	struct unwrap_reference {
		using type = _Ty;
	};
	template <class _Ty>
	struct unwrap_reference<reference_wrapper<_Ty>> {
		using type = _Ty&;
	};
	template <class _Ty>
	using unwrap_reference_t = typename unwrap_reference<_Ty>::type;

	template <class _Ty>
	using unwrap_ref_decay_t = unwrap_reference_t<decay_t<_Ty>>;
	template <class _Ty>
	struct unwrap_ref_decay {
		using type = unwrap_ref_decay_t<_Ty>;
	};
#endif // _HAS_CXX20

	template <class _Ty>
	struct _Is_swappable;

	template <class _Ty>
	struct _Is_nothrow_swappable;

	template <class _Ty, enable_if_t<is_move_constructible_v<_Ty>&& is_move_assignable_v<_Ty>, int> = 0>

	constexpr void swap(_Ty&, _Ty&) noexcept(is_nothrow_move_constructible_v<_Ty>&& is_nothrow_move_assignable_v<_Ty>);

	template <class _Ty, size_t _Size, enable_if_t<_Is_swappable<_Ty>::value, int> = 0>
	constexpr void swap(_Ty(&)[_Size], _Ty(&)[_Size]) noexcept(_Is_nothrow_swappable<_Ty>::value);

	template <class _Ty1, class _Ty2, class = void>
	struct _Swappable_with_helper : false_type {}; // swap(declval<_Ty1>(), declval<_Ty2>()) is not valid

	template <class _Ty1, class _Ty2>
	struct _Swappable_with_helper<_Ty1, _Ty2, void_t<decltype(swap(nonstd:: declval<_Ty1>(), nonstd:: declval<_Ty2>()))>>
		: true_type {}; // swap(declval<_Ty1>(), declval<_Ty2>()) is valid

	template <class _Ty1, class _Ty2>
	struct _Is_swappable_with
		: bool_constant<conjunction_v<_Swappable_with_helper<_Ty1, _Ty2>, _Swappable_with_helper<_Ty2, _Ty1>>> {
		// Determine if expressions with type and value category _Ty1 and _Ty2 can be swapped (and vice versa)
	};

	template <class _Ty>
	struct _Is_swappable : _Is_swappable_with<add_lvalue_reference_t<_Ty>, add_lvalue_reference_t<_Ty>>::type {
		// Determine if _Ty lvalues satisfy is_swappable_with
	};

	template <class _Ty1, class _Ty2>
	struct _Swap_cannot_throw : bool_constant<noexcept(swap(nonstd:: declval<_Ty1>(), nonstd:: declval<_Ty2>())) //
		&& noexcept(swap(nonstd:: declval<_Ty2>(), nonstd:: declval<_Ty1>()))> {
		// Determine if expressions with type and value category _Ty1 and _Ty2
		// (presumed to satisfy is_swappable_with) can be swapped without emitting exceptions
	};

	template <class _Ty1, class _Ty2>
	struct _Is_nothrow_swappable_with
		: bool_constant<conjunction_v<_Is_swappable_with<_Ty1, _Ty2>, _Swap_cannot_throw<_Ty1, _Ty2>>> {
		// Determine if expressions with type and value category _Ty1 and _Ty2
		// satisfy is_swappable_with, and can be swapped without emitting exceptions
	};

	template <class _Ty>
	struct _Is_nothrow_swappable
		: _Is_nothrow_swappable_with<add_lvalue_reference_t<_Ty>, add_lvalue_reference_t<_Ty>>::type {
		// Determine if _Ty lvalues satisfy is_nothrow_swappable_with
	};

#if _HAS_CXX17
	template <class _Ty1, class _Ty2>
	struct is_swappable_with : _Is_swappable_with<_Ty1, _Ty2>::type {
		// Determine if expressions with type and value category _Ty1 and _Ty2
		// can be swapped (and vice versa)
	};

	template <class _Ty1, class _Ty2>
	inline constexpr bool is_swappable_with_v =
		conjunction_v<_Swappable_with_helper<_Ty1, _Ty2>, _Swappable_with_helper<_Ty2, _Ty1>>;

	template <class _Ty>
	struct is_swappable : _Is_swappable<_Ty>::type {}; // Determine if _Ty lvalues satisfy is_swappable_with

	template <class _Ty>
	inline constexpr bool is_swappable_v = _Is_swappable<_Ty>::value;

	template <class _Ty1, class _Ty2>
	struct is_nothrow_swappable_with : _Is_nothrow_swappable_with<_Ty1, _Ty2>::type {
		// Determine if expressions with type and value category _Ty1 and _Ty2
		// satisfy is_swappable_with, and can be swapped without emitting exceptions
	};

	template <class _Ty1, class _Ty2>
	inline constexpr bool is_nothrow_swappable_with_v = _Is_nothrow_swappable_with<_Ty1, _Ty2>::value;

	template <class _Ty>
	struct is_nothrow_swappable : _Is_nothrow_swappable<_Ty>::type {
		// Determine if _Ty lvalues satisfy is_nothrow_swappable_with
	};

	template <class _Ty>
	inline constexpr bool is_nothrow_swappable_v = _Is_nothrow_swappable<_Ty>::value;
#endif // _HAS_CXX17

	namespace _Has_ADL_swap_detail {
		void swap(); // undefined (deliberate shadowing)

		template <class, class = void>
		struct _Has_ADL_swap : false_type {};
		template <class _Ty>
		struct _Has_ADL_swap<_Ty, void_t<decltype(swap(nonstd:: declval<_Ty&>(), nonstd:: declval<_Ty&>()))>> : true_type {};
	} // namespace _Has_ADL_swap_detail
	using _Has_ADL_swap_detail::_Has_ADL_swap;

	template <class _Ty>
	_INLINE_VAR constexpr bool _Is_trivially_swappable_v = conjunction_v<is_trivially_destructible<_Ty>,
		is_trivially_move_constructible<_Ty>, is_trivially_move_assignable<_Ty>, negation<_Has_ADL_swap<_Ty>>>;


	template <>
	inline constexpr bool _Is_trivially_swappable_v<byte> = true;

	template <class _Ty>
	struct _Is_trivially_swappable : bool_constant<_Is_trivially_swappable_v<_Ty>> {
		// true_type if and only if it is valid to swap two _Ty lvalues by exchanging object representations.
	};

#define _NONSTD_BITMASK_OPS(_BITMASK)                                                                  \
    _NODISCARD constexpr _BITMASK operator&(_BITMASK _Left, _BITMASK _Right) noexcept {         \
        using _IntTy = nonstd:: underlying_type_t<_BITMASK>;                                        \
        return static_cast<_BITMASK>(static_cast<_IntTy>(_Left) & static_cast<_IntTy>(_Right)); \
    }                                                                                           \
                                                                                                \
    _NODISCARD constexpr _BITMASK operator|(_BITMASK _Left, _BITMASK _Right) noexcept {         \
        using _IntTy = nonstd:: underlying_type_t<_BITMASK>;                                        \
        return static_cast<_BITMASK>(static_cast<_IntTy>(_Left) | static_cast<_IntTy>(_Right)); \
    }                                                                                           \
                                                                                                \
    _NODISCARD constexpr _BITMASK operator^(_BITMASK _Left, _BITMASK _Right) noexcept {         \
        using _IntTy = nonstd:: underlying_type_t<_BITMASK>;                                        \
        return static_cast<_BITMASK>(static_cast<_IntTy>(_Left) ^ static_cast<_IntTy>(_Right)); \
    }                                                                                           \
                                                                                                \
    constexpr _BITMASK& operator&=(_BITMASK& _Left, _BITMASK _Right) noexcept {                 \
        return _Left = _Left & _Right;                                                          \
    }                                                                                           \
                                                                                                \
    constexpr _BITMASK& operator|=(_BITMASK& _Left, _BITMASK _Right) noexcept {                 \
        return _Left = _Left | _Right;                                                          \
    }                                                                                           \
                                                                                                \
    constexpr _BITMASK& operator^=(_BITMASK& _Left, _BITMASK _Right) noexcept {                 \
        return _Left = _Left ^ _Right;                                                          \
    }                                                                                           \
                                                                                                \
    _NODISCARD constexpr _BITMASK operator~(_BITMASK _Left) noexcept {                          \
        using _IntTy = nonstd:: underlying_type_t<_BITMASK>;                                        \
        return static_cast<_BITMASK>(~static_cast<_IntTy>(_Left));                              \
    }

	template <class _Bitmask>
	_NODISCARD constexpr bool _Bitmask_includes_any(_Bitmask _Left, _Bitmask _Elements) noexcept {
		return (_Left & _Elements) != _Bitmask{};
	}

	template <class _Bitmask>
	_NODISCARD constexpr bool _Bitmask_includes_all(_Bitmask _Left, _Bitmask _Elements) noexcept {
		return (_Left & _Elements) == _Elements;
	}

	// These FNV-1a utility functions are extremely performance sensitive,
	// check examples like that in VSO-653642 before making changes.
#if defined(_WIN64)
	_INLINE_VAR constexpr size_t _FNV_offset_basis = 14695981039346656037ULL;
	_INLINE_VAR constexpr size_t _FNV_prime = 1099511628211ULL;
#else // defined(_WIN64)
	_INLINE_VAR constexpr size_t _FNV_offset_basis = 2166136261U;
	_INLINE_VAR constexpr size_t _FNV_prime = 16777619U;
#endif // defined(_WIN64)

	_NODISCARD inline size_t _Fnv1a_append_bytes(size_t _Val, const unsigned char* const _First,
		const size_t _Count) noexcept { // accumulate range [_First, _First + _Count) into partial FNV-1a hash _Val
		for (size_t _Idx = 0; _Idx < _Count; ++_Idx) {
			_Val ^= static_cast<size_t>(_First[_Idx]);
			_Val *= _FNV_prime;
		}

		return _Val;
	}

	template <class _Ty>
	_NODISCARD size_t _Fnv1a_append_range(const size_t _Val, const _Ty* const _First,
		const _Ty* const _Last) noexcept { // accumulate range [_First, _Last) into partial FNV-1a hash _Val
		static_assert(is_trivial_v<_Ty>, "Only trivial types can be directly hashed.");
		const auto _Firstb = reinterpret_cast<const unsigned char*>(_First);
		const auto _Lastb = reinterpret_cast<const unsigned char*>(_Last);
		return _Fnv1a_append_bytes(_Val, _Firstb, static_cast<size_t>(_Lastb - _Firstb));
	}

	template <class _Kty>
	_NODISCARD size_t _Fnv1a_append_value(
		const size_t _Val, const _Kty& _Keyval) noexcept { // accumulate _Keyval into partial FNV-1a hash _Val
		static_assert(is_trivial_v<_Kty>, "Only trivial types can be directly hashed.");
		return _Fnv1a_append_bytes(_Val, &reinterpret_cast<const unsigned char&>(_Keyval), sizeof(_Kty));
	}

	template <class _Kty>
	_NODISCARD size_t _Hash_representation(const _Kty& _Keyval) noexcept { // bitwise hashes the representation of a key
		return _Fnv1a_append_value(_FNV_offset_basis, _Keyval);
	}

	template <class _Kty>
	_NODISCARD size_t _Hash_array_representation(
		const _Kty* const _First, const size_t _Count) noexcept { // bitwise hashes the representation of an array
		static_assert(is_trivial_v<_Kty>, "Only trivial types can be directly hashed.");
		return _Fnv1a_append_bytes(
			_FNV_offset_basis, reinterpret_cast<const unsigned char*>(_First), _Count * sizeof(_Kty));
	}

	template <class _Kty>
	struct hash;

	template <class _Kty, bool _Enabled>
	struct _Conditionally_enabled_hash { // conditionally enabled hash base
		using _Unnameable_argument  = _Kty;
		using _Unnameable_result  = size_t;

		_NODISCARD size_t operator()(const _Kty& _Keyval) const
			noexcept(noexcept(hash<_Kty>::_Do_hash(_Keyval))) /* strengthened */ {
			return hash<_Kty>::_Do_hash(_Keyval);
		}
	};

	template <class _Kty>
	struct _Conditionally_enabled_hash<_Kty, false> { // conditionally disabled hash base
		_Conditionally_enabled_hash() = delete;
		_Conditionally_enabled_hash(const _Conditionally_enabled_hash&) = delete;
		_Conditionally_enabled_hash(_Conditionally_enabled_hash&&) = delete;
		_Conditionally_enabled_hash& operator=(const _Conditionally_enabled_hash&) = delete;
		_Conditionally_enabled_hash& operator=(_Conditionally_enabled_hash&&) = delete;
	};

	template <class _Kty>
	struct hash
		: _Conditionally_enabled_hash<_Kty,
		!is_const_v<_Kty> && !is_volatile_v<_Kty> && (is_enum_v<_Kty> || is_integral_v<_Kty> || is_pointer_v<_Kty>)> {
		// hash functor primary template (handles enums, integrals, and pointers)
		static size_t _Do_hash(const _Kty& _Keyval) noexcept {
			return _Hash_representation(_Keyval);
		}
	};

	template <>
	struct hash<float> {
		using _Unnameable_argument  = float;
		using _Unnameable_result  = size_t;
		_NODISCARD size_t operator()(const float _Keyval) const noexcept {
			return _Hash_representation(_Keyval == 0.0F ? 0.0F : _Keyval); // map -0 to 0
		}
	};

	template <>
	struct hash<double> {
		using _Unnameable_argument  = double;
		using _Unnameable_result  = size_t;
		_NODISCARD size_t operator()(const double _Keyval) const noexcept {
			return _Hash_representation(_Keyval == 0.0 ? 0.0 : _Keyval); // map -0 to 0
		}
	};

	template <>
	struct hash<long double> {
		using _Unnameable_argument  = long double;
		using _Unnameable_result  = size_t;
		_NODISCARD size_t operator()(const long double _Keyval) const noexcept {
			return _Hash_representation(_Keyval == 0.0L ? 0.0L : _Keyval); // map -0 to 0
		}
	};

	template <>
	struct hash<nullptr_t> {
		using _Unnameable_argument  = nullptr_t;
		using _Unnameable_result  = size_t;
		_NODISCARD size_t operator()(nullptr_t) const noexcept {
			void* _Null{};
			return _Hash_representation(_Null);
		}
	};

	template <class _Kty, class = void>
	struct _Is_nothrow_hashable : false_type {}; // tests if std::hash can hash _Kty with noexcept

	template <class _Kty>
	struct _Is_nothrow_hashable < _Kty, void_t<decltype(hash<_Kty>{}(nonstd:: declval<const _Kty&>())) >>
		: bool_constant < noexcept(hash<_Kty>{}(nonstd:: declval<const _Kty&>())) > {};

	// vvvvvvvvvv DERIVED FROM corecrt_internal_fltintrn.h vvvvvvvvvv

	template <class _FloatingType>
	struct _Floating_type_traits;

	template <>
	struct _Floating_type_traits<float> {
		static constexpr int32_t _Mantissa_bits = 24; // FLT_MANT_DIG
		static constexpr int32_t _Exponent_bits = 8; // sizeof(float) * CHAR_BIT - FLT_MANT_DIG
		static constexpr int32_t _Maximum_binary_exponent = 127; // FLT_MAX_EXP - 1
		static constexpr int32_t _Minimum_binary_exponent = -126; // FLT_MIN_EXP - 1
		static constexpr int32_t _Exponent_bias = 127;
		static constexpr int32_t _Sign_shift = 31; // _Exponent_bits + _Mantissa_bits - 1
		static constexpr int32_t _Exponent_shift = 23; // _Mantissa_bits - 1

		using _Uint_type = uint32_t;

		static constexpr uint32_t _Exponent_mask = 0x000000FFu; // (1u << _Exponent_bits) - 1
		static constexpr uint32_t _Normal_mantissa_mask = 0x00FFFFFFu; // (1u << _Mantissa_bits) - 1
		static constexpr uint32_t _Denormal_mantissa_mask = 0x007FFFFFu; // (1u << (_Mantissa_bits - 1)) - 1
		static constexpr uint32_t _Special_nan_mantissa_mask = 0x00400000u; // 1u << (_Mantissa_bits - 2)
		static constexpr uint32_t _Shifted_sign_mask = 0x80000000u; // 1u << _Sign_shift
		static constexpr uint32_t _Shifted_exponent_mask = 0x7F800000u; // _Exponent_mask << _Exponent_shift
	};

	template <>
	struct _Floating_type_traits<double> {
		static constexpr int32_t _Mantissa_bits = 53; // DBL_MANT_DIG
		static constexpr int32_t _Exponent_bits = 11; // sizeof(double) * CHAR_BIT - DBL_MANT_DIG
		static constexpr int32_t _Maximum_binary_exponent = 1023; // DBL_MAX_EXP - 1
		static constexpr int32_t _Minimum_binary_exponent = -1022; // DBL_MIN_EXP - 1
		static constexpr int32_t _Exponent_bias = 1023;
		static constexpr int32_t _Sign_shift = 63; // _Exponent_bits + _Mantissa_bits - 1
		static constexpr int32_t _Exponent_shift = 52; // _Mantissa_bits - 1

		using _Uint_type = uint64_t;

		static constexpr uint64_t _Exponent_mask = 0x00000000000007FFu; // (1ULL << _Exponent_bits) - 1
		static constexpr uint64_t _Normal_mantissa_mask = 0x001FFFFFFFFFFFFFu; // (1ULL << _Mantissa_bits) - 1
		static constexpr uint64_t _Denormal_mantissa_mask = 0x000FFFFFFFFFFFFFu; // (1ULL << (_Mantissa_bits - 1)) - 1
		static constexpr uint64_t _Special_nan_mantissa_mask = 0x0008000000000000u; // 1ULL << (_Mantissa_bits - 2)
		static constexpr uint64_t _Shifted_sign_mask = 0x8000000000000000u; // 1ULL << _Sign_shift
		static constexpr uint64_t _Shifted_exponent_mask = 0x7FF0000000000000u; // _Exponent_mask << _Exponent_shift
	};

	template <>
	struct _Floating_type_traits<long double> : _Floating_type_traits<double> {};

}