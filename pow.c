#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef uint64_t (*powFunc)(unsigned int, unsigned int);


//UINT64_C(1) means to define a constant of type uint64_t
//#define UINT64_C(value)   __CONCAT(value, ULL)
void printRet(unsigned int x, unsigned int n, powFunc callPow)
{
    printf("%u ^ %u = %lu \n",x,n,callPow(x,n));
}

uint64_t pow1(unsigned int x, unsigned int n)
{
    uint64_t ret = 1;
    while(n--){
        ret*=x;
    }
    return ret;
}

uint64_t pow2(unsigned int x, unsigned int n)
{
    if(n==0)return 1;
    if(n&1){
        return x * pow2(x,(n-1)/2) * pow2(x,(n-1)/2);
    }else{
        return pow2(x, n/2) * pow2(x, n/2);
    }
}

uint64_t pow2_1(unsigned int x, unsigned int n)
{
    if(n==0)return 1;
    if(n&1){
        return x * pow2_1(x*x,(n-1)/2);
    }else{
        return pow2_1(x*x, n/2);
    }
}

uint64_t pow3(unsigned int x, unsigned int n)
{
    if(n==0)return 1;

    return (n & UINT64_C(1)) * x * pow3(x,(n-1)/2) * pow3(x,(n-1)/2) + !(n & UINT64_C(1)) * pow3(x, n/2) * pow3(x, n/2);
}

uint64_t pow3_1(unsigned int x, unsigned int n)
{
    if(n==0)return 1;

    return (n & UINT64_C(1)) * x * pow3_1(x*x,(n-1)/2) + !(n & UINT64_C(1)) * pow3_1(x*x, n/2);
}
uint64_t pow4(unsigned int x, unsigned int n)
{
    if(n==0)return 1;
    uint64_t k = pow4(x*x,n>>1);
    return (n&1) ? k * x : k;
}
uint64_t pow4_1(unsigned int x, unsigned int n)
{
    if(n==0)return 1;
    uint64_t k = pow4_1(x*x,n>>1);
    return k * x * (n & UINT64_C(1)) + k * !(n & UINT64_C(1));
}

//iterative version
uint64_t pow5(unsigned int x, unsigned int n)
{
    if(n==0)return 1;
    uint64_t r = 1;
    while(n > 1){
        if(n&1){
            r *= x;
            x *= x;
            n = (n - 1)/2;
        }else{
            x *= x;
            n = n/2;
        }
    }
    return r * x;
}

uint64_t pow5_1(unsigned int x, unsigned int n)
{
    uint64_t r = 1;
    while(n){
        if(n&1){
            r *= x;
        }
        x *= x;
        n = (n - ( n & UINT64_C(1))) >> 1;
    }
    return r;
}
uint64_t pow5_2(unsigned int x, unsigned int n)
{
    uint64_t r = 1;
    while(n){
        r = r * x * (n & UINT64_C(1)) + r * !(n & UINT64_C(1));
        x *= x;
        n = (n - ( n & UINT64_C(1))) >> 1;
    }
    return r;
}


int main(void)
{
    int x = 10, n = 19, n1 = 15;
    printRet(x,n,pow1);
    printRet(x,n,pow2);
    printRet(x,n1,pow2_1);
    printRet(x,n,pow3);
    printRet(x,n1,pow3_1);
    printRet(x,n1,pow4);//care for overflow when do (x * x)
    printRet(x,n1,pow4_1);
    printRet(x,n1,pow5);
    printRet(x,n1,pow5_1);
    printRet(x,n1,pow5_2);
    return 0;
}
