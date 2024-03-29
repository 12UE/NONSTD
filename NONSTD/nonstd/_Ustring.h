#pragma once
#include<stdarg.h>//这个在内核使用是安全的
#include <winternl.h>
//#include<ntstrsafe.h>
namespace nonstd {
	template<class T, class _Alloc = allocator<T>>//基础字符串类 用于内核
	class basic_string {
	private:
		T* m_str=nullptr;
		size_t m_length=0;
		size_t m_capacity=0;//容量
		void releasebuffer();//释放缓冲区
		void _strcpy(T* dest,const T* src,size_t length);//字符串复制
		size_t strlength(const T* str);//获取字符串长度
		using iterator = RandomAccessIterator<T>;
		using const_iterator = const iterator;
	public:
		using allocate_type = _Alloc;//分配器类型
		using value_type = T*;//值类型
		using char_type = T;//字符类型
		//结束符号
		char_type endChar = nonstd::is_same<T, char>::value ? '\0' : L'\0';
		static constexpr auto npos = -1;//无效位置
		
		basic_string();//构造函数
		template<typename ...elem,typename U=nonstd::enable_if_t<!nonstd::is_same_v<elem,char_type>>>
		basic_string(elem... args);//构造函数
		
		basic_string(nonstd::size_t size,char_type _endCh);//构造函数
		
		basic_string(const char_type* strbegin,const char_type* strEnd);//构造函数

		basic_string(const T* str);//构造函数

		basic_string(const basic_string& str);//拷贝构造函数

		basic_string(const UNICODE_STRING & unicode_str);//构造函数


		//迭代器构造
		basic_string(iterator _begin, iterator _end);

		T first();//返回第一个字符

		T last();//返回最后一个字符

		_INLINE iterator begin() const;//返回开头迭代器

		_INLINE iterator end() const;//返回结尾迭代器

		_INLINE void erase(size_t pos = 0, size_t len = 0);//删除字符串

		_INLINE void earse(size_t first, size_t last);//区间删除

		basic_string operator=(const basic_string& str);//赋值运算符

		basic_string operator+(const basic_string& str);//字符串后面追加字符串

		basic_string operator+=(const basic_string& str);//字符串后面追加字符串

		basic_string operator+(const T* str);

		basic_string operator+=(const T* str);

		basic_string operator=(const T* str);

		template<class Z, class U = typename nonstd::enable_if_t<nonstd::has_type_v<Z, basic_string, const char*, const wchar_t*, UNICODE_STRING>>>
		_INLINE bool operator==(Z str);//判断是否相等

		operator UNICODE_STRING();

		template<typename U = nonstd::enable_if_t<nonstd::is_same_v<T, char>>>
		_INLINE  basic_string& format(const char* format, ...);//格式化A字符串
		
		template<typename U = nonstd::enable_if_t<nonstd::is_same_v<T, wchar_t>>>
		_INLINE  basic_string& format(const wchar_t* format, ...);//格式化W字符串

		_INLINE size_t length() const;//获取字符串长度

		_INLINE size_t capacity() const;//获取字符串容量

		_INLINE size_t max_capacity() const;//获取字符串最大容量

		_INLINE T& operator[](size_t index);//重载[]运算符

		_INLINE T* c_str() const;

		_INLINE T* data();//返回字符串

		_INLINE size_t find(const T* str, size_t start = 0);//查找字符串

		_INLINE size_t find(const basic_string& str, size_t start = 0);//查找字符串

		_INLINE basic_string substr(size_t start, size_t length);//截取字符串

		_INLINE size_t compare(const T* str);//比较字符串

		_INLINE size_t compare(const basic_string& str);//比较字符串

		_INLINE bool empty();//判断是否为空

		_INLINE void claer();//清空字符串

		_INLINE bool startwith(const T* str);//判断是否以str开头

		_INLINE void pop_back();//删除最后一个字符

		_INLINE void resever(size_t nSize);//保留nSize个字符的空间

		_INLINE void resize(size_t nSize);//调整字符串长度

		_INLINE void tolower();//全部转换为小写

		_INLINE void toupper();//全部转换为大写
		
		_INLINE T at(size_t index) const;//返回指定位置的字符

		_INLINE void shrink_to_fit();//释放多余的内存

		~basic_string();
	};
	template<class T, class _Alloc>
	inline void basic_string<T, _Alloc>::releasebuffer() {
		_Alloc::Free(m_str, m_capacity);//释放内存
		m_capacity = 0;
	}
	template <class T, class _Alloc>
	inline void basic_string<T, _Alloc>::_strcpy(T* dest,const T* src,size_t length){
		if (length == 0|| src == nullptr || dest == nullptr) return;
		endChar = nonstd::is_same<T, char>::value ? '\0' : L'\0';
		for (size_t i = 0; i < length; i++) {
			*dest = *src;
			dest++;
			src++;
		}
		*dest = endChar;
	}
	template<class T, class _Alloc>
	inline size_t basic_string<T, _Alloc>::strlength(const T* str) {//获取字符串长度
		endChar = nonstd::is_same<T, char>::value ? '\0' : L'\0';
		size_t length = 0;
		while (*str != endChar) {//遍历字符串
			length++;
			str++;
		}
		return length;
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc>::basic_string() {
		m_str = _Alloc::Alloc(1);//分配内存
		endChar = nonstd::is_same<T, char>::value ? '\0' : L'\0';
		*m_str = endChar;
		m_length = 0;
		m_capacity = 1;
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc>::basic_string(nonstd::size_t size, char_type _endCh){
		//预先保留长度 为size的字符串,结束为_endch
		m_str = _Alloc::Alloc(size + 1);//分配内存
		m_capacity = size + 1;
		m_length = size;
		for (size_t i = 0; i < size; i++) {
			m_str[i] = _endCh;
		}
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc>::basic_string(const char_type* strbegin, const char_type* strEnd){
		//从strbegin到strEnd的字符串
		m_length = strEnd - strbegin;
		m_capacity = m_length + 1;
		m_str = _Alloc::Alloc(m_capacity);//分配内存
		
	}

	template<class T, class _Alloc>
	inline basic_string<T, _Alloc>::basic_string(const T* str) {
		m_length = strlength(str);
		m_str = _Alloc::Alloc(m_length + 1);//分配内存
		_strcpy(m_str, str, m_length);
		m_capacity = m_length + 1;
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc>::basic_string(const basic_string& str) {
		m_length = str.length();
		m_str = _Alloc::Alloc(m_length + 1);
		_strcpy(m_str, str.c_str(), m_length);
		m_capacity = m_length + 1;
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc>::basic_string(const UNICODE_STRING& unicode_str) {
		if constexpr (nonstd::is_same_v<T, char>) {//如果是char类型
			m_length = unicode_str.Length / 2;
			m_str = _Alloc::Alloc(m_length + 1);
			ANSI_STRING szAnsiStr = { 0 };//
			if (NT_SUCCESS(RtlUnicodeStringToAnsiString(&szAnsiStr, &unicode_str, TRUE))) {//转换为ANSI_STRING
				m_str[m_length] = endChar;
				_strcpy(m_str, szAnsiStr.Buffer, m_length);
				RtlFreeAnsiString(&szAnsiStr);//释放内存
				m_length = strlength(m_str);
			}
			else {
				m_str[0] = endChar;
				m_length = 0;
				DbgPrint("RtlUnicodeStringToAnsiString failed\n");//转换失败
			}
		}
		else {//T为wchar_t
			m_length = unicode_str.Length / 2;
			m_str = _Alloc::Alloc(m_length + 1);
			_strcpy(m_str, unicode_str.Buffer, m_length);
		}
		m_capacity = m_length + 1;
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc>::basic_string(iterator _begin, iterator _end){
		if (_begin <= _end) {
			m_length = _end - _begin;
			m_str = _Alloc::Alloc(m_length + 1);
			nonstd::copy_n(_begin, m_length, m_str);
			m_capacity = m_length + 1;
		}
	}
	template<class T, class _Alloc>
	inline T basic_string<T, _Alloc>::first() {
		if (m_length == 0) return endChar;
		return m_str[0];
	}
	template<class T, class _Alloc>
	inline T basic_string<T, _Alloc>::last() {
		if (m_length == 0) return endChar;
		return m_str[m_length - 1];
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc>::iterator basic_string<T, _Alloc>::begin() const { return basic_string<T, _Alloc>::iterator(m_str); }
	
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc>::iterator basic_string<T, _Alloc>::end() const { return basic_string<T, _Alloc>::iterator(m_str + m_length); }
	
	template<class T, class _Alloc>
	inline void basic_string<T, _Alloc>::erase(size_t pos, size_t len) {
		if (pos < 0 || pos >= m_length) return;//判断是否越界
		if (len == 0) len = m_length - pos;//如果len为0，则删除pos后面的所有字符
		if (len < 0 || pos + len > m_length) return;//判断是否越界
		for (size_t i = pos; i < m_length - len; i++)//删除pos后面的len个字符
			m_str[i] = m_str[i + len];//将pos后面的len个字符前移
		m_length -= len;//更新长度
		endChar = nonstd::is_same<T, char>::value ? '\0' : L'\0';
		m_str[m_length] = endChar;//添加结束符
	}
	template<class T, class _Alloc>
	inline void basic_string<T, _Alloc>::earse(size_t first, size_t last) {
		if (first < 0 || first >= m_length) return;//判断是否越界
		if (last < 0 || last >= m_length) last = m_length - 1;//如果last越界，则删除到最后
		if (first > last) return;//判断是否越界
		for (size_t i = first; i < m_length - last; i++)//删除first到last之间的字符
			m_str[i] = m_str[i + last - first + 1];//将first到last之间的字符前移
		m_length -= last - first + 1;//更新长度
		endChar = nonstd::is_same<T, char>::value ? '\0' : L'\0';
		m_str[m_length] = endChar;//添加结束符
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc> basic_string<T, _Alloc>::operator=(const basic_string& str) {
		if(_Alloc::CheckAddr(m_str)) _Alloc::Free(m_str);//释放原来的内存
		m_length = str.length();//更新长度
		m_capacity = m_length + 1;//更新容量
		m_str = _Alloc::Alloc(m_capacity);//重新分配内存
		_strcpy(m_str, str.c_str(), m_length);//复制字符串
		return *this;
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc> basic_string<T, _Alloc>::operator+(const basic_string& str) {
		basic_string temp;
		temp.m_length = m_length + str.length();//计算长度
		temp.m_str = _Alloc::Alloc(temp.m_length + 1);//分配内存
		_strcpy(temp.m_str,m_str,m_length);//首先复制自己的字符串
		_strcpy(temp.m_str + m_length,str.c_str(),str.length());//再复制str的字符串
		return temp;
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc> basic_string<T, _Alloc>::operator+=(const basic_string& str) {
		size_t len = m_length + str.length();
		T* temp = _Alloc::Alloc(len + 1);
		_strcpy(temp, m_str, m_length);
		_strcpy(temp + m_length, str.c_str(), str.length());
		releasebuffer();
		m_str = temp;
		m_length = len;
		m_capacity = m_length + 1;
		return *this;
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc> basic_string<T, _Alloc>::operator+(const T* str) {
		basic_string temp;
		size_t strln = strlength(str);
		temp.m_length = m_length + strln;
		temp.m_str = _Alloc::Alloc(temp.m_length + 1);
		_strcpy(temp.m_str, m_str, m_length);
		_strcpy(temp.m_str + m_length, str, strln);
		temp.m_capacity = temp.m_length + 1;
		return temp;
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc> basic_string<T, _Alloc>::operator+=(const T* str) {
		size_t strln = strlength(str);
		size_t len = m_length + strln;
		T* temp = _Alloc::Alloc(len + 1);
		_strcpy(temp, m_str, m_length);
		_strcpy(temp + m_length, str, strln);
		releasebuffer();
		m_str = temp;
		m_length = len;
		m_capacity = m_length + 1;
		return *this;
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc> basic_string<T, _Alloc>::operator=(const T* str) {
		m_length = strlength(str);
		if (m_length > 0) {
			releasebuffer();
			m_str = _Alloc::Alloc(m_length + 1);
			_strcpy(m_str, str, m_length);
			m_capacity = m_length + 1;
		}
		return *this;
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc>::operator UNICODE_STRING() {
		if (m_str == nullptr) return UNICODE_STRING();
		UNICODE_STRING unicode_str;
		if constexpr (nonstd::is_same_v<T, char>) {
			ANSI_STRING szAnsiStr = { 0 };
			RtlInitAnsiString(&szAnsiStr, m_str);
			RtlAnsiStringToUnicodeString(&unicode_str, &szAnsiStr, TRUE);
			return unicode_str;
		}
		else {
			RtlInitUnicodeString(&unicode_str, m_str);
			return unicode_str;
		}
	}
	template<class T, class _Alloc>//w
	template<typename U>
	inline basic_string<T, _Alloc>& basic_string<T, _Alloc>::format(const char* format, ...) {
		va_list args;
		va_start(args, format);//获取参数
		size_t remainlength = 0;
		const size_t MaxStrLength = 4096;//最大长度
		T* strbuffer = _Alloc::Alloc(MaxStrLength);//分配内存
		size_t written = vsprintf(strbuffer, format, args);//格式化字符串
		va_end(args);//结束变参
		m_str = _Alloc::Alloc(written + 1);//分配内存
		_strcpy(m_str, strbuffer, written);//复制字符串
		_Alloc::Free(strbuffer);//释放内存
		m_length = written;
		m_capacity = m_length + 1;
		return *this;
	}

	template<class T, class _Alloc>
	template<typename U>
	inline basic_string<T, _Alloc>& basic_string<T, _Alloc>::format(const wchar_t* format, ...) {
		va_list args;
		va_start(args, format);//获取参数
		size_t remainlength = 0;
		const size_t MaxStrLength = 8192;//最大长度
		T* strbuffer = _Alloc::Alloc(MaxStrLength);//分配内存
		size_t written =vswprintf(strbuffer,format, args);//格式化字符串
		va_end(args);//结束变参
		m_str = _Alloc::Alloc(written + 1);//分配内存
		_strcpy(m_str, strbuffer, written);//复制字符串
		_Alloc::Free(strbuffer);//释放内存
		m_length = written;
		m_capacity = m_length + 1;
		return *this;
	}
	template<class T, class _Alloc>
	inline size_t basic_string<T, _Alloc>::length() const { return m_length; }

	template<class T, class _Alloc>
	inline size_t basic_string<T, _Alloc>::capacity() const {
		return m_capacity;
	}
	template<class T, class _Alloc>
	inline size_t basic_string<T, _Alloc>::max_capacity() const {
		return _Alloc::max_size();
	}
	template<class T, class _Alloc>
	inline T& basic_string<T, _Alloc>::operator[](size_t index) {
		if (index >= 0 || index <= m_length) {
			return m_str[index];
		}else {
			return m_str[m_length];//返回结束符号
		}
	}


	
	template<class T, class _Alloc>
	inline T* basic_string<T, _Alloc>::c_str() const { return m_str; }
	
	template<class T, class _Alloc>
	inline T* basic_string<T, _Alloc>::data() { 
		return m_str; 
	}
	
	template<class T, class _Alloc>
	inline size_t basic_string<T, _Alloc>::find(const T* str, size_t start) {
		basic_string temp(str);
		return find(temp, start);
	}
	template<class T, class _Alloc>
	inline size_t basic_string<T, _Alloc>::find(const basic_string& str, size_t start) {//Sunday算法 O(n)
		if (start < 0 || start >= m_length) return -1;
		if (str.length() == 0) return -1;
		for (auto m_it = 0, s_it = 0; s_it < m_length; s_it++) {
			if (m_str[s_it] == str.m_str[m_it]) {
				m_it++;
				if (m_it == str.length()) return s_it - m_it + 1;
			}
			else {
				s_it -= m_it;
				m_it = 0;
			}
		}
		return -1;
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc> basic_string<T, _Alloc>::substr(size_t start, size_t length) {//截取字符串
		if (start < 0 || start >= m_length) return basic_string();
		basic_string temp;
		temp.m_length = length;
		temp.m_str = _Alloc::Alloc(temp.m_length + 1);
		for (size_t i = 0; i < length; i++) {
			temp.m_str[i] = m_str[start + i];
		}
		endChar = nonstd::is_same<T, char>::value ? '\0' : L'\0';
		temp.m_str[temp.m_length] = endChar;
		return temp;
	}
	template<class T, class _Alloc>
	inline size_t basic_string<T, _Alloc>::compare(const T* str) {//比较字符串
		basic_string temp(str);
		return compare(temp);
	}
	template<class T, class _Alloc>
	inline size_t basic_string<T, _Alloc>::compare(const basic_string& str) {//比较字符串
		if (m_length > str.m_length) {
			return 1;
		}else if (m_length < str.m_length) {
			return -1;
		}else {
			for (size_t i = 0; i < m_length; i++) {
				if (m_str[i] > str.m_str[i]) {
					return 1;
				}
				else if (m_str[i] < str.m_str[i]) {
					return -1;
				}
			}
			return 0;
		}
	}
	template<class T, class _Alloc>
	inline bool basic_string<T, _Alloc>::empty() { return m_length == 0; }
	template<class T, class _Alloc>
	inline void basic_string<T, _Alloc>::claer() {
		releasebuffer();
		m_length = 0;
		m_str = _Alloc::Alloc(1);
		m_str[0] = endChar;
		m_capacity = 0;
	}
	template<class T, class _Alloc>
	inline bool basic_string<T, _Alloc>::startwith(const T* str) {//判断是否以str开头
		size_t length = strlength(str);
		for (size_t i = 0; i < length; i++) {
			if (m_str[i] != str[i])return false;
		}
		return true;
	}
	template<class T, class _Alloc>
	inline void basic_string<T, _Alloc>::pop_back() {
		if (m_length > 0) {
			m_length--;
			m_str[m_length] = endChar;
		}
	}
	template<class T, class _Alloc>
	inline void basic_string<T, _Alloc>::resever(size_t nSize) {
		if (nSize > m_capacity) {
			size_t newSize = m_capacity * 2;
			T* temp = _Alloc::Alloc(newSize);
			_strcpy(temp, m_str, m_length);
			releasebuffer();//释放原来的内存
			m_str = temp;
			m_capacity = newSize;
		}
	}
	template<class T, class _Alloc>
	inline void basic_string<T, _Alloc>::resize(size_t nSize) {
		if (nSize > m_length) {
			T* temp = _Alloc::Alloc(nSize + 1);
			_strcpy(temp, m_str, m_length);
			releasebuffer();
			m_str = temp;
			m_length = nSize;
		}
	}
	template<class T, class _Alloc>
	inline void basic_string<T, _Alloc>::tolower() {
		char_type upper = nonstd::is_same<T, char>::value ? 'A' : L'A';
		char_type lower = nonstd::is_same<T, char>::value ? 'a' : L'a';
		auto diff = lower - upper;
		for (size_t i = 0; i < m_length; i++) {
			if (m_str[i] >= upper && m_str[i] <= upper + 25) {
				m_str[i] += diff;
			}
		}
	}
	template<class T, class _Alloc>
	inline void basic_string<T, _Alloc>::toupper() {
		char_type upper = nonstd::is_same<T, char>::value ? 'A' : L'A';
		char_type lower = nonstd::is_same<T, char>::value ? 'a' : L'a';
		const auto diff = lower - upper;
		for (size_t i = 0; i < m_length; i++) {
			if (m_str[i] >= lower && m_str[i] <= lower + 25) {
				m_str[i] -= diff;
			}
		}
	}
	template<class T, class _Alloc>
	inline T basic_string<T, _Alloc>::at(size_t index) const {
		if (index < 0 || index >= m_length) return endChar;
		return m_str[index];
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc>::~basic_string() {
		releasebuffer();
	}
	
	template<class T, class _Alloc>
	template<typename ...elem,typename U>
	inline basic_string<T, _Alloc>::basic_string(elem ...args)
	{
		m_length = sizeof...(args);
		m_str = _Alloc::Alloc(m_length + 1);
		m_capacity = m_length+1;
		size_t i = 0;
		((m_str[i++] = *args), ...);
		m_str[m_length] = endChar;
	}
	template<class T, class _Alloc>
	template<class Z, class U>
	inline bool basic_string<T, _Alloc>::operator==(Z str) {
		if constexpr (nonstd::is_same_v<Z, basic_string>) {
			if (m_length != str.length()) return false;
			for (size_t i = 0; i < m_length; i++) {//比较字符串
				if (m_str[i] != str.m_str[i])return false;//不相等
			}
			return true;
		}else {
			basic_string temp(str);
			return operator==(temp);
		}
	}
	//重载operator<
	template<class T, class _Alloc>
	inline bool operator<(const basic_string<T, _Alloc>& str1,const basic_string<T, _Alloc>& str2) {
		size_t length = str1.length() < str2.length() ? str1.length() : str2.length();
		for (size_t i = 0; i < length; i++) {
			if (str1.at(i) < str2.at(i))return true;
			else if (str1.at(i) > str2.at(i))return false;
		}
		return str1.length() < str2.length();
	}

	
	template<class T, class _Alloc>
	inline void basic_string<T, _Alloc>::shrink_to_fit() {
		if (m_length < m_capacity) {
			T* temp = _Alloc::Alloc(m_length + 1);
			_strcpy(temp, m_str, m_length);
			releasebuffer();
			m_str = temp;
			m_capacity = m_length+1;
		}
		else {
			resever(m_length);
		}
	}
	using string = basic_string<char, allocator<char>>;
	using wstring = basic_string<wchar_t, allocator<wchar_t>>;
}