#pragma once
namespace nonstd {
	template<class T, nonstd::size_t N>
	class array {
	private:
		
	
		using value_type = T;
		using size_type = nonstd::size_t;
		using  difference_type = nonstd::ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using iterator=RandomAccessIterator<T>;
		//成员函数
		//构造函数
		array() = default;
		array(const array&) = default;
		array(array&&) = default;
		array& operator=(const array&) = default;
		array& operator=(array&&) = default;
		~array() = default;
		//构造函数
		array(const T& val) {
			for (auto& x : _data) {
				x = val;
			}
		}
		//at
		reference at(size_type pos) {
			if (pos >= size()) {
				DbgPrint("out of range\n");
			}
			return _data[pos];
		}
		//[]
		reference operator[](size_type pos) {
			if (pos >= size()) {
				DbgPrint("out of range\n");
			}
			return _data[pos];
		}
		//front
		reference front() {
			return _data[0];
		}
		//back
		reference back() {
			return _data[size() - 1];
		}
		//data
		pointer data() {
			return _data;
		}
		//empty
		bool empty() {
			return size() == 0;
		}
		//size
		size_type size() {
			return N;
		}
		//max_size
		size_type max_size() {
			return N;
		}
		//fill
		void fill(const T& val) {
			for (auto& x : _data) {
				x = val;
			}
		}
		//swap
		void swap(array& other) {
			for (size_type i = 0; i < size(); ++i) {
				nonstd::swap(_data[i], other._data[i]);
			}
		}
		//重载运算符
		//==
		bool operator==(const array& other) {
			for (size_type i = 0; i < size(); ++i) {
				if (_data[i] != other._data[i]) {
					return false;
				}
			}
			return true;
		}
		//!=
		bool operator!=(const array& other) {
			return !(*this == other);
		}
		//<
		bool operator<(const array& other) {
			for (size_type i = 0; i < size(); ++i) {
				if (_data[i] >= other._data[i]) {
					return false;
				}
			}
			return true;
		}
		//<=
		bool operator<=(const array& other) {
			for (size_type i = 0; i < size(); ++i) {
				if (_data[i] > other._data[i]) {
					return false;
				}
			}
			return true;
		}
		//>
		bool operator>(const array& other) {
			for (size_type i = 0; i < size(); ++i) {
				if (_data[i] <= other._data[i]) {
					return false;
				}
			}
			return true;
		}
		//>=
		bool operator>=(const array& other) {
			for (size_type i = 0; i < size(); ++i) {
				if (_data[i] < other._data[i]) {
					return false;
				}
			}
			return true;
		}
		//重载运算符
		//<=>
		//nonstd::strong_ordering operator<=>(const array& other) {
		//	for (size_type i = 0; i < size(); ++i) {
		//		if (_data[i] < other._data[i]) {
		//			return nonstd::strong_ordering::less;
		//		}
		//		else if (_data[i] > other._data[i]) {
		//			return nonstd::strong_ordering::greater;
		//		}
		//		std::less<int>
		//	}
		//	return nonstd::strong_ordering::equal;
		//}
	private:
		value_type _data[N];
	};
}