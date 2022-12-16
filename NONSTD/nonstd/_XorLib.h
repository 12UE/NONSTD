namespace nonstd {
#define __TIMESTAMP "Fri Dec 16 20:36:24 2022"
	namespace detail {
		template<size_t S>struct unsigned_;
		template<>struct unsigned_<1> { using type = uint8_t; };
		template<>struct unsigned_<2> { using type = uint16_t; };
		template<>struct unsigned_<4> { using type = uint32_t; };
		template<auto C, auto...>struct pack_value_type { using type = decltype(C); };
		template<size_t Size> constexpr size_t _buffer_size() { return ((Size / 16) + (Size % 16 != 0)) * 2; }
		template<auto... Cs> struct tstring_ {
			using value_type = typename pack_value_type<Cs...>::type;
			constexpr static size_t size = sizeof...(Cs);
			constexpr static value_type  str[size] = { Cs... };
			constexpr static size_t buffer_size = _buffer_size<sizeof(str)>();
			constexpr static size_t buffer_align = ((sizeof(str) > 16) ? 32 : 16);
		};
		template<size_t I, uint64_t K> struct _ki {
			constexpr static size_t   idx = I;
			constexpr static uint64_t key = K;
		};
		template<uint32_t Seed> constexpr uint32_t key4() noexcept {
			uint32_t value = Seed;
			for (char c : __TIMESTAMP)
				value = static_cast<uint32_t>((value ^ c) * 78920519ull);
			return value;
		}
		template<size_t S> constexpr uint64_t key8() {
			constexpr auto first_part = key4<4784258102 + S>();
			constexpr auto second_part = key4<first_part>();
			return (static_cast<uint64_t>(first_part) << 32) | second_part;
		}
		template<class T, uint64_t... Keys> struct string_storage {
			uint64_t storage[T::buffer_size];
			__forceinline constexpr string_storage() noexcept : storage{ Keys... } {
				using cast_type = typename unsigned_<sizeof(typename T::value_type)>::type;
				constexpr auto value_size = sizeof(typename T::value_type);
				for (size_t i = 0; i < T::size; ++i) {
					storage[i / (8 / value_size)] ^= (uint64_t{ static_cast<cast_type>(T::str[i]) } << ((i % (8 / value_size)) * 8 * value_size));
				}
			}
		};
	}
	template<class T, class... Keys>
	class xor_string {
		alignas(T::buffer_align) uint64_t _storage[T::buffer_size];
		__forceinline void _crypt_256_single(const uint64_t* keys, uint64_t* storage) noexcept {
			for (int i = 0; i < 4; ++i) {
				storage[i] ^= keys[i];
			}
		}
		template<size_t... Idxs>
		__forceinline void _crypt_256(const uint64_t* keys, index_sequence<Idxs...>) noexcept {
			(_crypt_256_single(keys + Idxs * 4, _storage + Idxs * 4), ...);
		}
		__forceinline void _crypt_128_single(const uint64_t* keys, uint64_t* storage) noexcept {
			for (int i = 0; i < 2; ++i) {
				storage[i] ^= keys[i];
			}
		}
		template<size_t... Idxs>
		__forceinline void _crypt_128(const uint64_t* keys, index_sequence<Idxs...>) noexcept {
			(_crypt_128_single(keys + Idxs * 2, _storage + Idxs * 2), ...);
		}
		__forceinline constexpr void _copy() noexcept {
			constexpr detail::string_storage<T, Keys::key...> storage{};
			static_cast<void>(initializer_list<uint64_t>{(const_cast<uint64_t*>(_storage))[Keys::idx] = storage.storage[Keys::idx]... });
		}
	public:
		using value_type = typename T::value_type;
		using size_type = size_t;
		using pointer = value_type*;
		using const_pointer = const pointer;
		__forceinline xor_string() noexcept { _copy(); }
		__forceinline constexpr size_type size() const noexcept { return T::size - 1; }
		__forceinline void crypt() noexcept {
			alignas(T::buffer_align) uint64_t keys[T::buffer_size]{};
			static_cast<void>(nonstd::initializer_list<nonstd::uint64_t>{( const_cast<nonstd::uint64_t*>(keys) )[Keys::idx] = Keys::key... });
			_copy();
			_crypt_256(keys, make_index_sequence<T::buffer_size / 4>{});
			if constexpr (T::buffer_size % 4 != 0) _crypt_128(keys, index_sequence<T::buffer_size / 2 - 1>{});
		}
		__forceinline const_pointer get() const noexcept { return reinterpret_cast<const_pointer>(_storage); }
		__forceinline const_pointer crypt_get() noexcept {
			crypt();
			return reinterpret_cast<const_pointer>(_storage);
		}
	};
	template<class Tstr, size_t... StringIndices, size_t... KeyIndices>
	__forceinline constexpr auto make_xorstr(Tstr str_lambda, index_sequence<StringIndices...>, index_sequence<KeyIndices...>) noexcept {
		return xor_string<detail::tstring_<str_lambda()[StringIndices]...>,
			detail::_ki<KeyIndices, detail::key8<KeyIndices>()>...>{};
	}

}
#define xor_str(str)                                             \
		nonstd::make_xorstr([&]() { return str; },               \
        nonstd::make_index_sequence<sizeof(str)/sizeof(*str)>{},    \
        nonstd::make_index_sequence<nonstd::detail::_buffer_size<sizeof(str)>()>{}).crypt_get()
#undef __TIMESTAMP