#pragma once

namespace nonstd {
    //字符处理
    _INLINE long int strtol(const char* str, char** endptr, int base) {
        // Check for invalid input
        if (str == NULL || (base != 0 && (base < 2 || base > 36))) {
            errno = EINVAL;
            return 0;
        }

        // Skip leading white space
        while (isspace(*str)) {
            str++;
        }

        // Check for optional sign
        int sign = 1;
        if (*str == '+') {
            str++;
        }
        else if (*str == '-') {
            sign = -1;
            str++;
        }

        // Check for base specification (leading 0x or 0X)
        if (base == 0 && *str == '0') {
            str++;
            if (*str == 'x' || *str == 'X') {
                base = 16;
                str++;
            }
            else {
                base = 8;
            }
        }

        // If no base was specified, use base 10
        if (base == 0) {
            base = 10;
        }

        // Accumulate result
        long int result = 0;
        while (isalnum(*str)) {
            // Convert character to digit
            int digit;
            if (isdigit(*str)) {
                digit = *str - '0';
            }
            else if (isupper(*str)) {
                digit = *str - 'A' + 10;
            }
            else {
                digit = *str - 'a' + 10;
            }

            // Check for invalid digit
            if (digit >= base) {
                break;
            }

            // Add digit to result
            result = result * base + digit;

            // Check for overflow
            if (result > LONG_MAX) {
                errno = ERANGE;
                return LONG_MAX;
            }
            else if (result < LONG_MIN) {
                errno = ERANGE;
                return LONG_MIN;
            }

            // Move to next character
            str++;
        }

        // Set end pointer
        if (endptr) {
            *endptr = (char*)str;
        }

        // Apply sign and return result
        return sign * result;
    }
    _INLINE int chartodigit(char c)
    {
        // 如果字符c是数字，则将其转换为数字并返回
        if (c >= '0' && c <= '9')
            return c - '0';

        // 如果字符c是大写字母，则将其转换为对应的数字并返回
        if (c >= 'A' && c <= 'Z')
            return c - 'A' + 10;

        // 如果字符c是小写字母，则将其转换为对应的数字并返回
        if (c >= 'a' && c <= 'z')
            return c - 'a' + 10;

        // 如果字符c既不是数字，也不是字母，则返回-1
        return -1;
    }
    _INLINE bool isvalid(char c, int base)
    {
        // 如果base小于2或大于36，则返回false
        if (base < 2 || base > 36)
            return false;

        // 如果字符c是数字，则返回true
        if (c >= '0' && c <= '9')
            return true;

        // 如果字符c是大写字母，并且它的值不大于base，则返回true
        if (c >= 'A' && c <= 'Z' && c - 'A' + 10 <= base)
            return true;

        // 如果字符c是小写字母，并且它的值不大于base，则返回true
        if (c >= 'a' && c <= 'z' && c - 'a' + 10 <= base)
            return true;

        // 如果字符c既不是数字，也不是字母，或者它的值大于base，则返回false
        return false;
    }
    _INLINE unsigned long strtoul(const char* str, char** endptr, int base)//endptr是
    {
        // 首先检查参数是否合法
        if (str == NULL || (base != 0 && (base < 2 || base > 36)))
        {
            errno = EINVAL;
            return 0;
        }

        // 初始化转换结果为0
        unsigned long result = 0;

        // 循环遍历字符串中的每个字符
        for (const char* p = str; *p != '\0'; ++p)
        {
            // 如果当前字符是空白字符，则跳过它
            if (isspace(*p))
                continue;

            // 如果当前字符是前导0，则看看下一个字符是否是x或X，如果是，则说明该数是16进制
            if (*p == '0' && (*(p + 1) == 'x' || *(p + 1) == 'X'))
            {
                base = 16;
                ++p;
            }

            // 如果指定了进制，则检查当前字符是否合法
            if (base != 0 && !isvalid(*p, base))
            {
                errno = EINVAL;
                return 0;
            }

            // 将当前字符转换为数字，并将它与原结果相乘并累加
            result = result * base + chartodigit(*p);
        }

        // 如果指定了endptr参数，则将其设置为指向字符串结尾的指
        if (endptr != NULL)
            *endptr = (char*)str + strlen(str);

        // 返回转换结果
        return result;
    }
    _INLINE long long strtoll(const char* str, char** endptr, int base)
    {
        // Skip leading whitespace
        while (*str && isspace(*str))
            str++;

        // Check for sign
        int sign = 1;
        if (*str == '+') {
            str++;
        }
        else if (*str == '-') {
            str++;
            sign = -1;
        }

        // Determine base
        if (base == 0) {
            if (*str == '0') {
                if (tolower(*(str + 1)) == 'x') {
                    base = 16;
                    str += 2;
                }
                else {
                    base = 8;
                    str++;
                }
            }
            else {
                base = 10;
            }
        }

        // Convert string to long long integer
        long long result = 0;
        while (*str && isalnum(*str)) {
            int digit;
            if (isdigit(*str)) {
                digit = *str - '0';
            }
            else {
                digit = tolower(*str) - 'a' + 10;
            }
            if (digit >= base) {
                break;
            }
            result = result * base + digit;
            str++;
        }

        // Set endptr if provided
        if (endptr) {
            *endptr = (char*)str;
        }

        return result * sign;
    }
    _INLINE unsigned long long strtoull(const char* str, char** endptr, int base)
    {
        // Skip leading whitespace
        while (*str && isspace(*str))
            str++;

        // Determine base
        if (base == 0) {
            if (*str == '0') {
                if (tolower(*(str + 1)) == 'x') {
                    base = 16;
                    str += 2;
                }
                else {
                    base = 8;
                    str++;
                }
            }
            else {
                base = 10;
            }
        }

        // Convert string to unsigned long long integer
        unsigned long long result = 0;
        while (*str && isalnum(*str)) {
            int digit;
            if (isdigit(*str)) {
                digit = *str - '0';
            }
            else {
                digit = tolower(*str) - 'a' + 10;
            }
            if (digit >= base) {
                break;
            }
            result = result * base + digit;
            str++;
        }

        // Set endptr if provided
        if (endptr) {
            *endptr = (char*)str;
        }

        return result;
    }
    _INLINE double atof(const char* str) {
        double value = 0.0;
        int sign = 1;

        // 跳过空格
        while (*str == ' ') {
            str++;
        }

        // 处理正负号
        if (*str == '-') {
            sign = -1;
            str++;
        }
        else if (*str == '+') {
            str++;
        }

        // 计算整数部分
        while (*str >= '0' && *str <= '9') {
            value = value * 10 + (*str - '0');
            str++;
        }

        // 处理小数部分
        if (*str == '.') {
            double fraction = 0.1;
            str++;

            while (*str >= '0' && *str <= '9') {
                value += (*str - '0') * fraction;
                fraction /= 10;
                str++;
            }
        }

        return sign * value;
    }
    _INLINE float strtof(const char* str, char** endptr) {
        // 跳过前导空格
        while (*str == ' ') {
            str++;
        }

        // 如果字符串为空或者不包含数字，返回0
        if (*str == '\0' || !isdigit(*str)) {
            *endptr = (char*)str;
            return 0;
        }

        // 找到数字部分
        const char* start = str;
        while (*str != '\0' && isdigit(*str)) {
            str++;
        }

        // 将字符串转换为浮点数并返回
        *endptr = (char*)str;
        return (float)atof(start);
    }
    _INLINE double strtod(const char* nptr, char** endptr) {
        double value = 0.0;
        int sign = 1;

        // 跳过空格
        while (*nptr == ' ') {
            nptr++;
        }

        // 处理正负号
        if (*nptr == '-') {
            sign = -1;
            nptr++;
        }
        else if (*nptr == '+') {
            nptr++;
        }

        // 计算整数部分
        while (*nptr >= '0' && *nptr <= '9') {
            value = value * 10 + (*nptr - '0');
            nptr++;
        }

        // 处理小数部分
        if (*nptr == '.') {
            double fraction = 0.1;
            nptr++;

            while (*nptr >= '0' && *nptr <= '9') {
                value += (*nptr - '0') * fraction;
                fraction /= 10;
                nptr++;
            }
        }

        // 处理科学计数法
        if (*nptr == 'e' || *nptr == 'E') {
            int exponent = 0;
            int exponent_sign = 1;
            nptr++;

            if (*nptr == '-') {
                exponent_sign = -1;
                nptr++;
            }
            else if (*nptr == '+') {
                nptr++;
            }

            while (*nptr >= '0' && *nptr <= '9') {
                exponent = exponent * 10 + (*nptr - '0');
                nptr++;
            }

            exponent *= exponent_sign;
            while (exponent > 0) {
                value *= 10;
                exponent--;
            }
            while (exponent < 0) {
                value /= 10;
                exponent++;
            }
        }

        if (endptr) {
            *endptr = (char*)nptr;
        }

        return sign * value;
    }
    _INLINE long double strtold(const char* nptr, char** endptr) {
        long double value = 0.0;
        int sign = 1;

        // 跳过空格
        while (*nptr == ' ') {
            nptr++;
        }

        // 处理正负号
        if (*nptr == '-') {
            sign = -1;
            nptr++;
        }
        else if (*nptr == '+') {
            nptr++;
        }

        // 计算整数部分
        while (*nptr >= '0' && *nptr <= '9') {
            value = value * 10 + (*nptr - '0');
            nptr++;
        }

        // 处理小数部分
        if (*nptr == '.') {
            long double fraction = 0.1;
            nptr++;

            while (*nptr >= '0' && *nptr <= '9') {
                value += (*nptr - '0') * fraction;
                fraction /= 10;
                nptr++;
            }
        }

        // 处理科学计数法
        if (*nptr == 'e' || *nptr == 'E') {
            int exponent = 0;
            int exponent_sign = 1;
            nptr++;

            if (*nptr == '-') {
                exponent_sign = -1;
                nptr++;
            }
            else if (*nptr == '+') {
                nptr++;
            }

            while (*nptr >= '0' && *nptr <= '9') {
                exponent = exponent * 10 + (*nptr - '0');
                nptr++;
            }

            exponent *= exponent_sign;
            while (exponent > 0) {
                value *= 10;
                exponent--;
            }
            while (exponent < 0) {
                value /= 10;
                exponent++;
            }
        }

        if (endptr) {
            *endptr = (char*)nptr;
        }

        return sign * value;
    }
    _INLINE long int wcstol(const wchar_t* nptr, wchar_t** endptr, int base)
    {
        // skip leading whitespace
        while (iswspace(*nptr))
            nptr++;

        // check for optional sign
        int sign = 1;
        if (*nptr == L'-')
        {
            sign = -1;
            nptr++;
        }
        else if (*nptr == L'+')
            nptr++;

        // check for optional base prefix (0x or 0X for hexadecimal, 0 for octal)
        if (base == 0)
        {
            if (nptr[0] == L'0' && (nptr[1] == L'x' || nptr[1] == L'X'))
            {
                base = 16;
                nptr += 2;
            }
            else if (nptr[0] == L'0')
                base = 8;
            else
                base = 10;
        }

        // check for invalid base
        if (base < 2 || base > 36)
        {
            errno = EINVAL;
            if (endptr)
                *endptr = (wchar_t*)nptr;
            return 0;
        }

        // convert the string to a long integer
        long int result = 0;
        while (iswalnum(*nptr))
        {
            int digit;
            if (iswdigit(*nptr))
                digit = *nptr - L'0';
            else
                digit = towlower(*nptr) - L'a' + 10;
            if (digit >= base)
                break;
            result = result * base + digit;
            nptr++;
        }

        // check for overflow
        if (result > LONG_MAX || (sign == -1 && result > (unsigned long)LONG_MAX + 1))
        {
            errno = ERANGE;
            result = LONG_MAX;
        }
        else if (result < LONG_MIN)
        {
            errno = ERANGE;
            result = LONG_MIN;
        }
        else
            result *= sign;

        // store the end pointer and return the result
        if (endptr)
            *endptr = (wchar_t*)nptr;
        return result;
    }
    _INLINE unsigned long wcstoul(const wchar_t* str, wchar_t** endptr, int base) {
        // Skip leading whitespace characters
        while (iswspace(*str)) {
            str++;
        }

        // Set the end pointer to the current position if it is non-null
        if (endptr) {
            *endptr = (wchar_t*)str;
        }

        // If the base is 0, determine the base from the string
        if (base == 0) {
            if (*str == L'0') {
                str++;
                if (*str == L'x' || *str == L'X') {
                    str++;
                    base = 16;
                }
                else {
                    base = 8;
                }
            }
            else {
                base = 10;
            }
        }

        // Parse the string as an unsigned long integer with the specified base
        unsigned long n = 0;
        while (*str) {
            wchar_t c = *str;
            int digit = 0;
            if (iswdigit(c)) {
                digit = c - L'0';
            }
            else if (iswalpha(c)) {
                digit = towupper(c) - L'A' + 10;
            }
            if (digit < 0 || digit >= base) {
                break;
            }
            n = n * base + digit;
            str++;
        }

        // Set the end pointer to the first unconverted character if it is non-null
        if (endptr) {
            *endptr = (wchar_t*)str;
        }

        return n;
    }
    _INLINE long long wcstoll(const wchar_t* str, wchar_t** endptr, int base)
    {
        // skip leading whitespace
        while (iswspace(*str)) str++;

        // determine sign of number
        int sign = 1;
        if (*str == L'-')
        {
            sign = -1;
            str++;
        }
        else if (*str == L'+')
        {
            str++;
        }

        // convert digits to long long integer
        long long num = 0;
        while (iswdigit(*str))
        {
            // check for overflow
            if (num > LLONG_MAX / base || (num == LLONG_MAX / base && *str - L'0' > LLONG_MAX % base))
            {
                num = LLONG_MAX;
                errno = ERANGE;
                break;
            }

            num = num * base + (*str - L'0');
            str++;
        }

        // set endptr to point to the character that stopped the conversion
        if (endptr) *endptr = (wchar_t*)str;

        // return the converted number
        return sign * num;
    }
    _INLINE long double wcstold(const wchar_t* str, wchar_t** endptr) {
        // Skip leading whitespace characters
        while (iswspace(*str)) {
            str++;
        }

        // Set the end pointer to the current position if it is non-null
        if (endptr) {
            *endptr = (wchar_t*)str;
        }

        // Parse the sign of the floating-point number
        int sign = 1;
        if (*str == L'-') {
            sign = -1;
            str++;
        }
        else if (*str == L'+') {
            str++;
        }

        // Parse the integer part of the floating-point number
        long long int integer_part = 0;
        while (*str) {
            wchar_t c = *str;
            if (!iswdigit(c)) {
                break;
            }
            integer_part = integer_part * 10 + (c - L'0');
            str++;
        }

        // Parse the fractional part of the floating-point number
        long double fractional_part = 0.0;
        if (*str == L'.') {
            str++;
            long double factor = 0.1;
            while (*str) {
                wchar_t c = *str;
                if (!iswdigit(c)) {
                    break;
                }
                fractional_part += (c - L'0') * factor;
                factor *= 0.1;
                str++;
            }
        }

        // Parse the exponent of the floating-point number
        long double exponent = 0.0;
        if (*str == L'e' || *str == L'E') {
            str++;
            int exponent_sign = 1;
            if (*str == L'-') {
                exponent_sign = -1;
                str++;
            }
            else if (*str == L'+') {
                str++;
            }
            long long int exponent_part = 0;
            while (*str) {
                wchar_t c = *str;
                if (!iswdigit(c)) {
                    break;
                }
                exponent_part = exponent_part * 10 + (c - L'0');
                str++;
            }
            exponent = pow(10, exponent_part * exponent_sign);
        }
        // Set the end pointer to the current position if it is non-null
        if (endptr) {
            *endptr = (wchar_t*)str;
        }

        // Return the floating-point number as a long double
        return sign * (integer_part + fractional_part) * exponent;
    }
    _INLINE unsigned long long wcstoull(const wchar_t* nptr, wchar_t** endptr, int base) {
        unsigned long long result = 0;
        const wchar_t* p = nptr;

        // Skip leading whitespace
        while (iswspace(*p)) p++;

        // Check for optional sign
        int sign = 1;
        if (*p == L'-') {
            sign = -1;
            p++;
        }
        else if (*p == L'+') {
            p++;
        }

        // Check for optional base specifier
        if (*p == L'0') {
            p++;
            if (*p == L'x' || *p == L'X') {
                // Hexadecimal
                if (base == 0) base = 16;
                p++;
            }
            else if (*p == L'b' || *p == L'B') {
                // Binary
                if (base == 0) base = 2;
                p++;
            }
            else {
                // Octal
                if (base == 0) base = 8;
            }
        }
        else if (base == 0) {
            // No base specifier, default to base 10
            base = 10;
        }

        // Convert digits
        while (iswalnum(*p)) {
            // Compute digit value
            int digit;
            if (iswdigit(*p)) {
                digit = *p - L'0';
            }
            else if (iswalpha(*p)) {
                digit = towlower(*p) - L'a' + 10;
            }
            else {
                // Invalid character, break out of loop
                break;
            }
            if (digit >= base) {
                break;
            }

            // Accumulate result
            result = result * base + digit;

            // Advance to next character
            p++;
        }
        // Set end pointer if provided
        if (endptr != NULL) {
            *endptr = (wchar_t*)p;
        }

        // Apply sign
        return sign * result;
    }
    _INLINE double wcstod(const wchar_t* str, wchar_t** endptr) {
        double result = 0.0;
        double factor = 0.1;
        bool decimals = false;
        bool negative = false;
        for (const wchar_t* p = str; *p != L'\0'; ++p) {
            if (*p == L'-') {
                negative = true;
            }
            else if (*p == L'.') {
                decimals = true;
            }
            else if (*p >= L'0' && *p <= L'9') {
                if (decimals) {
                    result += (*p - L'0') * factor;
                    factor *= 0.1;
                }
                else {
                    result = result * 10 + (*p - L'0');
                }
            }
            else {
                if (endptr) {
                    *endptr = (wchar_t*)p;
                }
                break;
            }
        }
        return negative ? -result : result;
    }
	_INLINE float wtof(const wchar_t* str) {
		return (float)wcstod(str, NULL);
	}
    _INLINE float wcstof(const wchar_t* nptr, wchar_t** endptr) {
        // 跳过前导空格
        while (*nptr == ' ') {
            nptr++;
        }

        // 如果字符串为空或者不包含数字，返回0
        if (*nptr == L'\0' || !isdigit(*nptr)) {
            *endptr = (wchar_t*)nptr;
            return 0;
        }

        // 找到数字部分
        const wchar_t* start = nptr;
        while (*nptr != '\0' && isdigit(*nptr)) {
            nptr++;
        }

        // 将字符串转换为浮点数并返回
        *endptr = (wchar_t*)nptr;
        return wtof(start);

    }
    

    //数字处理
    enum class byte : unsigned char {};

    template <class _IntType, enable_if_t<is_integral_v<_IntType>, int> = 0>
    _NODISCARD constexpr byte operator<<(const byte _Arg, const _IntType _Shift) noexcept {
        // every static_cast is intentional
        return static_cast<byte>(static_cast<unsigned char>(static_cast<unsigned int>(_Arg) << _Shift));
    }

    template <class _IntType, enable_if_t<is_integral_v<_IntType>, int> = 0>
    _NODISCARD constexpr byte operator>>(const byte _Arg, const _IntType _Shift) noexcept {
        // every static_cast is intentional
        return static_cast<byte>(static_cast<unsigned char>(static_cast<unsigned int>(_Arg) >> _Shift));
    }

    _NODISCARD constexpr byte operator|(const byte _Left, const byte _Right) noexcept {
        // every static_cast is intentional
        return static_cast<byte>(
            static_cast<unsigned char>(static_cast<unsigned int>(_Left) | static_cast<unsigned int>(_Right)));
    }

    _NODISCARD constexpr byte operator&(const byte _Left, const byte _Right) noexcept {
        // every static_cast is intentional
        return static_cast<byte>(
            static_cast<unsigned char>(static_cast<unsigned int>(_Left) & static_cast<unsigned int>(_Right)));
    }

    _NODISCARD constexpr byte operator^(const byte _Left, const byte _Right) noexcept {
        // every static_cast is intentional
        return static_cast<byte>(
            static_cast<unsigned char>(static_cast<unsigned int>(_Left) ^ static_cast<unsigned int>(_Right)));
    }

    _NODISCARD constexpr byte operator~(const byte _Arg) noexcept {
        // every static_cast is intentional
        return static_cast<byte>(static_cast<unsigned char>(~static_cast<unsigned int>(_Arg)));
    }

    template <class _IntType, enable_if_t<is_integral_v<_IntType>, int> = 0>
    constexpr byte& operator<<=(byte& _Arg, const _IntType _Shift) noexcept {
        return _Arg = _Arg << _Shift;
    }

    template <class _IntType, enable_if_t<is_integral_v<_IntType>, int> = 0>
    constexpr byte& operator>>=(byte& _Arg, const _IntType _Shift) noexcept {
        return _Arg = _Arg >> _Shift;
    }

    constexpr byte& operator|=(byte& _Left, const byte _Right) noexcept {
        return _Left = _Left | _Right;
    }

    constexpr byte& operator&=(byte& _Left, const byte _Right) noexcept {
        return _Left = _Left & _Right;
    }

    constexpr byte& operator^=(byte& _Left, const byte _Right) noexcept {
        return _Left = _Left ^ _Right;
    }

    template <class _IntType, enable_if_t<is_integral_v<_IntType>, int> = 0>
    _NODISCARD constexpr _IntType to_integer(const byte _Arg) noexcept {
        return static_cast<_IntType>(_Arg);
    }

   

}