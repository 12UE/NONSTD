#pragma once
#include<stdarg.h>//������ں�ʹ���ǰ�ȫ��
#include <winternl.h>
//#include<ntstrsafe.h>
namespace nonstd {
	template<class T, class _Alloc = allocator<T>>//�����ַ����� �����ں�
	class basic_string {
	private:
		T* m_str=nullptr;
		size_t m_length=0;
		size_t m_capacity=0;//����
		void releasebuffer();//�ͷŻ�����
		void _strcpy(T* dest,const T* src,size_t length);//�ַ�������
		size_t strlength(const T* str);//��ȡ�ַ�������
		using iterator = RandomAccessIterator<T>;
		using const_iterator = const iterator;
	public:
		using allocate_type = _Alloc;//����������
		using value_type = T*;//ֵ����
		using char_type = T;//�ַ�����
		//��������
		char_type endChar = nonstd::is_same<T, char>::value ? '\0' : L'\0';
		static constexpr auto npos = -1;//��Чλ��
		
		basic_string();//���캯��
		template<typename ...elem,typename U=nonstd::enable_if_t<!nonstd::is_same_v<elem,char_type>>>
		basic_string(elem... args);//���캯��
		
		basic_string(nonstd::size_t size,char_type _endCh);//���캯��
		
		basic_string(const char_type* strbegin,const char_type* strEnd);//���캯��

		basic_string(const T* str);//���캯��

		basic_string(const basic_string& str);//�������캯��

		basic_string(const UNICODE_STRING & unicode_str);//���캯��


		//����������
		basic_string(iterator _begin, iterator _end);

		T first();//���ص�һ���ַ�

		T last();//�������һ���ַ�

		_INLINE iterator begin() const;//���ؿ�ͷ������

		_INLINE iterator end() const;//���ؽ�β������

		_INLINE void erase(size_t pos = 0, size_t len = 0);//ɾ���ַ���

		_INLINE void earse(size_t first, size_t last);//����ɾ��

		basic_string operator=(const basic_string& str);//��ֵ�����

		basic_string operator+(const basic_string& str);//�ַ�������׷���ַ���

		basic_string operator+=(const basic_string& str);//�ַ�������׷���ַ���

		basic_string operator+(const T* str);

		basic_string operator+=(const T* str);

		basic_string operator=(const T* str);

		template<class Z, class U = typename nonstd::enable_if_t<nonstd::has_type_v<Z, basic_string, const char*, const wchar_t*, UNICODE_STRING>>>
		_INLINE bool operator==(Z str);//�ж��Ƿ����

		operator UNICODE_STRING();

		template<typename U = nonstd::enable_if_t<nonstd::is_same_v<T, char>>>
		_INLINE  basic_string& format(const char* format, ...);//��ʽ��A�ַ���
		
		template<typename U = nonstd::enable_if_t<nonstd::is_same_v<T, wchar_t>>>
		_INLINE  basic_string& format(const wchar_t* format, ...);//��ʽ��W�ַ���

		_INLINE size_t length() const;//��ȡ�ַ�������

		_INLINE size_t capacity() const;//��ȡ�ַ�������

		_INLINE size_t max_capacity() const;//��ȡ�ַ����������

		_INLINE T& operator[](size_t index);//����[]�����

		_INLINE T* c_str() const;

		_INLINE T* data();//�����ַ���

		_INLINE size_t find(const T* str, size_t start = 0);//�����ַ���

		_INLINE size_t find(const basic_string& str, size_t start = 0);//�����ַ���

		_INLINE basic_string substr(size_t start, size_t length);//��ȡ�ַ���

		_INLINE size_t compare(const T* str);//�Ƚ��ַ���

		_INLINE size_t compare(const basic_string& str);//�Ƚ��ַ���

		_INLINE bool empty();//�ж��Ƿ�Ϊ��

		_INLINE void claer();//����ַ���

		_INLINE bool startwith(const T* str);//�ж��Ƿ���str��ͷ

		_INLINE void pop_back();//ɾ�����һ���ַ�

		_INLINE void resever(size_t nSize);//����nSize���ַ��Ŀռ�

		_INLINE void resize(size_t nSize);//�����ַ�������

		_INLINE void tolower();//ȫ��ת��ΪСд

		_INLINE void toupper();//ȫ��ת��Ϊ��д
		_INLINE T at(size_t index);//����ָ��λ�õ��ַ�

		_INLINE void shrink_to_fit();//�ͷŶ�����ڴ�

		~basic_string();
	};
	template<class T, class _Alloc>
	inline void basic_string<T, _Alloc>::releasebuffer() {
		_Alloc::Free(m_str, m_capacity);//�ͷ��ڴ�
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
	inline size_t basic_string<T, _Alloc>::strlength(const T* str) {//��ȡ�ַ�������
		endChar = nonstd::is_same<T, char>::value ? '\0' : L'\0';
		size_t length = 0;
		while (*str != endChar) {//�����ַ���
			length++;
			str++;
		}
		return length;
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc>::basic_string() {
		m_str = _Alloc::Alloc(1);//�����ڴ�
		endChar = nonstd::is_same<T, char>::value ? '\0' : L'\0';
		*m_str = endChar;
		m_length = 0;
		m_capacity = 1;
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc>::basic_string(nonstd::size_t size, char_type _endCh){
		//Ԥ�ȱ������� Ϊsize���ַ���,����Ϊ_endch
		m_str = _Alloc::Alloc(size + 1);//�����ڴ�
		m_capacity = size + 1;
		m_length = size;
		for (size_t i = 0; i < size; i++) {
			m_str[i] = _endCh;
		}
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc>::basic_string(const char_type* strbegin, const char_type* strEnd){
		//��strbegin��strEnd���ַ���
		m_length = strEnd - strbegin;
		m_capacity = m_length + 1;
		m_str = _Alloc::Alloc(m_capacity);//�����ڴ�
		
	}

	template<class T, class _Alloc>
	inline basic_string<T, _Alloc>::basic_string(const T* str) {
		m_length = strlength(str);
		m_str = _Alloc::Alloc(m_length + 1);//�����ڴ�
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
		if constexpr (nonstd::is_same_v<T, char>) {//�����char����
			m_length = unicode_str.Length / 2;
			m_str = _Alloc::Alloc(m_length + 1);
			ANSI_STRING szAnsiStr = { 0 };//
			if (NT_SUCCESS(RtlUnicodeStringToAnsiString(&szAnsiStr, &unicode_str, TRUE))) {//ת��ΪANSI_STRING
				m_str[m_length] = endChar;
				_strcpy(m_str, szAnsiStr.Buffer, m_length);
				RtlFreeAnsiString(&szAnsiStr);//�ͷ��ڴ�
				m_length = strlength(m_str);
			}
			else {
				m_str[0] = endChar;
				m_length = 0;
				DbgPrint("RtlUnicodeStringToAnsiString failed\n");//ת��ʧ��
			}
		}
		else {//TΪwchar_t
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
		if (pos < 0 || pos >= m_length) return;//�ж��Ƿ�Խ��
		if (len == 0) len = m_length - pos;//���lenΪ0����ɾ��pos����������ַ�
		if (len < 0 || pos + len > m_length) return;//�ж��Ƿ�Խ��
		for (size_t i = pos; i < m_length - len; i++)//ɾ��pos�����len���ַ�
			m_str[i] = m_str[i + len];//��pos�����len���ַ�ǰ��
		m_length -= len;//���³���
		endChar = nonstd::is_same<T, char>::value ? '\0' : L'\0';
		m_str[m_length] = endChar;//��ӽ�����
	}
	template<class T, class _Alloc>
	inline void basic_string<T, _Alloc>::earse(size_t first, size_t last) {
		if (first < 0 || first >= m_length) return;//�ж��Ƿ�Խ��
		if (last < 0 || last >= m_length) last = m_length - 1;//���lastԽ�磬��ɾ�������
		if (first > last) return;//�ж��Ƿ�Խ��
		for (size_t i = first; i < m_length - last; i++)//ɾ��first��last֮����ַ�
			m_str[i] = m_str[i + last - first + 1];//��first��last֮����ַ�ǰ��
		m_length -= last - first + 1;//���³���
		endChar = nonstd::is_same<T, char>::value ? '\0' : L'\0';
		m_str[m_length] = endChar;//��ӽ�����
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc> basic_string<T, _Alloc>::operator=(const basic_string& str) {
		if(_Alloc::CheckAddr(m_str)) _Alloc::Free(m_str);//�ͷ�ԭ�����ڴ�
		m_length = str.length();//���³���
		m_capacity = m_length + 1;//��������
		m_str = _Alloc::Alloc(m_capacity);//���·����ڴ�
		_strcpy(m_str, str.c_str(), m_length);//�����ַ���
		return *this;
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc> basic_string<T, _Alloc>::operator+(const basic_string& str) {
		basic_string temp;
		temp.m_length = m_length + str.length();//���㳤��
		temp.m_str = _Alloc::Alloc(temp.m_length + 1);//�����ڴ�
		_strcpy(temp.m_str,m_str,m_length);//���ȸ����Լ����ַ���
		_strcpy(temp.m_str + m_length,str.c_str(),str.length());//�ٸ���str���ַ���
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
		va_start(args, format);//��ȡ����
		size_t remainlength = 0;
		const size_t MaxStrLength = 4096;//��󳤶�
		T* strbuffer = _Alloc::Alloc(MaxStrLength);//�����ڴ�
		size_t written = vsprintf(strbuffer, format, args);//��ʽ���ַ���
		va_end(args);//�������
		m_str = _Alloc::Alloc(written + 1);//�����ڴ�
		_strcpy(m_str, strbuffer, written);//�����ַ���
		_Alloc::Free(strbuffer);//�ͷ��ڴ�
		m_length = written;
		m_capacity = m_length + 1;
		return *this;
	}

	template<class T, class _Alloc>
	template<typename U>
	inline basic_string<T, _Alloc>& basic_string<T, _Alloc>::format(const wchar_t* format, ...) {
		va_list args;
		va_start(args, format);//��ȡ����
		size_t remainlength = 0;
		const size_t MaxStrLength = 8192;//��󳤶�
		T* strbuffer = _Alloc::Alloc(MaxStrLength);//�����ڴ�
		size_t written =vswprintf(strbuffer,format, args);//��ʽ���ַ���
		va_end(args);//�������
		m_str = _Alloc::Alloc(written + 1);//�����ڴ�
		_strcpy(m_str, strbuffer, written);//�����ַ���
		_Alloc::Free(strbuffer);//�ͷ��ڴ�
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
		}
		else {
			return m_str[m_length];//���ؽ�������
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
	inline size_t basic_string<T, _Alloc>::find(const basic_string& str, size_t start) {//Sunday�㷨 O(n)
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
	inline basic_string<T, _Alloc> basic_string<T, _Alloc>::substr(size_t start, size_t length) {//��ȡ�ַ���
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
	inline size_t basic_string<T, _Alloc>::compare(const T* str) {//�Ƚ��ַ���
		basic_string temp(str);
		return compare(temp);
	}
	template<class T, class _Alloc>
	inline size_t basic_string<T, _Alloc>::compare(const basic_string& str) {//�Ƚ��ַ���
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
	inline bool basic_string<T, _Alloc>::startwith(const T* str) {//�ж��Ƿ���str��ͷ
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
			releasebuffer();//�ͷ�ԭ�����ڴ�
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
	inline T basic_string<T, _Alloc>::at(size_t index) {
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
			for (size_t i = 0; i < m_length; i++) {//�Ƚ��ַ���
				if (m_str[i] != str.m_str[i])return false;//�����
			}
			return true;
		}else {
			basic_string temp(str);
			return operator==(temp);
		}
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