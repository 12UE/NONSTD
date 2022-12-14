#pragma once
#include<stdarg.h>//������ں�ʹ���ǰ�ȫ��
#include <winternl.h>
//#include<ntstrsafe.h>
namespace nonstd {
	template<class T, class _Alloc = allcator<T>>//�����ַ����� �����ں�
	class basic_string {
	private:
		T* m_str;
		int m_length;
		int m_capacity;//����
		void releasebuffer();//�ͷŻ�����
		void strcopy(const T* src, T* dest);//�ַ�������
		void strcopy(const T* src, T* dest, int length);//�ַ�������
		int strlength(const T* str);//��ȡ�ַ�������
		using iterator = RandomAccessIterator<T>;
		using const_iterator = const iterator;
	public:
		using allocate_type = _Alloc;//����������
		using value_type = T*;//ֵ����
		using char_type = T;//�ַ�����
		const char_type upper = nonstd::is_same<T, char>::value ? 'A' : L'A';
		const char_type lower = nonstd::is_same<T, char>::value ? 'a' : L'a';
		//��������
		const char_type endChar = nonstd::is_same<T, char>::value ? '\0' : L'\0';
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

		_INLINE void erase(int pos = 0, int len = 0);//ɾ���ַ���

		_INLINE void earse(int first, int last);//����ɾ��

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

		_INLINE int length() const;//��ȡ�ַ�������

		_INLINE int capacity() const;//��ȡ�ַ�������

		_INLINE int max_capacity() const;//��ȡ�ַ����������

		_INLINE T& operator[](int index);//����[]�����

		_INLINE T* c_str() const;

		_INLINE T* data();//�����ַ���

		_INLINE int find(const T* str, int start = 0);//�����ַ���

		_INLINE int find(const basic_string& str, int start = 0);//�����ַ���

		_INLINE basic_string substr(int start, int length);//��ȡ�ַ���

		_INLINE int compare(const T* str);//�Ƚ��ַ���

		_INLINE int compare(const basic_string& str);//�Ƚ��ַ���

		_INLINE bool empty();//�ж��Ƿ�Ϊ��

		_INLINE void claer();//����ַ���

		_INLINE bool startwith(const T* str);//�ж��Ƿ���str��ͷ

		_INLINE void pop_back();//ɾ�����һ���ַ�

		_INLINE void resever(size_t nSize);//����nSize���ַ��Ŀռ�

		_INLINE void resize(size_t nSize);//�����ַ�������

		_INLINE void tolower();//ȫ��ת��ΪСд

		_INLINE void toupper();//ȫ��ת��Ϊ��д
		_INLINE T at(int index);//����ָ��λ�õ��ַ�

		_INLINE void shrink_to_fit();//�ͷŶ�����ڴ�

		~basic_string();
	};
	template<class T, class _Alloc>
	inline void basic_string<T, _Alloc>::releasebuffer() {
		if (m_str != nullptr) {
			_Alloc::deallocate(m_str);//�ͷ��ڴ�
			m_str = nullptr;
			m_capacity = 0;
		}
	}
	template <class T, class _Alloc>
	inline void basic_string<T, _Alloc>::strcopy(const T* src, T* dest)
	{                         // �����ַ���
		while (*src != endChar) {//�����ַ���
			*dest = *src;
			src++, dest++;
		}
		*dest = endChar;
	}
	template <class T, class _Alloc>
	inline void basic_string<T, _Alloc>::strcopy(const T* src, T* dest,int length){
		for (int i = 0; i < length; i++) {//�����ַ���
			*dest = *src;
			src++, dest++;
		}
		*dest = endChar;
	}
	template<class T, class _Alloc>
	inline int basic_string<T, _Alloc>::strlength(const T* str) {//��ȡ�ַ�������
		int length = 0;
		while (*str != endChar) {//�����ַ���
			length++;
			str++;
		}
		return length;
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc>::basic_string() {
		m_str = _Alloc::allocate(1);//�����ڴ�
		*m_str = endChar;
		m_length = 0;
		m_capacity = 1;
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc>::basic_string(nonstd::size_t size, char_type _endCh){
		//Ԥ�ȱ������� Ϊsize���ַ���,����Ϊ_endch
		m_str = _Alloc::allocate(size + 1);//�����ڴ�
		m_capacity = size + 1;
		m_length = size;
		for (int i = 0; i < size; i++) {
			m_str[i] = _endCh;
		}
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc>::basic_string(const char_type* strbegin, const char_type* strEnd){
		//��strbegin��strEnd���ַ���
		m_length = strEnd - strbegin;
		m_capacity = m_length + 1;
		m_str = _Alloc::allocate(m_capacity);//�����ڴ�
		strcopy(strbegin, m_str, m_length);
	}

	template<class T, class _Alloc>
	inline basic_string<T, _Alloc>::basic_string(const T* str) {
		m_length = strlength(str);
		releasebuffer();
		m_str = _Alloc::allocate(m_length + 1);//�����ڴ�
		strcopy(str, m_str);
		m_capacity = m_length + 1;
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc>::basic_string(const basic_string& str) {
		m_length = str.length();
		releasebuffer();
		m_str = _Alloc::allocate(m_length + 1);
		strcopy(str.c_str(), m_str);
		m_capacity = m_length + 1;
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc>::basic_string(const UNICODE_STRING& unicode_str) {
		if constexpr (nonstd::is_same_v<T, char>) {//�����char����
			m_length = unicode_str.Length / 2;
			releasebuffer();
			m_str = _Alloc::allocate(m_length + 1);
			ANSI_STRING szAnsiStr = { 0 };//
			if (NT_SUCCESS(RtlUnicodeStringToAnsiString(&szAnsiStr, &unicode_str, TRUE))) {//ת��ΪANSI_STRING
				m_str[m_length] = endChar;
				strcopy(szAnsiStr.Buffer, m_str);
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
			releasebuffer();
			m_str = _Alloc::allocate(m_length + 1);
			strcopy(unicode_str.Buffer, m_str);
		}
		m_capacity = m_length + 1;
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc>::basic_string(iterator _begin, iterator _end){
		if (_begin <= _end) {
			m_length = _end - _begin;
			releasebuffer();
			m_str = _Alloc::allocate(m_length + 1);
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
	inline void basic_string<T, _Alloc>::erase(int pos, int len) {
		if (pos < 0 || pos >= m_length) return;//�ж��Ƿ�Խ��
		if (len == 0) len = m_length - pos;//���lenΪ0����ɾ��pos����������ַ�
		if (len < 0 || pos + len > m_length) return;//�ж��Ƿ�Խ��
		for (int i = pos; i < m_length - len; i++)//ɾ��pos�����len���ַ�
			m_str[i] = m_str[i + len];//��pos�����len���ַ�ǰ��
		m_length -= len;//���³���
		m_str[m_length] = endChar;//��ӽ�����
	}
	template<class T, class _Alloc>
	inline void basic_string<T, _Alloc>::earse(int first, int last) {
		if (first < 0 || first >= m_length) return;//�ж��Ƿ�Խ��
		if (last < 0 || last >= m_length) last = m_length - 1;//���lastԽ�磬��ɾ�������
		if (first > last) return;//�ж��Ƿ�Խ��
		for (int i = first; i < m_length - last; i++)//ɾ��first��last֮����ַ�
			m_str[i] = m_str[i + last - first + 1];//��first��last֮����ַ�ǰ��
		m_length -= last - first + 1;//���³���
		m_str[m_length] = endChar;//��ӽ�����
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc> basic_string<T, _Alloc>::operator=(const basic_string& str) {
		if (this != &str) {
			m_length = str.length();//���³���
			releasebuffer();//�ͷ�ԭ�����ڴ�
			m_str = _Alloc::allocate(m_length + 1);//���·����ڴ�
			strcopy(str.c_str(), m_str);//�����ַ���
			m_capacity = m_length + 1;//��������
		}
		return *this;
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc> basic_string<T, _Alloc>::operator+(const basic_string& str) {
		basic_string temp;
		temp.m_length = m_length + str.length();
		temp.m_str = _Alloc::allocate(temp.m_length + 1);
		strcopy(m_str, temp.m_str);
		strcopy(str.c_str(), temp.m_str + m_length);
		return temp;
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc> basic_string<T, _Alloc>::operator+=(const basic_string& str) {
		int len = m_length + str.length();
		T* temp = _Alloc::allocate(len + 1);
		strcopy(m_str, temp);
		strcopy(str.c_str(), temp + m_length);
		releasebuffer();
		m_str = temp;
		m_length = len;
		m_capacity = m_length + 1;
		return *this;
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc> basic_string<T, _Alloc>::operator+(const T* str) {
		basic_string temp;
		temp.m_length = m_length + strlength(str);
		temp.m_str = _Alloc::allocate(temp.m_length + 1);
		strcopy(m_str, temp.m_str);
		strcopy(str, temp.m_str + m_length);
		temp.m_capacity = temp.m_length + 1;
		return temp;
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc> basic_string<T, _Alloc>::operator+=(const T* str) {
		int len = m_length + strlength(str);
		T* temp = _Alloc::allocate(len + 1);
		strcopy(m_str, temp);
		strcopy(str, temp + m_length);
		releasebuffer();
		m_str = temp;
		m_length = len;
		m_capacity = m_length + 1;
		return *this;
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc> basic_string<T, _Alloc>::operator=(const T* str) {
		releasebuffer();
		m_length = strlength(str);
		m_str = _Alloc::allocate(m_length + 1);
		strcopy(str, m_str);
		m_capacity = m_length + 1;
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
		const int MaxStrLength = 8192;//��󳤶�
		T* strbuffer = _Alloc::allocate(MaxStrLength);//�����ڴ�
		int written = vsprintf(strbuffer, format, args);//��ʽ���ַ���
		va_end(args);//�������
		T* temp = _Alloc::allocate(written + 1);//�����ڴ�
		strcopy(strbuffer, temp);//�����ַ���
		_Alloc::deallocate(strbuffer);//�ͷ��ڴ�
		releasebuffer();
		m_str = temp;
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
		const int MaxStrLength = 8192;//��󳤶�
		T* strbuffer = _Alloc::allocate(MaxStrLength);//�����ڴ�
		int written =vswprintf(strbuffer,format, args);//��ʽ���ַ���
		va_end(args);//�������
		T* temp = _Alloc::allocate(written + 1);//�����ڴ�
		strcopy(strbuffer, temp);//�������ַ���
		_Alloc::deallocate(strbuffer);//�ͷ��ڴ�
		releasebuffer();
		m_str = temp;
		m_length = written;
		m_capacity = m_length + 1;
		return *this;
	}
	template<class T, class _Alloc>
	inline int basic_string<T, _Alloc>::length() const { return m_length; }

	template<class T, class _Alloc>
	inline int basic_string<T, _Alloc>::capacity() const {
		return m_capacity;
	}
	template<class T, class _Alloc>
	inline int basic_string<T, _Alloc>::max_capacity() const {
		return _Alloc::max_size();
	}
	template<class T, class _Alloc>
	inline T& basic_string<T, _Alloc>::operator[](int index) {
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
	inline T* basic_string<T, _Alloc>::data() { return m_str; }
	template<class T, class _Alloc>
	inline int basic_string<T, _Alloc>::find(const T* str, int start) {
		basic_string temp(str);
		return find(temp, start);
	}
	template<class T, class _Alloc>
	inline int basic_string<T, _Alloc>::find(const basic_string& str, int start) {//Sunday�㷨 O(n)
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
	inline basic_string<T, _Alloc> basic_string<T, _Alloc>::substr(int start, int length) {//��ȡ�ַ���
		if (start < 0 || start >= m_length) return basic_string();
		basic_string temp;
		temp.m_length = length;
		temp.m_str = _Alloc::allocate(temp.m_length + 1);
		for (int i = 0; i < length; i++) {
			temp.m_str[i] = m_str[start + i];
		}
		temp.m_str[temp.m_length] = endChar;
		return temp;
	}
	template<class T, class _Alloc>
	inline int basic_string<T, _Alloc>::compare(const T* str) {//�Ƚ��ַ���
		basic_string temp(str);
		return compare(temp);
	}
	template<class T, class _Alloc>
	inline int basic_string<T, _Alloc>::compare(const basic_string& str) {//�Ƚ��ַ���
		if (m_length > str.m_length) {
			return 1;
		}else if (m_length < str.m_length) {
			return -1;
		}else {
			for (int i = 0; i < m_length; i++) {
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
		m_str = _Alloc::allocate(1);
		m_str[0] = endChar;
		m_capacity = 0;
	}
	template<class T, class _Alloc>
	inline bool basic_string<T, _Alloc>::startwith(const T* str) {//�ж��Ƿ���str��ͷ
		int length = strlength(str);
		for (int i = 0; i < length; i++) {
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
			int newSize = m_capacity * 2;
			T* temp = _Alloc::allocate(newSize);
			strcopy(m_str, temp);
			releasebuffer();//�ͷ�ԭ�����ڴ�
			m_str = temp;
			m_capacity = newSize;
		}
	}
	template<class T, class _Alloc>
	inline void basic_string<T, _Alloc>::resize(size_t nSize) {
		if (nSize > m_length) {
			T* temp = _Alloc::allocate(nSize + 1);
			strcopy(m_str, temp);
			releasebuffer();
			m_str = temp;
			m_length = nSize;
		}
	}
	template<class T, class _Alloc>
	inline void basic_string<T, _Alloc>::tolower() {
		auto diff = lower - upper;
		for (int i = 0; i < m_length; i++) {
			if (m_str[i] >= upper && m_str[i] <= upper + 25) {
				m_str[i] += diff;
			}
		}
	}
	template<class T, class _Alloc>
	inline void basic_string<T, _Alloc>::toupper() {
		const auto diff = lower - upper;
		for (int i = 0; i < m_length; i++) {
			if (m_str[i] >= lower && m_str[i] <= lower + 25) {
				m_str[i] -= diff;
			}
		}
	}
	template<class T, class _Alloc>
	inline T basic_string<T, _Alloc>::at(int index) {
		if (index < 0 || index >= m_length) return endChar;
		return m_str[index];
	}
	template<class T, class _Alloc>
	inline basic_string<T, _Alloc>::~basic_string() { releasebuffer(); }
	template<class T, class _Alloc>
	template<typename ...elem,typename U>
	inline basic_string<T, _Alloc>::basic_string(elem ...args)
	{
		m_length = sizeof...(args);
		m_str = _Alloc::allocate(m_length + 1);
		m_capacity = m_length;
		int i = 0;
		((m_str[i++] = *args), ...);
		m_str[m_length] = endChar;
	}
	template<class T, class _Alloc>
	template<class Z, class U>
	inline bool basic_string<T, _Alloc>::operator==(Z str) {
		if constexpr (nonstd::is_same_v<Z, basic_string>) {
			if (m_length != str.length()) return false;
			for (int i = 0; i < m_length; i++) {//�Ƚ��ַ���
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
			T* temp = _Alloc::allocate(m_length + 1);
			strcopy(m_str, temp);
			releasebuffer();
			m_str = temp;
			m_capacity = m_length;
		}
		else {
			resever(m_length);
		}
	}
	using string = basic_string<char, allcator<char>>;
	using wstring = basic_string<wchar_t, allcator<wchar_t>>;
}