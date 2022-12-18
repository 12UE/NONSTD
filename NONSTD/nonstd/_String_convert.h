#pragma once
namespace nonstd { 
    inline void _Xinvalid_argument(const char* szError) {
        DbgPrint("%s", szError);
    }
    
    inline void _Xout_of_range(const char* szError) {
        DbgPrint("%s", szError);
    }
    
    [[nodiscard]] inline int stoi(const string& _Str, size_t* _Idx = nullptr, int _Base = 10) {
        int& _Errno_ref = errno; // Nonzero cost, pay it once
        const char* _Ptr = _Str.c_str();
        char* _Eptr;
        _Errno_ref = 0;
        const long _Ans =nonstd::strtol(_Ptr, &_Eptr, _Base);

        if (_Ptr == _Eptr) {
            _Xinvalid_argument("invalid stoi argument");
        }

        if (_Errno_ref == ERANGE) {
            _Xout_of_range("stoi argument out of range");
        }

        if (_Idx) {
            *_Idx = static_cast<size_t>(_Eptr - _Ptr);
        }

        return static_cast<int>(_Ans);
    }

    [[nodiscard]] inline long stol(const string& _Str, size_t* _Idx = nullptr, int _Base = 10) {
        int& _Errno_ref = errno; // Nonzero cost, pay it once
        const char* _Ptr = _Str.c_str();
        char* _Eptr;
        _Errno_ref = 0;
        const long _Ans = nonstd::strtol(_Ptr, &_Eptr, _Base);

        if (_Ptr == _Eptr) {
            _Xinvalid_argument("invalid stol argument");
        }

        if (_Errno_ref == ERANGE) {
            _Xout_of_range("stol argument out of range");
        }

        if (_Idx) {
            *_Idx = static_cast<size_t>(_Eptr - _Ptr);
        }

        return _Ans;
    }

    [[nodiscard]] inline unsigned long stoul(const string& _Str, size_t* _Idx = nullptr, int _Base = 10) {
        int& _Errno_ref = errno; // Nonzero cost, pay it once
        const char* _Ptr = _Str.c_str();
        char* _Eptr;
        _Errno_ref = 0;
        const unsigned long _Ans = nonstd::strtoul(_Ptr, &_Eptr, _Base);

        if (_Ptr == _Eptr) {
            _Xinvalid_argument("invalid stoul argument");
        }

        if (_Errno_ref == ERANGE) {
            _Xout_of_range("stoul argument out of range");
        }

        if (_Idx) {
            *_Idx = static_cast<size_t>(_Eptr - _Ptr);
        }

        return _Ans;
    }

    [[nodiscard]] inline long long stoll(const string& _Str, size_t* _Idx = nullptr, int _Base = 10) {
        int& _Errno_ref = errno; // Nonzero cost, pay it once
        const char* _Ptr = _Str.c_str();
        char* _Eptr;
        _Errno_ref = 0;
        const long long _Ans = nonstd::strtoll(_Ptr, &_Eptr, _Base);

        if (_Ptr == _Eptr) {
            _Xinvalid_argument("invalid stoll argument");
        }

        if (_Errno_ref == ERANGE) {
            _Xout_of_range("stoll argument out of range");
        }

        if (_Idx) {
            *_Idx = static_cast<size_t>(_Eptr - _Ptr);
        }

        return _Ans;
    }

    [[nodiscard]] inline unsigned long long stoull(const string& _Str, size_t* _Idx = nullptr, int _Base = 10) {
        int& _Errno_ref = errno; // Nonzero cost, pay it once
        const char* _Ptr = _Str.c_str();
        char* _Eptr;
        _Errno_ref = 0;
        const unsigned long long _Ans = nonstd::strtoull(_Ptr, &_Eptr, _Base);

        if (_Ptr == _Eptr) {
            _Xinvalid_argument("invalid stoull argument");
        }

        if (_Errno_ref == ERANGE) {
            _Xout_of_range("stoull argument out of range");
        }

        if (_Idx) {
            *_Idx = static_cast<size_t>(_Eptr - _Ptr);
        }

        return _Ans;
    }

    [[nodiscard]] inline float stof(const string& _Str, size_t* _Idx = nullptr) {
        int& _Errno_ref = errno; // Nonzero cost, pay it once
        const char* _Ptr = _Str.c_str();
        char* _Eptr;
        _Errno_ref = 0;
        const float _Ans = nonstd::strtof(_Ptr, &_Eptr);

        if (_Ptr == _Eptr) {
            _Xinvalid_argument("invalid stof argument");
        }

        if (_Errno_ref == ERANGE) {
            _Xout_of_range("stof argument out of range");
        }

        if (_Idx) {
            *_Idx = static_cast<size_t>(_Eptr - _Ptr);
        }

        return _Ans;
    }

    [[nodiscard]] inline double stod(const string& _Str, size_t* _Idx = nullptr) {
        int& _Errno_ref = errno; // Nonzero cost, pay it once
        const char* _Ptr = _Str.c_str();
        char* _Eptr;
        _Errno_ref = 0;
        const double _Ans = nonstd::strtod(_Ptr, &_Eptr);

        if (_Ptr == _Eptr) {
            _Xinvalid_argument("invalid stod argument");
        }

        if (_Errno_ref == ERANGE) {
            _Xout_of_range("stod argument out of range");
        }

        if (_Idx) {
            *_Idx = static_cast<size_t>(_Eptr - _Ptr);
        }

        return _Ans;
    }

    [[nodiscard]] inline long double stold(const string& _Str, size_t* _Idx = nullptr) {
        int& _Errno_ref = errno; // Nonzero cost, pay it once
        const char* _Ptr = _Str.c_str();
        char* _Eptr;
        _Errno_ref = 0;
        const long double _Ans = nonstd::strtold(_Ptr, &_Eptr);

        if (_Ptr == _Eptr) {
            _Xinvalid_argument("invalid stold argument");
        }

        if (_Errno_ref == ERANGE) {
            _Xout_of_range("stold argument out of range");
        }

        if (_Idx) {
            *_Idx = static_cast<size_t>(_Eptr - _Ptr);
        }

        return _Ans;
    }

    [[nodiscard]] inline int stoi(const wstring& _Str, size_t* _Idx = nullptr, int _Base = 10) {
        int& _Errno_ref = errno; // Nonzero cost, pay it once
        const wchar_t* _Ptr = _Str.c_str();
        wchar_t* _Eptr;
        _Errno_ref = 0;
        const long _Ans = nonstd::wcstol(_Ptr, &_Eptr, _Base);

        if (_Ptr == _Eptr) {
            _Xinvalid_argument("invalid stoi argument");
        }

        if (_Errno_ref == ERANGE) {
            _Xout_of_range("stoi argument out of range");
        }

        if (_Idx) {
            *_Idx = static_cast<size_t>(_Eptr - _Ptr);
        }

        return static_cast<int>(_Ans);
    }

    [[nodiscard]] inline long stol(const wstring& _Str, size_t* _Idx = nullptr, int _Base = 10) {
        int& _Errno_ref = errno; // Nonzero cost, pay it once
        const wchar_t* _Ptr = _Str.c_str();
        wchar_t* _Eptr;
        _Errno_ref = 0;
        const long _Ans = nonstd::wcstol(_Ptr, &_Eptr, _Base);

        if (_Ptr == _Eptr) {
            _Xinvalid_argument("invalid stol argument");
        }

        if (_Errno_ref == ERANGE) {
            _Xout_of_range("stol argument out of range");
        }

        if (_Idx) {
            *_Idx = static_cast<size_t>(_Eptr - _Ptr);
        }

        return _Ans;
    }

    [[nodiscard]] inline unsigned long stoul(const wstring& _Str, size_t* _Idx = nullptr, int _Base = 10) {
        int& _Errno_ref = errno; // Nonzero cost, pay it once
        const wchar_t* _Ptr = _Str.c_str();
        wchar_t* _Eptr;
        _Errno_ref = 0;
        const unsigned long _Ans = nonstd::wcstoul(_Ptr, &_Eptr, _Base);

        if (_Ptr == _Eptr) {
            _Xinvalid_argument("invalid stoul argument");
        }

        if (_Errno_ref == ERANGE) {
            _Xout_of_range("stoul argument out of range");
        }

        if (_Idx) {
            *_Idx = static_cast<size_t>(_Eptr - _Ptr);
        }

        return _Ans;
    }

    [[nodiscard]] inline long long stoll(const wstring& _Str, size_t* _Idx = nullptr, int _Base = 10) {
        int& _Errno_ref = errno; // Nonzero cost, pay it once
        const wchar_t* _Ptr = _Str.c_str();
        wchar_t* _Eptr;
        _Errno_ref = 0;
        const long long _Ans = nonstd::wcstoll(_Ptr, &_Eptr, _Base);

        if (_Ptr == _Eptr) {
            _Xinvalid_argument("invalid stoll argument");
        }

        if (_Errno_ref == ERANGE) {
            _Xout_of_range("stoll argument out of range");
        }

        if (_Idx) {
            *_Idx = static_cast<size_t>(_Eptr - _Ptr);
        }

        return _Ans;
    }

    [[nodiscard]] inline unsigned long long stoull(const wstring& _Str, size_t* _Idx = nullptr, int _Base = 10) {
        int& _Errno_ref = errno; // Nonzero cost, pay it once
        const wchar_t* _Ptr = _Str.c_str();
        wchar_t* _Eptr;
        _Errno_ref = 0;
        const unsigned long long _Ans = nonstd::wcstoull(_Ptr, &_Eptr, _Base);

        if (_Ptr == _Eptr) {
            _Xinvalid_argument("invalid stoull argument");
        }

        if (_Errno_ref == ERANGE) {
            _Xout_of_range("stoull argument out of range");
        }

        if (_Idx) {
            *_Idx = static_cast<size_t>(_Eptr - _Ptr);
        }

        return _Ans;
    }

    [[nodiscard]] inline float stof(const wstring& _Str, size_t* _Idx = nullptr) {
        int& _Errno_ref = errno; // Nonzero cost, pay it once
        const wchar_t* _Ptr = _Str.c_str();
        wchar_t* _Eptr;
        _Errno_ref = 0;
        const float _Ans = nonstd:: wcstof(_Ptr, &_Eptr);

        if (_Ptr == _Eptr) {
            _Xinvalid_argument("invalid stof argument");
        }

        if (_Errno_ref == ERANGE) {
            _Xout_of_range("stof argument out of range");
        }

        if (_Idx) {
            *_Idx = static_cast<size_t>(_Eptr - _Ptr);
        }

        return _Ans;
    }

    [[nodiscard]] inline double stod(const wstring& _Str, size_t* _Idx = nullptr) {
        int& _Errno_ref = errno; // Nonzero cost, pay it once
        const wchar_t* _Ptr = _Str.c_str();
        wchar_t* _Eptr;
        _Errno_ref = 0;
        const double _Ans = nonstd::wcstod(_Ptr, &_Eptr);

        if (_Ptr == _Eptr) {
            _Xinvalid_argument("invalid stod argument");
        }

        if (_Errno_ref == ERANGE) {
            _Xout_of_range("stod argument out of range");
        }

        if (_Idx) {
            *_Idx = static_cast<size_t>(_Eptr - _Ptr);
        }

        return _Ans;
    }

    [[nodiscard]] inline long double stold(const wstring& _Str, size_t* _Idx = nullptr) {
        int& _Errno_ref = errno; // Nonzero cost, pay it once
        const wchar_t* _Ptr = _Str.c_str();
        wchar_t* _Eptr;
        _Errno_ref = 0;
        const long double _Ans = nonstd::wcstold(_Ptr, &_Eptr);

        if (_Ptr == _Eptr) {
            _Xinvalid_argument("invalid stold argument");
        }

        if (_Errno_ref == ERANGE) {
            _Xout_of_range("stold argument out of range");
        }

        if (_Idx) {
            *_Idx = static_cast<size_t>(_Eptr - _Ptr);
        }

        return _Ans;
    }

    template <class _Elem, class _UTy>
    [[nodiscard]] _Elem* _UIntegral_to_buff(_Elem* _RNext, _UTy _UVal) {
        // format _UVal into buffer *ending at* _RNext
        static_assert(nonstd::is_unsigned_v<_UTy>, "_UTy must be unsigned");

#ifdef _WIN64
        auto _UVal_trunc = _UVal;
#else // ^^^ _WIN64 ^^^ // vvv !_WIN64 vvv

        constexpr bool _Big_uty = sizeof(_UTy) > 4;
        if constexpr (_Big_uty) { // For 64-bit numbers, work in chunks to avoid 64-bit divisions.
            while (_UVal > 0xFFFFFFFFU) {
                auto _UVal_chunk = static_cast<unsigned long>(_UVal % 1000000000);
                _UVal /= 1000000000;

                for (int _Idx = 0; _Idx != 9; ++_Idx) {
                    *--_RNext = static_cast<_Elem>('0' + _UVal_chunk % 10);
                    _UVal_chunk /= 10;
                }
            }
        }

        auto _UVal_trunc = static_cast<unsigned long>(_UVal);
#endif // _WIN64

        do {
            *--_RNext = static_cast<_Elem>('0' + _UVal_trunc % 10);
            _UVal_trunc /= 10;
        } while (_UVal_trunc != 0);
        return _RNext;
    }

    template <class _Elem, class _Ty>
    [[nodiscard]] basic_string<_Elem> _Integral_to_string(const _Ty _Val) {
        // convert _Val to string
        static_assert(nonstd::is_integral_v<_Ty>, "_Ty must be integral");
        using _UTy = nonstd::make_unsigned_t<_Ty>;
        _Elem _Buff[21]{}; // can hold -2^63 and 2^64 - 1, plus NUL
        _Elem* const _Buff_end = _Buff + 20;
        _Elem* _RNext = _Buff_end;
        const auto _UVal = static_cast<_UTy>(_Val);
        if (_Val < 0) {
            _RNext = nonstd::_UIntegral_to_buff(_RNext, 0 - _UVal);
            *--_RNext = '-';
        }else {
            _RNext = nonstd::_UIntegral_to_buff(_RNext, _UVal);
        }

        return basic_string<_Elem>(_RNext, _Buff_end);
    }

    // TRANSITION, CUDA - warning: pointless comparison of unsigned integer with zero
    template <class _Elem, class _Ty>
    [[nodiscard]] basic_string<_Elem> _UIntegral_to_string(const _Ty _Val) {
        // convert _Val to string
        static_assert(nonstd::is_integral_v<_Ty>, "_Ty must be integral");
        static_assert(nonstd::is_unsigned_v<_Ty>, "_Ty must be unsigned");
        _Elem _Buff[21]; // can hold 2^64 - 1, plus NUL
		_Elem* const _Buff_end = _Buff + 20;
        _Elem* const _RNext = nonstd::_UIntegral_to_buff(_Buff_end, _Val);
        return basic_string<_Elem>(_RNext, _Buff_end);
    }

    [[nodiscard]] inline string to_string(int _Val) {
        return _Integral_to_string<char>(_Val);
    }

    [[nodiscard]] inline string to_string(unsigned int _Val) {
        return _UIntegral_to_string<char>(_Val);
    }

    [[nodiscard]] inline string to_string(long _Val) {
        return _Integral_to_string<char>(_Val);
    }

    [[nodiscard]] inline string to_string(unsigned long _Val) {
        return _UIntegral_to_string<char>(_Val);
    }

    [[nodiscard]] inline string to_string(long long _Val) {
        return _Integral_to_string<char>(_Val);
    }

    [[nodiscard]] inline string to_string(unsigned long long _Val) {
        return _UIntegral_to_string<char>(_Val);
    }

    [[nodiscard]] inline string to_string(double _Val) {
        const auto _Len = static_cast<size_t>(::_scprintf("%f", _Val));
        nonstd::string _Str(_Len, '\0');
        ::sprintf_s(_Str.data(), _Len + 1, "%f", _Val);
        return _Str;
    }

    [[nodiscard]] inline string to_string(float _Val) {
        return nonstd::to_string(static_cast<double>(_Val));
    }

    [[nodiscard]] inline string to_string(long double _Val) {
        return nonstd::to_string(static_cast<double>(_Val));
    }

    [[nodiscard]] inline wstring to_wstring(int _Val) {
        return _Integral_to_string<wchar_t>(_Val);
    }

    [[nodiscard]] inline wstring to_wstring(unsigned int _Val) {
        return _UIntegral_to_string<wchar_t>(_Val);
    }

    [[nodiscard]] inline wstring to_wstring(long _Val) {
        return _Integral_to_string<wchar_t>(_Val);
    }

    [[nodiscard]] inline wstring to_wstring(unsigned long _Val) {
        return _UIntegral_to_string<wchar_t>(_Val);
    }

    [[nodiscard]] inline wstring to_wstring(long long _Val) {
        return _Integral_to_string<wchar_t>(_Val);
    }

    [[nodiscard]] inline wstring to_wstring(unsigned long long _Val) {
        return _UIntegral_to_string<wchar_t>(_Val);
    }

    [[nodiscard]] inline wstring to_wstring(double _Val) {
        const auto _Len = static_cast<size_t>(_scwprintf(L"%f", _Val));
        wstring _Str(_Len, L'\0');
        swprintf_s(_Str.data(), _Len + 1, L"%f", _Val);
        return _Str;
    }

    [[nodiscard]] inline wstring to_wstring(float _Val) {
        return nonstd::to_wstring(static_cast<double>(_Val));
    }

    [[nodiscard]] inline wstring to_wstring(long double _Val) {
        return nonstd::to_wstring(static_cast<double>(_Val));
    }
}