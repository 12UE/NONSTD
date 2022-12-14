#pragma once

namespace nonstd {
	//ÔËËãÆ÷
	//plus
	template<class T>
	struct plus {
		using value_type = T;
		_NODISCARD _CONSTEXPR T operator()(T& x, T& y) {
			return x + y;
		}
	};
	//minus
	template<class T>
	struct minus {
		using value_type = T;
		_NODISCARD _CONSTEXPR T operator()(T& x, T& y) {
			return x - y;
		}
	};
	//multiplies
	template<class T>
	struct multiplies {
		using value_type = T;
		_NODISCARD _CONSTEXPR T operator()(T& x, T& y) {
			return x * y;
		}
	};
	//divides
	template<class T>
	struct divides {
		using value_type = T;
		_NODISCARD _CONSTEXPR T operator()(T& x, T& y) {
			return x / y;
		}
	};
	//modulus
	template<class T>
	struct modulus {
		using value_type = T;
		_NODISCARD _CONSTEXPR T operator()(T& x, T& y) {
			return x % y;
		}
	};
	//negate
	template<class T>
	struct negate {
		using value_type = T;
		_NODISCARD _CONSTEXPR T operator()(T& x) {
			return -x;
		}
	};

	//logical_and
	template<class T>
	struct logical_and {
		using value_type = T;
		_NODISCARD _CONSTEXPR bool operator()(T& x, T& y) {
			return x && y;
		}
	};
	//logical_or
	template<class T>
	struct logical_or {
		using value_type = T;
		_NODISCARD _CONSTEXPR bool operator()(T& x, T& y) {
			return x || y;
		}
	};
	//logical_not
	template<class T>
	struct logical_not {
		using value_type = T;
		_NODISCARD _CONSTEXPR bool operator()(T& x) {
			return !x;
		}
	};

	//±È½ÏÆ÷
	template<class T>
	struct less {
		using value_type = T;
		_NODISCARD _CONSTEXPR bool operator()(T& x, T& y) {
			return x < y;
		}
	};
	//less_equal
	template<class T>
	struct less_equal {
		using value_type = T;
		_NODISCARD _CONSTEXPR bool operator()(T& x, T& y) {
			return x <= y;
		}
	};

	template<class T>
	struct greater {
		using value_type = T;
		_NODISCARD _CONSTEXPR bool operator()(T& x, T& y) {
			return x > y;
		}
	};

	template<class T>
	struct greater_equal {
		using value_type = T;
		_NODISCARD _CONSTEXPR bool operator()(T& x, T& y) {
			return x >= y;
		}
	};
	//not_equal_to
	template<class T>
	struct not_equal_to {
		using value_type = T;
		_NODISCARD _CONSTEXPR bool operator()(T& x, T& y) {
			return x != y;
		}
	};
	//equal_to
	template<class T>
	struct equal_to {
		using value_type = T;
		_NODISCARD _CONSTEXPR bool operator()(T& x, T& y) {
			return x == y;
		}
	};
	//identity
	template<class T>
	struct identity {
		using value_type = T;
		_NODISCARD _CONSTEXPR T& operator()(T& x) {
			return x;
		}
	};
	//select1st
	template<class T>
	struct select1st {
		using value_type = T;
		_NODISCARD _CONSTEXPR T& operator()(T& x) {
			return x.first;
		}
	};
	//select2nd
	template<class T>
	struct select2nd {
		using value_type = T;
		_NODISCARD _CONSTEXPR T& operator()(T& x) {
			return x.second;
		}
	};
	_INLINE int __cdecl toupper(_In_ int _C) {
		return _C - 'a' + 'A';
	}
	_INLINE int __cdecl tolower(_In_ int _C) {
		return _C - 'A' + 'a';
	}
	_INLINE int _cdecl towlower(wchar_t _C) {
		return _C - L'A' + L'a';
	}
	_INLINE int _cdecl towupper(wchar_t _C) {
		return _C - L'a' + L'A';
	}
	_INLINE int __cdecl isupper(_In_ int _C) {
		return _C >= 'A' && _C <= 'Z';
	}
	_INLINE int __cdecl islower(_In_ int _C) {
		return _C >= 'a' && _C <= 'z';
	}
	_INLINE int __cdecl isalpha(_In_ int _C) {
		return isupper(_C) || islower(_C);
	}
	_INLINE int __cdecl isdigit(_In_ int _C) {
		return _C >= '0' && _C <= '9';
	}
	_INLINE int __cdecl isalnum(_In_ int _C) {
		return isalpha(_C) || isdigit(_C);
	}
	_INLINE int __cdecl isxdigit(_In_ int _C) {
		return isdigit(_C) || (_C >= 'A' && _C <= 'F') || (_C >= 'a' && _C <= 'f');
	}
	_INLINE int __cdecl isspace(_In_ int _C) {
		return _C == ' ' || _C == '\t' || _C == '\n' || _C == '\r' || _C == '\f' || _C == '\v';
	}
	_INLINE int __cdecl ispunct(_In_ int _C) {
		return isprint(_C) && !isspace(_C) && !isalnum(_C);
	}
	_INLINE int __cdecl isprint(_In_ int _C) {
		return _C >= ' ' && _C <= '~';
	}
	_INLINE int __cdecl isgraph(_In_ int _C) {
		return _C > ' ' && _C <= '~';
	}
	//·Âº¯Êý to_upper
	template<class T>
	struct to_upper {
		using value_type = T;
		_NODISCARD _CONSTEXPR T operator()(T& x) {
			//ÅÐ¶ÏÊÇ¿í×Ö·û»¹ÊÇÕ­×Ö·û
			if constexpr (nonstd::is_same_v<T, char>) {
				return (T)nonstd::toupper(x);
			}
			else if constexpr (nonstd::is_same_v<T, wchar_t>) {
				return (T)nonstd::towupper(x);
			}
			else if constexpr (nonstd::is_same_v<T, char16_t>) {
				return (T)nonstd::towupper(x);
			}
			else if constexpr (nonstd::is_same_v<T, char32_t>) {
				return (T)nonstd::towupper(x);
			}else{
				return nonstd::toupper(x);
			}
	
		}
	};
	//·Âº¯Êý to_lower
	template<class T>
	struct to_lower {
		using value_type = T;
		_NODISCARD _CONSTEXPR T operator()(T& x) {
			//ÅÐ¶ÏÊÇ¿í×Ö·û»¹ÊÇÕ­×Ö·û
			if constexpr (nonstd::is_same_v<T, char>) {
				return nonstd::tolower(x);
			}
			else if constexpr (nonstd::is_same_v<T, wchar_t>) {
				return nonstd::towlower(x);
			}
			else if constexpr (nonstd::is_same_v<T, char16_t>) {
				return nonstd::towlower(x);
			}
			else if constexpr (nonstd::is_same_v<T, char32_t>) {
				return nonstd::towlower(x);
			}
			else {
				return nonstd::tolower(x);
			}
		}
	};

	template <class T = int, class F, class... Args> [[nodiscard]] decltype(auto) _STDCall(F f, Args...args) {
		return(f) ? reinterpret_cast<T(__stdcall*)(Args...)>(f)(args...) : T();//_nonstd::call
	}
	template <class T = int, class F, class... Args> [[nodiscard]] decltype(auto) _CdeclCall(F f, Args...args) {
		return(f) ? reinterpret_cast<T(__cdecl*)(Args...)>(f)(args...) : T();//_nonstd::call
	}
	template <class T = int, class F, class... Args> [[nodiscard]] decltype(auto) _FastCall(F f, Args...args) {
		return(f) ? reinterpret_cast<T(__fastcall*)(Args...)>(f)(args...) : T();//_nonstd::call
	}
	template <class T = int, class F, class... Args> [[nodiscard]] decltype(auto) _ThisCall(F f, Args...args) {
		return(f) ? reinterpret_cast<T(__thiscall*)(Args...)>(f)(args...) : T();//_nonstd::call
	}
	template <class T = int, class F, class... Args> [[nodiscard]] decltype(auto) _VectorCall(F f, Args...args) {
		return(f) ? reinterpret_cast<T(__vectorcall*)(Args...)>(f)(args...) : T();//_nonstd::call
	}
	
	template<class _Ty,class _Pr>
	constexpr _Ty _max(nonstd::initializer_list<_Ty> _list, _Pr bin) {
		_Ty _max = *_list.begin();
		for (auto _it = _list.begin(); _it != _list.end(); ++_it) {
			if (bin(_max, *_it)) {
				_max = *_it;
			}
		}
		return _max;
	}
	template <class _Ty>
	constexpr _Ty _max(nonstd::initializer_list<_Ty> _list) {
		return _max(_list, [](const _Ty& _Left, const _Ty& _Right) {return _Left < _Right; });
	}
	template<class _Ty, class _Pr>
	constexpr _Ty _min(nonstd::initializer_list<_Ty> _list, _Pr bin) {
		_Ty _min = *_list.begin();
		for (auto _it = _list.begin(); _it != _list.end(); ++_it) {
			if (bin(_min, *_it)) {
				_min = *_it;
			}
		}
		return _min;
	}
	template <class _Ty>
	constexpr _Ty _min(nonstd::initializer_list<_Ty> _list) {
		return _min(_list, [](const _Ty& _Left, const _Ty& _Right) {return _Left < _Right; });
	}
}