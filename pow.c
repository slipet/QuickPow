#include "pow.h"

// UINT64_C(1) means to define a constant of type uint64_t
//#define UINT64_C(value)   __CONCAT(value, ULL)

uint64_t pow1(unsigned int x, unsigned int n)
{
    uint64_t ret = 1;
    while (n--) {
        ret *= x;
    }
    return ret;
}

uint64_t pow2(unsigned int x, unsigned int n)
{
    if (n == 0)
        return 1;
    if (n & 1) {
        return x * pow2(x, (n - 1) / 2) * pow2(x, (n - 1) / 2);
    } else {
        return pow2(x, n / 2) * pow2(x, n / 2);
    }
}

uint64_t pow2_1(unsigned int x, unsigned int n)
{
    if (n == 0)
        return 1;
    if (n & 1) {
        return x * pow2_1(x * x, (n - 1) / 2);
    } else {
        return pow2_1(x * x, n / 2);
    }
}
// Branchless version
uint64_t pow3(unsigned int x, unsigned int n)
{
    if (n == 0)
        return 1;
    uint64_t k = pow3(x * x, n >> 1);
    return (n & 1) ? k * x : k;
}

uint64_t pow3_1(unsigned int x, unsigned int n)
{
    if (n == 0)
        return 1;
    uint64_t k = pow3_1(x * x, n >> 1);
    return (n & UINT64_C(1)) * x * k + !(n & UINT64_C(1)) * k;
}


// iterative version
uint64_t pow4(unsigned int x, unsigned int n)
{
    if (n == 0)
        return 1;
    uint64_t r = 1;
    while (n > 1) {
        if (n & 1) {
            r *= x;
            x *= x;
            n = (n - 1) / 2;
        } else {
            x *= x;
            n = n / 2;
        }
    }
    return r * x;
}

uint64_t pow4_1(unsigned int x, unsigned int n)
{
    uint64_t r = 1;
    while (n) {
        r = r * x * (n & UINT64_C(1)) + r * !(n & UINT64_C(1));
        x *= x;
        n = (n - (n & UINT64_C(1))) >> 1;
    }
    return r;
}


uint64_t pow5(unsigned int x, unsigned int n)
{
    int stack[100];
    int top = -1;
    while (n) {
        stack[++top] = n;
        n >>= 1;
    }
    uint64_t r = 1;
    while (top >= 0) {
        n = stack[top--];
        r *= r;
        if (n & 1) {
            r *= x;
        }
    }
    return r;
}

uint64_t pow5_1(unsigned int x, unsigned int n)
{
    int hi = 31 - __builtin_clz(n);

    uint64_t r = 1;
    for (int i = hi; i >= 0; --i) {
        r *= r;
        r = r * x * !!(n & (UINT64_C(1) << i)) +
            r * !!!(n & (UINT64_C(1) << i));
    }
    return r;
}
