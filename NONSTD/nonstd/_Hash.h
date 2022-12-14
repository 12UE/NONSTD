#pragma once
namespace nonstd {
	template <typename ResultT, ResultT OffsetBasis, ResultT Prime>
	class basic_fnv1a final {
	public:
		static_assert(nonstd::is_unsigned<ResultT>::value, "need unsigned integer");
		using result_type = ResultT;
		result_type state_{};
		constexpr basic_fnv1a() noexcept : state_{ OffsetBasis } {}
		constexpr void update(const void* const data, const nonstd::size_t size) noexcept {
			const auto cdata = static_cast<const unsigned char*>(data);
			auto acc = this->state_;
			for (auto i = nonstd::size_t{}; i < size; ++i) {
				const auto next = nonstd::size_t{ cdata[i] };
				acc = (acc ^ next) * Prime;
			}
			this->state_ = acc;
		}
		constexpr result_type digest() const noexcept { return this->state_; }
	};
	template <nonstd::size_t Bits>struct fnv1a;
	template <>struct fnv1a<32> { using type = basic_fnv1a<nonstd::uint32_t, UINT32_C(2166136261), UINT32_C(16777619)>; };
	template <>struct fnv1a<64> { using type = basic_fnv1a<nonstd::uint64_t, UINT64_C(14695981039346656037), UINT64_C(1099511628211)>; };
	template <nonstd::size_t Bits>
	using fnv1a_t = typename fnv1a<Bits>::type;
	constexpr nonstd::size_t hash_bytes(const void* const data, const nonstd::size_t size) noexcept {
		auto hashfn = fnv1a_t<CHAR_BIT * sizeof(nonstd::size_t)>{};
		hashfn.update(data, size);
		return hashfn.digest();
		
	}
}