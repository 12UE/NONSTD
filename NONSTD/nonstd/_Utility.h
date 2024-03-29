
#pragma once
namespace nonstd {
    template <class _Ty, class _Pr>
    _NODISCARD constexpr const _Ty& (_max)(const _Ty& _Left, const _Ty& _Right, _Pr _Pred) noexcept(
        noexcept(_Pred(_Left, _Right))) /* strengthened */ {
        // return larger of _Left and _Right
        return _Pred(_Left, _Right) ? _Right : _Left;
    }

    #pragma warning(push)
    #pragma warning(disable : 28285) // (syntax error in SAL annotation, occurs when _Ty is not an integral type)
    template <class _Ty>
    _NODISCARD _Post_equal_to_(_Left < _Right ? _Right : _Left) constexpr const _Ty& //
        (_max)(const _Ty& _Left, const _Ty& _Right) noexcept(noexcept(_Left < _Right)) /* strengthened */ {
        // return larger of _Left and _Right
        return _Left < _Right ? _Right : _Left;
    }
    #pragma warning(pop)

    template <class _Ty, class _Pr>
    _NODISCARD constexpr _Ty(_max)(nonstd::initializer_list<_Ty>, _Pr); // implemented in <algorithm>

    template <class _Ty>
    _NODISCARD constexpr _Ty(_max)(nonstd::initializer_list<_Ty>); // implemented in <algorithm>

    template <class _Ty, class _Pr>
    _NODISCARD constexpr const _Ty& (_min)(const _Ty& _Left, const _Ty& _Right, _Pr _Pred) noexcept(
        noexcept(_Pred(_Right, _Left))) /* strengthened */ {
        // return smaller of _Left and _Right
        return _Pred(_Right, _Left) ? _Right : _Left;
    }

    #pragma warning(push)
    #pragma warning(disable : 28285) // (syntax error in SAL annotation, occurs when _Ty is not an integral type)
    template <class _Ty>
    _NODISCARD _Post_equal_to_(_Right < _Left ? _Right : _Left) constexpr const _Ty& //
        (_min)(const _Ty& _Left, const _Ty& _Right) noexcept(noexcept(_Right < _Left)) /* strengthened */ {
        // return smaller of _Left and _Right
        return _Right < _Left ? _Right : _Left;
    }
    #pragma warning(pop)

    template <class _Ty, class _Pr>
    _NODISCARD constexpr _Ty(_min)(nonstd::initializer_list<_Ty>, _Pr); // implemented in <algorithm>

    template <class _Ty>
    _NODISCARD constexpr _Ty(_min)(nonstd::initializer_list<_Ty>); // implemented in <algorithm>

    template <class _FwdIt1, class _FwdIt2>
    constexpr void iter_swap(_FwdIt1 _Left, _FwdIt2 _Right) { // swap *_Left and *_Right
        nonstd::swap(*_Left, *_Right);
    }

    template <class _Ty, size_t _Size, enable_if_t<_Is_swappable<_Ty>::value, int> _Enabled>
    constexpr void swap(_Ty(&_Left)[_Size], _Ty(&_Right)[_Size]) noexcept(_Is_nothrow_swappable<_Ty>::value) {
        if (&_Left != &_Right) {
            _Ty* _First1 = _Left;
            _Ty* _Last1 = _First1 + _Size;
            _Ty* _First2 = _Right;
            for (; _First1 != _Last1; ++_First1, ++_First2) {
                nonstd:: iter_swap(_First1, _First2);
            }
        }
    }

    #if _HAS_CXX17
    template <class _Ty, enable_if_t<is_move_constructible_v<_Ty>&& is_move_assignable_v<_Ty>, int> _Enabled>
    #else // ^^^ _HAS_CXX17 / !_HAS_CXX17 vvv
    template <class _Ty, int _Enabled>
    #endif // _HAS_CXX17
    constexpr void swap(_Ty& _Left, _Ty& _Right) noexcept(
        is_nothrow_move_constructible_v<_Ty>&& is_nothrow_move_assignable_v<_Ty>) {
        _Ty _Tmp = nonstd:: move(_Left);
        _Left = nonstd:: move(_Right);
        _Right = nonstd:: move(_Tmp);
    }

    template <class _Ty>
    constexpr void _Swap_adl(_Ty& _Left, _Ty& _Right) noexcept(_Is_nothrow_swappable<_Ty>::value) {
        swap(_Left, _Right);
    }

    struct piecewise_construct_t { // tag type for pair tuple arguments
        explicit piecewise_construct_t() = default;
    };

    _INLINE_VAR constexpr piecewise_construct_t piecewise_construct{};

    template <class _Ty, class _Alloc, class = void>
    struct _Has_allocator_type : false_type {}; // tests for suitable _Ty::allocator_type

    template <class _Ty, class _Alloc>
    struct _Has_allocator_type<_Ty, _Alloc, void_t<typename _Ty::allocator_type>>
        : is_convertible<_Alloc, typename _Ty::allocator_type>::type {}; // tests for suitable _Ty::allocator_type

    struct allocator_arg_t { // tag type for added allocator argument
        explicit allocator_arg_t() = default;
    };

    _INLINE_VAR constexpr allocator_arg_t allocator_arg{};

    template <class _Ty, class _Alloc>
    struct uses_allocator : _Has_allocator_type<_Ty, _Alloc>::type {
        // determine whether _Ty has an allocator_type member type
    };

    template <class _Ty, class _Alloc>
    _INLINE_VAR constexpr bool uses_allocator_v = uses_allocator<_Ty, _Alloc>::value;

    template <class...>
    class tuple;

    template <size_t _Index, class... _Types>
    _NODISCARD constexpr auto&& _Tuple_get(tuple<_Types...>&& _Tuple) noexcept;

    template <class _Ty1, class _Ty2>
    struct pair { // store a pair of values
        using first_type = _Ty1;
        using second_type = _Ty2;

        template <class _Uty1 = _Ty1, class _Uty2 = _Ty2,
            enable_if_t<conjunction_v<is_default_constructible<_Uty1>, is_default_constructible<_Uty2>>, int> = 0>
        constexpr explicit(
            !conjunction_v<_Is_implicitly_default_constructible<_Uty1>, _Is_implicitly_default_constructible<_Uty2>>)
            pair() noexcept(
                is_nothrow_default_constructible_v<_Uty1>&& is_nothrow_default_constructible_v<_Uty2>) // strengthened
            : first(), second() {}

        template <class _Uty1 = _Ty1, class _Uty2 = _Ty2,
            enable_if_t<conjunction_v<is_copy_constructible<_Uty1>, is_copy_constructible<_Uty2>>, int> = 0>
        constexpr explicit(!conjunction_v<is_convertible<const _Uty1&, _Uty1>, is_convertible<const _Uty2&, _Uty2>>)
            pair(const _Ty1& _Val1, const _Ty2& _Val2) noexcept(
                is_nothrow_copy_constructible_v<_Uty1>&& is_nothrow_copy_constructible_v<_Uty2>) // strengthened
            : first(_Val1), second(_Val2) {}

        template <class _Other1, class _Other2,
            enable_if_t<conjunction_v<is_constructible<_Ty1, _Other1>, is_constructible<_Ty2, _Other2>>, int> = 0>
        constexpr explicit(!conjunction_v<is_convertible<_Other1, _Ty1>, is_convertible<_Other2, _Ty2>>)
            pair(_Other1&& _Val1, _Other2&& _Val2) noexcept(
                is_nothrow_constructible_v<_Ty1, _Other1>&& is_nothrow_constructible_v<_Ty2, _Other2>) // strengthened
            : first(nonstd:: forward<_Other1>(_Val1)), second(nonstd:: forward<_Other2>(_Val2)) {
        }

        pair(const pair&) = default;
        pair(pair&&) = default;



        template <class _Other1, class _Other2,
            enable_if_t<conjunction_v<is_constructible<_Ty1, const _Other1&>, is_constructible<_Ty2, const _Other2&>>,
            int> = 0>
        constexpr explicit(!conjunction_v<is_convertible<const _Other1&, _Ty1>, is_convertible<const _Other2&, _Ty2>>)
            pair(const pair<_Other1, _Other2>& _Right) noexcept(is_nothrow_constructible_v<_Ty1, const _Other1&>&&
                is_nothrow_constructible_v<_Ty2, const _Other2&>) // strengthened
            : first(_Right.first), second(_Right.second) {}

        template <class _Other1, class _Other2,
            enable_if_t<conjunction_v<is_constructible<_Ty1, _Other1>, is_constructible<_Ty2, _Other2>>, int> = 0>
        constexpr explicit(!conjunction_v<is_convertible<_Other1, _Ty1>, is_convertible<_Other2, _Ty2>>)
            pair(pair<_Other1, _Other2>&& _Right) noexcept(
                is_nothrow_constructible_v<_Ty1, _Other1>&& is_nothrow_constructible_v<_Ty2, _Other2>) // strengthened
            : first(nonstd:: forward<_Other1>(_Right.first)), second(nonstd:: forward<_Other2>(_Right.second)) {}

    #if _HAS_CXX23
        template <class _Other1, class _Other2,
            enable_if_t<conjunction_v<is_constructible<_Ty1, const _Other1>, is_constructible<_Ty2, const _Other2>>, int> =
            0>
        constexpr explicit(!conjunction_v<is_convertible<const _Other1, _Ty1>, is_convertible<const _Other2, _Ty2>>)
            pair(const pair<_Other1, _Other2>&& _Right) noexcept(is_nothrow_constructible_v<_Ty1, const _Other1>&&
                is_nothrow_constructible_v<_Ty2, const _Other2>) // strengthened
            : first(nonstd:: forward<const _Other1>(_Right.first)), second(nonstd:: forward<const _Other2>(_Right.second)) {}
    #endif // _HAS_CXX23

        template <class _Tuple1, class _Tuple2, size_t... _Indexes1, size_t... _Indexes2>
        constexpr pair(_Tuple1& _Val1, _Tuple2& _Val2, index_sequence<_Indexes1...>, index_sequence<_Indexes2...>)
            : first(_Tuple_get<_Indexes1>(nonstd:: move(_Val1))...), second(_Tuple_get<_Indexes2>(nonstd:: move(_Val2))...) {}

        template <class... _Types1, class... _Types2>
        constexpr pair(piecewise_construct_t, tuple<_Types1...> _Val1, tuple<_Types2...> _Val2)
            : pair(_Val1, _Val2, index_sequence_for<_Types1...>{}, index_sequence_for<_Types2...>{}) {}

        pair& operator=(const volatile pair&) = delete;

        template <class _Myself = pair,
            enable_if_t<conjunction_v<_Is_copy_assignable_no_precondition_check<typename _Myself::first_type>,
            _Is_copy_assignable_no_precondition_check<typename _Myself::second_type>>,
            int> = 0>
            constexpr pair& operator=(_Identity_t<const _Myself&> _Right) noexcept(
                conjunction_v<is_nothrow_copy_assignable<_Ty1>, is_nothrow_copy_assignable<_Ty2>>) /* strengthened */ {
            first = _Right.first;
            second = _Right.second;
            return *this;
        }

    #if _HAS_CXX23
        template <class _Myself = pair,
            enable_if_t<conjunction_v<_Is_copy_assignable_no_precondition_check<const typename _Myself::first_type>,
            _Is_copy_assignable_no_precondition_check<const typename _Myself::second_type>>,
            int> = 0>
            constexpr const pair& operator=(_Identity_t<const _Myself&> _Right) const
            noexcept(conjunction_v<is_nothrow_copy_assignable<const _Ty1>,
                is_nothrow_copy_assignable<const _Ty2>>) /* strengthened */ {
            first = _Right.first;
            second = _Right.second;
            return *this;
        }
    #endif // _HAS_CXX23

        template <class _Myself = pair,
            enable_if_t<conjunction_v<_Is_move_assignable_no_precondition_check<typename _Myself::first_type>,
            _Is_move_assignable_no_precondition_check<typename _Myself::second_type>>,
            int> = 0>
            constexpr pair& operator=(_Identity_t<_Myself&&> _Right) noexcept(
                conjunction_v<is_nothrow_move_assignable<_Ty1>, is_nothrow_move_assignable<_Ty2>>) /* strengthened */ {
            first = nonstd:: forward<_Ty1>(_Right.first);
            second = nonstd:: forward<_Ty2>(_Right.second);
            return *this;
        }

    #if _HAS_CXX23
        template <class _Myself = pair,
            enable_if_t<conjunction_v<_Is_assignable_no_precondition_check<const typename _Myself::first_type&, _Ty1>,
            _Is_assignable_no_precondition_check<const typename _Myself::second_type&, _Ty2>>,
            int> = 0>
            constexpr const pair& operator=(_Identity_t<_Myself&&> _Right) const
            noexcept(conjunction_v<is_nothrow_assignable<const _Ty1&, _Ty1>,
                is_nothrow_assignable<const _Ty2&, _Ty2>>) /* strengthened */ {
            first = nonstd:: forward<_Ty1>(_Right.first);
            second = nonstd:: forward<_Ty2>(_Right.second);
            return *this;
        }
    #endif // _HAS_CXX23

        template <class _Other1, class _Other2,
            enable_if_t<conjunction_v<negation<is_same<pair, pair<_Other1, _Other2>>>, is_assignable<_Ty1&, const _Other1&>,
            is_assignable<_Ty2&, const _Other2&>>,
            int> = 0>
            constexpr pair& operator=(const pair<_Other1, _Other2>& _Right) noexcept(
                is_nothrow_assignable_v<_Ty1&, const _Other1&>&&
                is_nothrow_assignable_v<_Ty2&, const _Other2&>) /* strengthened */ {
            first = _Right.first;
            second = _Right.second;
            return *this;
        }

    #if _HAS_CXX23
        template <class _Other1, class _Other2,
            enable_if_t<conjunction_v<negation<is_same<pair, pair<_Other1, _Other2>>>,
            is_assignable<const _Ty1&, const _Other1&>, is_assignable<const _Ty2&, const _Other2&>>,
            int> = 0>
            constexpr const pair& operator=(const pair<_Other1, _Other2>& _Right) const
            noexcept(is_nothrow_assignable_v<const _Ty1&, const _Other1&>&&
                is_nothrow_assignable_v<const _Ty2&, const _Other2&>) /* strengthened */ {
            first = _Right.first;
            second = _Right.second;
            return *this;
        }
    #endif // _HAS_CXX23

        template <class _Other1, class _Other2,
            enable_if_t<conjunction_v<negation<is_same<pair, pair<_Other1, _Other2>>>, is_assignable<_Ty1&, _Other1>,
            is_assignable<_Ty2&, _Other2>>,
            int> = 0>
            constexpr pair& operator=(pair<_Other1, _Other2>&& _Right) noexcept(
                is_nothrow_assignable_v<_Ty1&, _Other1>&& is_nothrow_assignable_v<_Ty2&, _Other2>) /* strengthened */ {
            first = nonstd:: forward<_Other1>(_Right.first);
            second = nonstd:: forward<_Other2>(_Right.second);
            return *this;
        }

    #if _HAS_CXX23
        template <class _Other1, class _Other2,
            enable_if_t<conjunction_v<negation<is_same<pair, pair<_Other1, _Other2>>>, is_assignable<const _Ty1&, _Other1>,
            is_assignable<const _Ty2&, _Other2>>,
            int> = 0>
            constexpr const pair& operator=(pair<_Other1, _Other2>&& _Right) const
            noexcept(is_nothrow_assignable_v<const _Ty1&, _Other1>&&
                is_nothrow_assignable_v<const _Ty2&, _Other2>) /* strengthened */ {
            first = nonstd:: forward<_Other1>(_Right.first);
            second = nonstd:: forward<_Other2>(_Right.second);
            return *this;
        }
    #endif // _HAS_CXX23

        constexpr void swap(pair& _Right) noexcept(
            _Is_nothrow_swappable<_Ty1>::value&& _Is_nothrow_swappable<_Ty2>::value) {
            if (this != nonstd:: addressof(_Right)) {
                _Swap_adl(first, _Right.first);
                _Swap_adl(second, _Right.second);
            }
        }

    #if _HAS_CXX23
        template <int = 0> // see GH-3013
        constexpr void swap(const pair& _Right) const
            noexcept(is_nothrow_swappable_v<const _Ty1>&& is_nothrow_swappable_v<const _Ty2>) {
            if (this != nonstd:: addressof(_Right)) {
                _Swap_adl(first, _Right.first);
                _Swap_adl(second, _Right.second);
            }
        }
    #endif // _HAS_CXX23

        _Ty1 first; // the first stored value
        _Ty2 second; // the second stored value
    };

    #if _HAS_CXX17
    template <class _Ty1, class _Ty2>
    pair(_Ty1, _Ty2) -> pair<_Ty1, _Ty2>;
    #endif // _HAS_CXX17

    template <class _Ty1, class _Ty2, enable_if_t<_Is_swappable<_Ty1>::value&& _Is_swappable<_Ty2>::value, int> = 0>
    constexpr void swap(pair<_Ty1, _Ty2>& _Left, pair<_Ty1, _Ty2>& _Right) noexcept(noexcept(_Left.swap(_Right))) {
        _Left.swap(_Right);
    }

    #if _HAS_CXX23
    template <class _Ty1, class _Ty2, enable_if_t<is_swappable_v<const _Ty1>&& is_swappable_v<const _Ty2>, int> = 0>
    constexpr void swap(const pair<_Ty1, _Ty2>& _Left, const pair<_Ty1, _Ty2>& _Right) noexcept(
        noexcept(_Left.swap(_Right))) {
        _Left.swap(_Right);
    }
    #endif // _HAS_CXX23

    template <class _Ty1, class _Ty2>
    _NODISCARD constexpr bool operator==(const pair<_Ty1, _Ty2>& _Left, const pair<_Ty1, _Ty2>& _Right) {
        return _Left.first == _Right.first && _Left.second == _Right.second;
    }

    #ifdef __cpp_lib_concepts
    template <class _Ty1, class _Ty2, class _Uty1, class _Uty2, template <class> class _TQual,
        template <class> class _UQual>
        requires requires {
        typename pair<common_reference_t<_TQual<_Ty1>, _UQual<_Uty1>>, common_reference_t<_TQual<_Ty2>, _UQual<_Uty2>>>;
    }
    struct basic_common_reference<pair<_Ty1, _Ty2>, pair<_Uty1, _Uty2>, _TQual, _UQual> {
        using type = pair<common_reference_t<_TQual<_Ty1>, _UQual<_Uty1>>, common_reference_t<_TQual<_Ty2>, _UQual<_Uty2>>>;
    };

    template <class _Ty1, class _Ty2, class _Uty1, class _Uty2>
        requires requires {
        typename pair<common_type_t<_Ty1, _Uty1>, common_type_t<_Ty2, _Uty2>>;
    }
    struct common_type<pair<_Ty1, _Ty2>, pair<_Uty1, _Uty2>> {
        using type = pair<common_type_t<_Ty1, _Uty1>, common_type_t<_Ty2, _Uty2>>;
    };
    #endif // __cpp_lib_concepts

    template <class _Ty>
    struct _Unrefwrap_helper { // leave unchanged if not a reference_wrapper
        using type = _Ty;
    };

    template <class _Ty>
    struct _Unrefwrap_helper<reference_wrapper<_Ty>> { // make a reference from a reference_wrapper
        using type = _Ty&;
    };

    // decay, then unwrap a reference_wrapper
    template <class _Ty>
    using _Unrefwrap_t = typename _Unrefwrap_helper<decay_t<_Ty>>::type;

    template <class _Ty1, class _Ty2>
    _NODISCARD constexpr pair<_Unrefwrap_t<_Ty1>, _Unrefwrap_t<_Ty2>> make_pair(_Ty1&& _Val1, _Ty2&& _Val2) noexcept(
        is_nothrow_constructible_v<_Unrefwrap_t<_Ty1>, _Ty1>&&
        is_nothrow_constructible_v<_Unrefwrap_t<_Ty2>, _Ty2>) /* strengthened */ {
        // return pair composed from arguments
        using _Mypair = pair<_Unrefwrap_t<_Ty1>, _Unrefwrap_t<_Ty2>>;
        return _Mypair(nonstd:: forward<_Ty1>(_Val1), nonstd:: forward<_Ty2>(_Val2));
    }

    namespace rel_ops {
        template <class _Ty>
        _NODISCARD bool operator!=(const _Ty& _Left, const _Ty& _Right) {
            return !(_Left == _Right);
        }

        template <class _Ty>
        _NODISCARD bool operator>(const _Ty& _Left, const _Ty& _Right) {
            return _Right < _Left;
        }

        template <class _Ty>
        _NODISCARD bool operator<=(const _Ty& _Left, const _Ty& _Right) {
            return !(_Right < _Left);
        }

        template <class _Ty>
        _NODISCARD bool operator>=(const _Ty& _Left, const _Ty& _Right) {
            return !(_Left < _Right);
        }
    } // namespace rel_ops

    template <class _Tuple>
    struct tuple_size;

    template <class _Tuple, class = void>
    struct _Tuple_size_sfinae {}; // selected when tuple_size<_Tuple>::value isn't well-formed

    template <class _Tuple>
    struct _Tuple_size_sfinae<_Tuple, void_t<decltype(tuple_size<_Tuple>::value)>>
        : integral_constant<size_t, tuple_size<_Tuple>::value> {}; // selected when tuple_size<_Tuple>::value is well-formed

    template <class _Tuple>
    struct tuple_size<const _Tuple> : _Tuple_size_sfinae<_Tuple> {}; // ignore cv

    template <class _Tuple>
    struct tuple_size<volatile _Tuple> : _Tuple_size_sfinae<_Tuple> {}; // ignore cv

    template <class _Tuple>
    struct tuple_size<const volatile _Tuple> : _Tuple_size_sfinae<_Tuple> {}; // ignore cv

    template <class _Ty>
    _INLINE_VAR constexpr size_t tuple_size_v = tuple_size<_Ty>::value;

    template <size_t _Index, class _Tuple>
    struct tuple_element;

    template <size_t _Index, class _Tuple>
    struct [[msvc::known_semantics]]tuple_element<_Index, const _Tuple> : tuple_element<_Index, _Tuple> {
        using _Mybase = tuple_element<_Index, _Tuple>;
        using type = add_const_t<typename _Mybase::type>;
    };

    template <size_t _Index, class _Tuple>
    struct [[msvc::known_semantics]]tuple_element<_Index, volatile _Tuple>
        : tuple_element<_Index, _Tuple>{
        using _Mybase = tuple_element<_Index, _Tuple>;
        using type = add_volatile_t<typename _Mybase::type>;
    };

    template <size_t _Index, class _Tuple>
    struct [[msvc::known_semantics]]tuple_element<_Index, const volatile _Tuple>
        : tuple_element<_Index, _Tuple>{
        using _Mybase = tuple_element<_Index, _Tuple>;
        using type = add_cv_t<typename _Mybase::type>;
    };

    template <size_t _Index, class _Tuple>
    using tuple_element_t = typename tuple_element<_Index, _Tuple>::type;

    template <class _Ty, size_t _Size>
    class array;

    template <class _Ty, size_t _Size>
    struct tuple_size<array<_Ty, _Size>> : integral_constant<size_t, _Size> {}; // size of array

    template <size_t _Idx, class _Ty, size_t _Size>
    struct [[msvc::known_semantics]]tuple_element<_Idx, array<_Ty, _Size>> {
        static_assert(_Idx < _Size, "array index out of bounds");

        using type = _Ty;
    };

    template <class... _Types>
    struct tuple_size<tuple<_Types...>> : integral_constant<size_t, sizeof...(_Types)> {}; // size of tuple

    template <size_t _Index>
    struct [[msvc::known_semantics]]tuple_element<_Index, tuple<>> { // enforce bounds checking
        static_assert(_Always_false<integral_constant<size_t, _Index>>, "tuple index out of bounds");
    };

    template <class _This, class... _Rest>
    struct [[msvc::known_semantics]]tuple_element<0, tuple<_This, _Rest...>> { // select first element
        using type = _This;
        // MSVC assumes the meaning of _Ttype; remove or rename, but do not change semantics
        using _Ttype = tuple<_This, _Rest...>;
    };

    template <size_t _Index, class _This, class... _Rest>
    struct [[msvc::known_semantics]]tuple_element<_Index, tuple<_This, _Rest...>>
        : tuple_element<_Index - 1, tuple<_Rest...>> {}; // recursive tuple_element definition

    template <class _Ty1, class _Ty2>
    struct tuple_size<pair<_Ty1, _Ty2>> : integral_constant<size_t, 2> {}; // size of pair

    template <size_t _Idx, class _Ty1, class _Ty2>
    struct [[msvc::known_semantics]]tuple_element<_Idx, pair<_Ty1, _Ty2>> {
        static_assert(_Idx < 2, "pair index out of bounds");

        using type = conditional_t<_Idx == 0, _Ty1, _Ty2>;
    };

    template <size_t _Idx, class _Ty1, class _Ty2>
    _NODISCARD constexpr tuple_element_t<_Idx, pair<_Ty1, _Ty2>>& get(pair<_Ty1, _Ty2>& _Pr) noexcept {
        // get reference to element at _Idx in pair _Pr
        if constexpr (_Idx == 0) {
            return _Pr.first;
        }
        else {
            return _Pr.second;
        }
    }

    template <class _Ty1, class _Ty2>
    _NODISCARD constexpr _Ty1& get(pair<_Ty1, _Ty2>& _Pr) noexcept {
        // get reference to element _Ty1 in pair _Pr
        return _Pr.first;
    }

    template <class _Ty2, class _Ty1>
    _NODISCARD constexpr _Ty2& get(pair<_Ty1, _Ty2>& _Pr) noexcept {
        // get reference to element _Ty2 in pair _Pr
        return _Pr.second;
    }

    template <size_t _Idx, class _Ty1, class _Ty2>
    _NODISCARD constexpr const tuple_element_t<_Idx, pair<_Ty1, _Ty2>>& get(const pair<_Ty1, _Ty2>& _Pr) noexcept {
        // get const reference to element at _Idx in pair _Pr
        if constexpr (_Idx == 0) {
            return _Pr.first;
        }
        else {
            return _Pr.second;
        }
    }

    template <class _Ty1, class _Ty2>
    _NODISCARD constexpr const _Ty1& get(const pair<_Ty1, _Ty2>& _Pr) noexcept {
        // get const reference to element _Ty1 in pair _Pr
        return _Pr.first;
    }

    template <class _Ty2, class _Ty1>
    _NODISCARD constexpr const _Ty2& get(const pair<_Ty1, _Ty2>& _Pr) noexcept {
        // get const reference to element _Ty2 in pair _Pr
        return _Pr.second;
    }

    template <size_t _Idx, class _Ty1, class _Ty2>
    _NODISCARD constexpr tuple_element_t<_Idx, pair<_Ty1, _Ty2>>&& get(pair<_Ty1, _Ty2>&& _Pr) noexcept {
        // get rvalue reference to element at _Idx in pair _Pr
        if constexpr (_Idx == 0) {
            return nonstd:: forward<_Ty1>(_Pr.first);
        }
        else {
            return nonstd:: forward<_Ty2>(_Pr.second);
        }
    }

    template <class _Ty1, class _Ty2>
    _NODISCARD constexpr _Ty1&& get(pair<_Ty1, _Ty2>&& _Pr) noexcept {
        // get rvalue reference to element _Ty1 in pair _Pr
        return nonstd:: forward<_Ty1>(_Pr.first);
    }

    template <class _Ty2, class _Ty1>
    _NODISCARD constexpr _Ty2&& get(pair<_Ty1, _Ty2>&& _Pr) noexcept {
        // get rvalue reference to element _Ty2 in pair _Pr
        return nonstd:: forward<_Ty2>(_Pr.second);
    }

    template <size_t _Idx, class _Ty1, class _Ty2>
    _NODISCARD constexpr const tuple_element_t<_Idx, pair<_Ty1, _Ty2>>&& get(const pair<_Ty1, _Ty2>&& _Pr) noexcept {
        // get const rvalue reference to element at _Idx in pair _Pr
        if constexpr (_Idx == 0) {
            return nonstd:: forward<const _Ty1>(_Pr.first);
        }
        else {
            return nonstd:: forward<const _Ty2>(_Pr.second);
        }
    }

    template <class _Ty1, class _Ty2>
    _NODISCARD constexpr const _Ty1&& get(const pair<_Ty1, _Ty2>&& _Pr) noexcept {
        // get const rvalue reference to element _Ty1 in pair _Pr
        return nonstd:: forward<const _Ty1>(_Pr.first);
    }

    template <class _Ty2, class _Ty1>
    _NODISCARD constexpr const _Ty2&& get(const pair<_Ty1, _Ty2>&& _Pr) noexcept {
        // get const rvalue reference to element _Ty2 in pair _Pr
        return nonstd:: forward<const _Ty2>(_Pr.second);
    }

    template <class _Ty, class _Other = _Ty>
    constexpr _Ty exchange(_Ty& _Val, _Other&& _New_val) noexcept(
        conjunction_v<is_nothrow_move_constructible<_Ty>, is_nothrow_assignable<_Ty&, _Other>>) {
        // assign _New_val to _Val, return previous _Val
        _Ty _Old_val = static_cast<_Ty&&>(_Val);
        _Val = static_cast<_Other&&>(_New_val);
        return _Old_val;
    }

    template <class _Ty>
    _NODISCARD constexpr add_const_t<_Ty>& as_const(_Ty& _Val) noexcept { // view _Val through const lenses
        return _Val;
    }

    template <class _Ty>
    void as_const(const _Ty&&) = delete;

    #if _HAS_CXX17
    struct in_place_t { // tag used to select a constructor which initializes a contained object in place
        explicit in_place_t() = default;
    };
    inline constexpr in_place_t in_place{};

    template <class>
    struct in_place_type_t { // tag that selects a type to construct in place
        explicit in_place_type_t() = default;
    };
    template <class _Ty>
    inline constexpr in_place_type_t<_Ty> in_place_type{};

    template <size_t>
    struct in_place_index_t { // tag that selects the index of a type to construct in place
        explicit in_place_index_t() = default;
    };
    template <size_t _Idx>
    inline constexpr in_place_index_t<_Idx> in_place_index{};
    #endif // _HAS_CXX17

    
    template <class _Ty>
    inline constexpr bool _Is_standard_integer = is_integral_v<_Ty>
    && !_Is_any_of_v<remove_cv_t<_Ty>, bool, char,
    #ifdef _NATIVE_WCHAR_T_DEFINED
        wchar_t,
    #endif // _NATIVE_WCHAR_T_DEFINED
    #ifdef __cpp_char8_t
        char8_t,
    #endif // __cpp_char8_t
        char16_t, char32_t>;

    template <class _Ty1, class _Ty2>
    _NODISCARD constexpr bool cmp_equal(const _Ty1 _Left, const _Ty2 _Right) noexcept {
        static_assert(_Is_standard_integer<_Ty1> && _Is_standard_integer<_Ty2>,
            "The integer comparison functions only accept standard and extended integer types.");
        if constexpr (is_signed_v<_Ty1> == is_signed_v<_Ty2>) {
            return _Left == _Right;
        }
        else if constexpr (is_signed_v<_Ty2>) {
            return _Left == static_cast<make_unsigned_t<_Ty2>>(_Right) && _Right >= 0;
        }
        else {
            return static_cast<make_unsigned_t<_Ty1>>(_Left) == _Right && _Left >= 0;
        }
    }

    template <class _Ty1, class _Ty2>
    _NODISCARD constexpr bool cmp_not_equal(const _Ty1 _Left, const _Ty2 _Right) noexcept {
        return !nonstd:: cmp_equal(_Left, _Right);
    }

    template <class _Ty1, class _Ty2>
    _NODISCARD constexpr bool cmp_less(const _Ty1 _Left, const _Ty2 _Right) noexcept {
        static_assert(_Is_standard_integer<_Ty1> && _Is_standard_integer<_Ty2>,
            "The integer comparison functions only accept standard and extended integer types.");
        if constexpr (is_signed_v<_Ty1> == is_signed_v<_Ty2>) {
            return _Left < _Right;
        }
        else if constexpr (is_signed_v<_Ty2>) {
            return _Right > 0 && _Left < static_cast<make_unsigned_t<_Ty2>>(_Right);
        }
        else {
            return _Left < 0 || static_cast<make_unsigned_t<_Ty1>>(_Left) < _Right;
        }
    }

    template <class _Ty1, class _Ty2>
    _NODISCARD constexpr bool cmp_greater(const _Ty1 _Left, const _Ty2 _Right) noexcept {
        return nonstd:: cmp_less(_Right, _Left);
    }

    template <class _Ty1, class _Ty2>
    _NODISCARD constexpr bool cmp_less_equal(const _Ty1 _Left, const _Ty2 _Right) noexcept {
        return !nonstd:: cmp_less(_Right, _Left);
    }

    template <class _Ty1, class _Ty2>
    _NODISCARD constexpr bool cmp_greater_equal(const _Ty1 _Left, const _Ty2 _Right) noexcept {
        return !nonstd:: cmp_less(_Left, _Right);
    }

    template <class _Ty>
    _NODISCARD _CONSTEVAL _Ty _Min_limit() noexcept { // same as (numeric_limits<_Ty>::_min)(), less throughput cost
        static_assert(_Is_standard_integer<_Ty>); // doesn't attempt to handle all types
        if constexpr (is_signed_v<_Ty>) {
            constexpr auto _Unsigned_max = static_cast<make_unsigned_t<_Ty>>(-1);
            return static_cast<_Ty>((_Unsigned_max >> 1) + 1); // well-defined, N4861 [conv.integral]/3
        }
        else {
            return 0;
        }
    }

    template <class _Ty>
    _NODISCARD _CONSTEVAL _Ty _Max_limit() noexcept { // same as (numeric_limits<_Ty>::_max)(), less throughput cost
        static_assert(_Is_standard_integer<_Ty>); // doesn't attempt to handle all types
        if constexpr (is_signed_v<_Ty>) {
            constexpr auto _Unsigned_max = static_cast<make_unsigned_t<_Ty>>(-1);
            return static_cast<_Ty>(_Unsigned_max >> 1);
        }
        else {
            return static_cast<_Ty>(-1);
        }
    }

    template <class _Rx, class _Ty>
    _NODISCARD constexpr bool in_range(const _Ty _Value) noexcept {
        static_assert(_Is_standard_integer<_Rx> && _Is_standard_integer<_Ty>,
            "The integer comparison functions only accept standard and extended integer types.");

        constexpr auto _Ty_min = _Min_limit<_Ty>();
        constexpr auto _Rx_min = _Min_limit<_Rx>();

        if constexpr (nonstd:: cmp_less(_Ty_min, _Rx_min)) {
            if (_Value < _Ty{ _Rx_min }) {
                return false;
            }
        }

        constexpr auto _Ty_max = _Max_limit<_Ty>();
        constexpr auto _Rx_max = _Max_limit<_Rx>();

        if constexpr (nonstd:: cmp_greater(_Ty_max, _Rx_max)) {
            if (_Value > _Ty{ _Rx_max }) {
                return false;
            }
        }

        return true;
    }

    template <class _Ty>
    using _With_reference = _Ty&;

    template <class _Ty>
    concept _Can_reference = requires {
        typename _With_reference<_Ty>;
    };




}