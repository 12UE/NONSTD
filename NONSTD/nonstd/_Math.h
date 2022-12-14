#pragma once
namespace nonstd {
#ifndef _HUGE_ENUF
#define _HUGE_ENUF  1e+300  // _HUGE_ENUF*_HUGE_ENUF must overflow
#endif
#define INFINITY   ((float)(_HUGE_ENUF * _HUGE_ENUF))
#define NAN        (-(float)(INFINITY * 0.0F))
    inline bool isnan(double x) {
        return x != x;
    }
    inline double floor(double x) {
        if (x >= 0) {
            return (int)x;
        }
        else {
            return (int)x - 1;
        }
    }
    inline double pow(double base, double exponent) {
        // Check for special cases
        if (base == 0.0 && exponent <= 0.0) {
            return NAN; // domain error
        }
        else if (base == 0.0) {
            return 0.0;
        }
        else if (exponent == 0.0) {
            return 1.0;
        }
        else if (isnan(base) || isnan(exponent)) {
            return NAN; // domain error
        }

        // Handle negative exponent
        int invert = 0;
        if (exponent < 0.0) {
            invert = 1;
            exponent = -exponent;
        }

        // Compute the result using repeated multiplication
        double result = 1.0;
        while (exponent > 0.0) {
            if ((exponent / 2.0) == floor(exponent / 2.0)) {
                // exponent is even, so square the base
                base = base * base;
                exponent = exponent / 2.0;
            }
            else {
                // exponent is odd, so multiply the result by the base
                result = result * base;
                exponent = exponent - 1.0;
            }
        }

        // Invert the result if necessary
        if (invert) {
            result = 1.0 / result;
        }

        return result;
    }
   
    inline double _sqrt(double x) {
        if (x < 0) {
            return NAN;
        }
        else if (x == 0) {
            return 0;
        }
        else if (x == INFINITY) {
            return INFINITY;
        }
        else if (isnan(x)) {
            return NAN;
        }

        // Use the Karatsuba algorithm to compute the square root
        double result = x;
        double lastResult = 0.0;
        while (result != lastResult) {
            lastResult = result;
            result = (result + x / result) / 2.0;
        }
        return result;
    }
    __forceinline float sqrt(float number) { //卡马克 快速平方根
        if (number < 0) {
            return NAN;
        }else if (number == 0) {
            return 0;
        }else if (number == INFINITY) {
            return INFINITY;
        }else if (isnan(number)) {
            return NAN;
        }
        float x2 = number * 0.5F;
        float y = number;
        long i = *(long*)&y;
        i = 0x5f3759df - (i >> 1);
        y = *(float*)&i;
        y = y * (1.5f - (x2 * y * y));   // 1st iteration
        y = y * (1.5f - (x2 * y * y));   // 2rd iteration this can be remove
        return number * y;
    }
}