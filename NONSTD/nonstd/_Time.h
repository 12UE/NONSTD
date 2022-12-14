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
    * �ú������ȼ�����ڸ߾��ȼ�ʱ����Ƶ���£������ CLOCKS_PER_SEC����ʾ clock() �������صĵ�λ���ı�����
    *
    �����߾��ȼ�ʱ���ļ���ֵ��������������������Խ��߾��ȼ�ʱ���ļ���ֵת��Ϊ�� CLOCKS_PER_SEC ��ͬ�ĵ�λ��

    Ȼ�󣬸ú������ø߾��ȼ�ʱ���ļ���ֵ����Ƶ�ʣ������������� CLOCKS_PER_SEC �ٳ���Ƶ�ʣ��õ�ת����ļ���ֵ��

    ��󣬸ú�������ת����ļ���ֵ��
    */
    inline static long long scale_count(long long count)
    {
        long long scaled_count = (count / source_frequency) * CLOCKS_PER_SEC;

        count %= source_frequency;

        scaled_count += (count * CLOCKS_PER_SEC) / source_frequency;

        return scaled_count;
    }
    /*
    * �ú������ڳ�ʼ���߾��ȼ�ʱ�������ڳ�ʼ��ʧ��ʱ���� 0��

    �ú���ʹ���� Windows API �е� QueryPerformanceFrequency �� QueryPerformanceCounter ��������ȡ�߾��ȼ�ʱ����Ƶ�ʺ͵�ǰ����ֵ��
    �����������������ʧ�ܣ�����Ƶ��Ϊ 0����˵����ʼ��ʧ�ܣ��������� 0��

    �����ʼ���ɹ��������ὫƵ�ʺ���ʼ����ֵ���浽 source_frequency �� start_count �����У������� 0��
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
    * ��δ���ʵ����һ����Ϊclock�����������������ڻ�ȡ��ǰʱ�䡣���ȣ������һ����̬��������IsInitialLized�����Ϊfalse��
    �������һ������_initialize_clock����ʼ����صı�����Ȼ��������QueryPerformanceCounter��������ȡ��ǰ��ʱ��ֵ��
    ��ʹ��scale_count���������ֵ�������š�����������ź�Ľ��ת��Ϊclock_t���Ͳ����ء�
    */
    
    static int const _clock_init = _initialize_clock();//���ڼ���ʱ����
    
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
	//��ʼ�� ʱ��
	
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
