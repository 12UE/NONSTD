#pragma once
namespace nonstd {
    typedef long _clock_t;

    static long long source_frequency;

    static long long start_count = -1;

    constexpr auto CLOCKS_PER_SEC_T = ((clock_t)1000);

    inline bool QueryPerformanceFrequency(LARGE_INTEGER* lpFrequency)
    {
        if (!lpFrequency) return false;

        KeQueryPerformanceCounter(lpFrequency);

        return true;
    }


    inline bool QueryPerformanceCounter(LARGE_INTEGER* lpPerformanceCount)
    {
        if (!lpPerformanceCount) return false;

        *lpPerformanceCount = KeQueryPerformanceCounter(nullptr);

        return true;
    }
    /*
    * 该函数首先计算出在高精度计时器的频率下，相对于 CLOCKS_PER_SEC（表示 clock() 函数返回的单位）的比例，
    *
    并将高精度计时器的计数值乘以这个比例。这样可以将高精度计时器的计数值转换为与 CLOCKS_PER_SEC 相同的单位。

    然后，该函数再用高精度计时器的计数值除以频率，并将余数乘上 CLOCKS_PER_SEC 再除以频率，得到转换后的计数值。

    最后，该函数返回转换后的计数值。
    */
    inline static long long scale_count(long long count)
    {
        long long scaled_count = (count / source_frequency) * CLOCKS_PER_SEC;

        count %= source_frequency;

        scaled_count += (count * CLOCKS_PER_SEC) / source_frequency;

        return scaled_count;
    }
    /*
    * 该函数用于初始化高精度计时器，并在初始化失败时返回 0。

    该函数使用了 Windows API 中的 QueryPerformanceFrequency 和 QueryPerformanceCounter 函数来获取高精度计时器的频率和当前计数值。
    如果调用这两个函数失败，或者频率为 0，则说明初始化失败，函数返回 0。

    如果初始化成功，则函数会将频率和起始计数值保存到 source_frequency 和 start_count 变量中，并返回 0。
    */
    static bool IsInitialLized = false;
    
    inline int _initialize_clock() {
        LARGE_INTEGER local_frequency;
        LARGE_INTEGER local_start_count;
        if (!QueryPerformanceFrequency(&local_frequency) ||
            !QueryPerformanceCounter(&local_start_count) ||
            local_frequency.QuadPart == 0) {
            source_frequency = -1;
            start_count = -1;
            return 0;
        }
        source_frequency = local_frequency.QuadPart;
        start_count = local_start_count.QuadPart;
        IsInitialLized = true;
        return 0;
    }
    /*
    * 这段代码实现了一个名为clock的内联函数，它用于获取当前时间。首先，它检查一个静态布尔变量IsInitialLized，如果为false，
    则调用另一个函数_initialize_clock来初始化相关的变量。然后，它调用QueryPerformanceCounter函数来获取当前的时间值，
    并使用scale_count函数对这个值进行缩放。最后，它将缩放后的结果转换为clock_t类型并返回。
    */
    
    static int const _clock_init = _initialize_clock();//会在加载时调用
    
    inline clock_t __cdecl clock(){
        
        if (!IsInitialLized) {
            _initialize_clock();
            IsInitialLized = true;
        }

        if (start_count == -1)
            return -1;

        LARGE_INTEGER current_count;
        if (!QueryPerformanceCounter(&current_count))
            return -1;

        long long const result = current_count.QuadPart - start_count;
        if (result < 0)
            return -1;

        long long const scaled_result = scale_count(result);

        if (scaled_result > LONG_MAX)
            return -1;

        return static_cast<clock_t>(scaled_result);
    }
	//初始化 时钟
	
	inline time_t __cdecl time(time_t* time)
	{
        LARGE_INTEGER system_time{};
		KeQuerySystemTime(&system_time);
		time_t const result = system_time.QuadPart / 10000000 - 11644473600;
		if (time)
			*time = result;
		return result;
	}

}
