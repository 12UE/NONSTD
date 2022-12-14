#pragma once
namespace nonstd {
	//经典的快速排序算法
	template<class Iter,class Comp>
	void qsort(Iter first,Iter last,Comp comp){
		using T = typename Iter::value_type;
		if (first == last) return;
		auto pivot = *first;
		auto i = first;
		auto j = last - 1;
		while (i < j) {
			while (i < j && (comp(*j, pivot))) --j;
			*i = *j;
			while (i < j && (!comp(*i,pivot))) ++i;
			*j = *i;
		}
		*i = pivot;
		qsort(first, i, comp);
		qsort(i + 1, last, comp);
	}
	//默认是nonstd::greater
	template<class Iter,class Comp= nonstd::greater_equal<typename Iter::value_type>>
	void sort(Iter first, Iter last) {
		nonstd::qsort(first, last, Comp{});
	}

	template<class Iter,class Comp>
	void sort(Iter first, Iter last, Comp _Pre) {
		nonstd::qsort(first, last, _Pre);
	}
	
	template<class InputIt, class UnaryFunction>
	UnaryFunction for_each(InputIt first, InputIt last, UnaryFunction f){
		for (; first != last; ++first) {
			f(*first);
		}
		return f; // C++11 起隐式移动
	}
	template<class InputIt, class Size, class UnaryFunction>
	InputIt for_each_n(InputIt first, Size n, UnaryFunction f)
	{
		for (Size i = 0; i < n; ++first, (void) ++i) {
			f(*first);
		}
		return first;
	}
	template<class InputIt, class T>
	typename iterator_traits<InputIt>::difference_type
		count(InputIt first, InputIt last, const T& value)
	{
		typename iterator_traits<InputIt>::difference_type ret = 0;
		for (; first != last; ++first) {
			if (*first == value) {
				ret++;
			}
		}
		return ret;
	}
	template<class InputIt, class OutputIt>
	OutputIt move(InputIt first, InputIt last, OutputIt d_first)
	{
		while (first != last) {
			*d_first++ = nonstd::move(*first++);
		}
		return d_first;
	}
	template<class InputIt, class UnaryPredicate>
	typename iterator_traits<InputIt>::difference_type
		count_if(InputIt first, InputIt last, UnaryPredicate p)
	{
		typename iterator_traits<InputIt>::difference_type ret = 0;
		for (; first != last; ++first) {
			if (p(*first)) {
				ret++;
			}
		}
		return ret;
	}
	template <typename Iter, typename T>
	Iter find(Iter first, Iter last, const T& value) {
		for (; first != last; ++first) {
			if (*first == value) {
				return first;
			}
		}
		return last;
	}

	template <typename Iter, typename Pred>
	Iter find_if(Iter first, Iter last, Pred pred) {
		for (; first != last; ++first) {
			if (pred(*first)) {
				return first;
			}
		}
		return last;
	}
	template<class InputIt, class ForwardIt>
	InputIt find_first_of(InputIt first, InputIt last,
		ForwardIt s_first, ForwardIt s_last)
	{
		for (; first != last; ++first) {
			for (ForwardIt it = s_first; it != s_last; ++it) {
				if (*first == *it) {
					return first;
				}
			}
		}
		return last;
	}
	template<class InputIt, class ForwardIt, class BinaryPredicate>
	InputIt find_first_of(InputIt first, InputIt last,
		ForwardIt s_first, ForwardIt s_last,
		BinaryPredicate p)
	{
		for (; first != last; ++first) {
			for (ForwardIt it = s_first; it != s_last; ++it) {
				if (p(*first, *it)) {
					return first;
				}
			}
		}
		return last;
	}
	template <typename Iter1, typename Iter2>
	Iter1 search(Iter1 first1, Iter1 last1, Iter2 first2, Iter2 last2) {
		if (first2 == last2) return first1;
		while (first1 != last1) {
			Iter1 it1 = first1;
			Iter2 it2 = first2;
			while (*it1 == *it2) {
				++it1;
				++it2;
				if (it2 == last2) return first1;
				if (it1 == last1) return last1;
			}
			++first1;
		}
		return last1;
	}

	template <typename Iter, typename Size, typename T>
	Iter search_n(Iter first, Iter last, Size count, const T& value) {
		if (count == 0) return first;
		first = nonstd::find(first, last, value);
		while (first != last) {
			Size n = count - 1;
			Iter i = first;
			++i;
			while (i != last && n != 0 && *i == value) {
				++i;
				--n;
			}
			if (n == 0) return first;
			else first = nonstd::find(i, last, value);
		}
		return last;
	}


	//写一个 std::min
	template <typename T>
	const T& Min(const T& a, const T& b) {
		return a < b ? a : b;
	}
	//写一个 std::max
	template <typename T>
	const T& Max(const T& a, const T& b) {
		return a > b ? a : b;
	}
	//写一个clamp
	template <typename T>
	const T& Clamp(const T& v, const T& lo, const T& hi) {
		return nonstd::Max(lo, nonstd::Min(v, hi));
	}
	//写一个swap
	template <typename T>
	void Swap(T& a, T& b) {
		T tmp = a;
		a = b;
		b = tmp;
	}
	//写一个 min_element
	template <typename Iter>
	Iter min_element(Iter first, Iter last) {
		if (first == last) return last;
		Iter smallest = first;
		while (++first != last) {
			if (*first < *smallest) {
				smallest = first;
			}
		}
		return smallest;
	}
	template <typename InputIt, typename OutputIt>
	OutputIt copy(InputIt first, InputIt last, OutputIt result) {
		while (first != last) {
			*result = *first;
			++result;
			++first;
		}
		return result;
	}
	template <typename InputIt, typename Size, typename OutputIt>
	OutputIt copy_n(InputIt first, Size count, OutputIt result) {
		if (count > 0) {
			*result++ = *first;
			for (Size i = 1; i < count; ++i) {
				*result++ = *++first;
			}
		}
		return result;
	}
	//写一个copy_if
	template <typename InputIt, typename OutputIt, typename UnaryPredicate>
	OutputIt copy_if(InputIt first, InputIt last, OutputIt result, UnaryPredicate pred) {
		while (first != last) {
			if (pred(*first)) {
				*result = *first;
				++result;
			}
			++first;
		}
		return result;
	}
	//写一个transform
	template <typename InputIt, typename OutputIt, typename UnaryOperation>
	OutputIt transform(InputIt first, InputIt last, OutputIt result, UnaryOperation op) {
		while (first != last) {
			*result = op(*first);
			++result;
			++first;
		}
		return result;
	}
	//写一个 remove
	template <typename Iter, typename T>
	Iter remove(Iter first, Iter last, const T& value) {
		Iter result = first;
		while (first != last) {
			if (!(*first == value)) {
				*result = *first;
				++result;
			}
			++first;
		}
		return result;
	}
	//写一个 remove_if
	template <typename Iter, typename Pred>
	Iter remove_if(Iter first, Iter last, Pred pred) {
		Iter result = first;
		while (first != last) {
			if (!pred(*first)) {
				*result = *first;
				++result;
			}
			++first;
		}
		return result;
	}
	template<class ForwardIt>
	ForwardIt unique(ForwardIt first, ForwardIt last)
	{
		if (first == last)
			return last;

		ForwardIt result = first;
		while (++first != last)
			if (!(*result == *first) && ++result != first)
				*result = nonstd::move(*first);

		return ++result;
	}
	template<class ForwardIt, class BinaryPredicate>
	ForwardIt unique(ForwardIt first, ForwardIt last, BinaryPredicate p)
	{
		if (first == last)
			return last;

		ForwardIt result = first;
		while (++first != last)
			if (!p(*result, *first) && ++result != first)
				*result = nonstd::move(*first);

		return ++result;
	}
	//写一个reverse
	template <typename Iter>
	void reverse(Iter first, Iter last) {
		while ((first != last) && (first != --last)) {
			nonstd::Swap(*first++, *last);
		}
	}
	//写一个is_sorted
	template <typename Iter,class Comp>
	bool is_sorted(Iter first, Iter last, Comp comp) {
		if (first == last) return true;
		Iter next = first;
		while (++next != last) {
			if (comp(*next, *first)) return false;
			first = next;
		}
		return true;
	}
	
	template<class Iter>
	bool is_sorted(Iter first, Iter last) {
		return nonstd::is_sorted(first, last, nonstd::less<typename Iter::value_type>());
	}

	//实现一个std::advance
	template <typename Iter, typename Distance>
	void advance(Iter& i, Distance n) {
		if (n >= 0) {
			while (n--) ++i;
		}
		else {
			while (n++) --i;
		}
	}

	//写一个std::distance
	template <typename Iter>
	typename nonstd::iterator_traits<Iter>::difference_type distance(Iter first, Iter last) {
		typename nonstd::iterator_traits<Iter>::difference_type n = 0;
		while (first != last) {
			++first;
			++n;
		}
		return n;
	}
	//二分搜索操作(基于一排序范围)
	//写一个lower_bound
	template<class ForwardIt, class T>
	ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T& value) {
		ForwardIt it;
		typename nonstd::iterator_traits<ForwardIt>::difference_type count, step;
		count = nonstd::distance(first, last);
		while (count > 0) {
			it = first;
			step = count / 2;
			nonstd::advance(it, step);

			if (*it < value) {
				first = ++it;
				count -= step + 1;
			}
			else
				count = step;
		}
		return first;
	}
	template<class ForwardIt, class T, class Compare>
	ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T& value, Compare comp) {
		ForwardIt it;
		typename nonstd::iterator_traits<ForwardIt>::difference_type count, step;
		count = nonstd::distance(first, last);

		while (count > 0)
		{
			it = first;
			step = count / 2;
			nonstd::advance(it, step);

			if (comp(*it, value))
			{
				first = ++it;
				count -= step + 1;
			}
			else
				count = step;
		}

		return first;
	}
	template<class ForwardIt, class T>
	ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T& value)
	{
		ForwardIt it;
		typename nonstd::iterator_traits<ForwardIt>::difference_type count, step;
		count = nonstd::distance(first, last);

		while (count > 0)
		{
			it = first;
			step = count / 2;
			nonstd::advance(it, step);

			if (!(value < *it))
			{
				first = ++it;
				count -= step + 1;
			}
			else
				count = step;
		}

		return first;
	}
	template<class ForwardIt, class T, class Compare>
	ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T& value, Compare comp)
	{
		ForwardIt it;
		typename nonstd::iterator_traits<ForwardIt>::difference_type count, step;
		count = nonstd::distance(first, last);

		while (count > 0)
		{
			it = first;
			step = count / 2;
			nonstd::advance(it, step);

			if (!comp(value, *it))
			{
				first = ++it;
				count -= step + 1;
			}
			else
				count = step;
		}

		return first;
	}
	template<class ForwardIt, class T>
	bool binary_search(ForwardIt first, ForwardIt last, const T& value)
	{
		first = nonstd::lower_bound(first, last, value);
		return (!(first == last) && !(value < *first));
	}
	template<class ForwardIt, class T, class Compare>
	bool binary_search(ForwardIt first, ForwardIt last, const T& value, Compare comp)
	{
		first = nonstd::lower_bound(first, last, value, comp);
		return (!(first == last) && !(comp(value, *first)));
	}
	template<class InputIt1, class InputIt2, class OutputIt>
	OutputIt merge(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2,
		OutputIt d_first)
	{
		for (; first1 != last1; ++d_first) {
			if (first2 == last2) {
				return nonstd::copy(first1, last1, d_first);
			}
			if (*first2 < *first1) {
				*d_first = *first2;
				++first2;
			}
			else {
				*d_first = *first1;
				++first1;
			}
		}
		return nonstd::copy(first2, last2, d_first);
	}
	template<class InputIt1, class InputIt2,
		class OutputIt, class Compare>
	OutputIt merge(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2,
		OutputIt d_first, Compare comp)
	{
		for (; first1 != last1; ++d_first) {
			if (first2 == last2) {
				return nonstd::copy(first1, last1, d_first);
			}
			if (comp(*first2, *first1)) {
				*d_first = *first2;
				++first2;
			}
			else {
				*d_first = *first1;
				++first1;
			}
		}
		return nonstd::copy(first2, last2, d_first);
	}
	template<class ForwardIt, class Compare>
	ForwardIt min_element(ForwardIt first, ForwardIt last, Compare comp)
	{
		if (first == last)
			return last;

		ForwardIt smallest = first;
		++first;

		for (; first != last; ++first)
			if (comp(*first, *smallest))
				smallest = first;

		return smallest;
	}
	template<class ForwardIt, class Compare>
	nonstd::pair<ForwardIt, ForwardIt>
		minmax_element(ForwardIt first, ForwardIt last, Compare comp)
	{
		auto min = first, max = first;

		if (first == last || ++first == last)
			return { min, max };

		if (comp(*first, *min)) {
			min = first;
		}
		else {
			max = first;
		}

		while (++first != last) {
			auto i = first;
			if (++first == last) {
				if (comp(*i, *min)) min = i;
				else if (!(comp(*i, *max))) max = i;
				break;
			}
			else {
				if (comp(*first, *i)) {
					if (comp(*first, *min)) min = first;
					if (!(comp(*i, *max))) max = i;
				}
				else {
					if (comp(*i, *min)) min = i;
					if (!(comp(*first, *max))) max = first;
				}
			}
		}
		return { min, max };
	}
	template<class ForwardIt>
	nonstd::pair<ForwardIt, ForwardIt>
		minmax_element(ForwardIt first, ForwardIt last)
	{
		using value_type = typename nonstd::iterator_traits<ForwardIt>::value_type;
		return nonstd::minmax_element(first, last, nonstd::less<value_type>());
	}
	template<class ForwardIt>
	ForwardIt max_element(ForwardIt first, ForwardIt last)
	{
		if (first == last)
			return last;

		ForwardIt largest = first;
		++first;

		for (; first != last; ++first)
			if (*largest < *first)
				largest = first;

		return largest;
	}
	template<class ForwardIt, class Compare>
	ForwardIt max_element(ForwardIt first, ForwardIt last, Compare comp)
	{
		if (first == last)
			return last;

		ForwardIt largest = first;
		++first;

		for (; first != last; ++first)
			if (comp(*largest, *first))
				largest = first;

		return largest;
	}
	template<class OutputIt, class Size, class T>
	OutputIt fill_n(OutputIt first, Size count, const T& value)
	{
		for (Size i = 0; i < count; ++i) {
			*first++ = value;
		}
		return first;
	}
	template<class InputIt1, class InputIt2>
	bool equal(InputIt1 first1, InputIt1 last1,
		InputIt2 first2)
	{
		for (; first1 != last1; ++first1, ++first2) {
			if (!(*first1 == *first2)) {
				return false;
			}
		}
		return true;
	}
	template<class InputIt1, class InputIt2, class BinaryPredicate>
	bool equal(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, BinaryPredicate p)
	{
		for (; first1 != last1; ++first1, ++first2) {
			if (!p(*first1, *first2)) {
				return false;
			}
		}
		return true;
	}
	template<class InputIt1, class InputIt2>
	bool equal(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2) {
		if (nonstd::distance(first1, last1) != nonstd::distance(first2, last2))return false;

		for (; first1 != last1, first2 != last2; ++first1, ++first2) {
			if (!(*first1 == *first2)) {
				return false;
			}
		}
		return true;
	}
	template< class InputIt1, class InputIt2, class BinaryPredicate >
	bool equal(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2,
		BinaryPredicate p) {
		if (nonstd::distance(first1, last1) != nonstd::distance(first2, last2)) {
			return false;
		}
		for (; first1 != last1, first2 != last2; ++first1, ++first2) {
			if (!p(*first1, *first2)) return false;
		}
		return true;
	}
	//数值算法
	template<class ForwardIterator, class T>
	constexpr // C++20 起
		void iota(ForwardIterator first, ForwardIterator last, T value)//用从起始值开始连续递增的值填充一个范围 
	{
		while (first != last) {
			*first++ = value;
			++value;
		}
	}
	template<class InputIt, class T>
	constexpr // C++20 起
		T accumulate(InputIt first, InputIt last, T init)
	{
		for (; first != last; ++first)
			init = nonstd::move(init) + *first; // C++20 起有 std::move

		return init;
	}
	template<class InputIt, class T, class BinaryOperation>
	constexpr // C++20 起
		T accumulate(InputIt first, InputIt last, T init, BinaryOperation op)
	{
		for (; first != last; ++first)
			init = op(nonstd::move(init), *first); // C++20 起有 std::move

		return init;
	}
	template<class InputIt, class OutputIt>
	constexpr // C++20 起
		OutputIt adjacent_difference(InputIt first, InputIt last, OutputIt d_first)
	{
		if (first == last)
			return d_first;

		typedef typename nonstd::iterator_traits<InputIt>::value_type value_t;
		value_t acc = *first;
		*d_first = acc;

		while (++first != last)
		{
			value_t val = *first;
			*++d_first = val - nonstd::move(acc); // C++20 起有 std::move
			acc = nonstd::move(val);
		}

		return ++d_first;
	}
	template<class InputIt, class OutputIt, class BinaryOperation>
	constexpr // C++20 起
		OutputIt adjacent_difference(InputIt first, InputIt last,
			OutputIt d_first, BinaryOperation op)
	{
		if (first == last)
			return d_first;

		typedef typename nonstd::iterator_traits<InputIt>::value_type value_t;
		value_t acc = *first;
		*d_first = acc;

		while (++first != last)
		{
			value_t val = *first;
			*++d_first = op(val, nonstd::move(acc)); // C++20 起有 std::move
			acc = nonstd::move(val);
		}

		return ++d_first;
	}
	template<class InputIt, class OutputIt>
	constexpr // C++20 起
		OutputIt partial_sum(InputIt first, InputIt last, OutputIt d_first)
	{
		if (first == last)
			return d_first;

		typename nonstd::iterator_traits<InputIt>::value_type sum = *first;
		*d_first = sum;

		while (++first != last)
		{
			sum = nonstd::move(sum) + *first; // C++20 起有 std::move
			*++d_first = sum;
		}

		return ++d_first;

		// 或 C++14 起：
		// return std::partial_sum(first, last, d_first, std::plus<>());
	}
	template<class InputIt, class OutputIt, class BinaryOperation>
	constexpr // C++20 起
		OutputIt partial_sum(InputIt first, InputIt last,
			OutputIt d_first, BinaryOperation op)
	{
		if (first == last)
			return d_first;

		typename nonstd::iterator_traits<InputIt>::value_type sum = *first;
		*d_first = sum;

		while (++first != last)
		{
			sum = op(nonstd::move(sum), *first); // C++20 起有 std::move
			*++d_first = sum;
		}

		return ++d_first;
	}
	template<class T, class... Args>
	constexpr T* construct_at(T* p, Args&&... args) {
		return ::new (const_cast<void*>(static_cast<const volatile void*>(p)))
			T(nonstd::forward<Args>(args)...);
	}

	template<class ForwardIt>
	ForwardIt rotate(ForwardIt first, ForwardIt n_first, ForwardIt last){
		if (first == n_first) return last;
		if (n_first == last) return first;

		auto read = n_first;
		auto write = first;
		auto next_read = first; // "read" 撞击 "last" 时的读取位置

		while (read != last) {
			if (write == next_read) next_read = read; // 跟踪 "first" 所至
			nonstd::iter_swap(write++, read++);
		}

		// 旋转剩余序列到位置中
		rotate(write, next_read, last);
		return write;
	}

}