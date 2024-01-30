#include <iostream>
#include <cstdint>

using namespace std;

typedef uint64_t (*powFunc)(unsigned int, unsigned int);

void printRet(unsigned int x, unsigned int n, powFunc callPow)
{
    cout<<x<<" ^ "<<n<<" ="<<callPow(x,n)<<endl;
}

uint64_t pow1(unsigned int x, unsigned int n)
{
    unsigned int ret = 1;
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

uint64_t pow3(unsigned int x, unsigned int n)
{
    if(n==0)return 1;

    return (n&(uint64_t)1) * x * pow2(x,(n-1)/2) * pow2(x,(n-1)/2) + !(n&(uint64_t)1) * pow2(x, n/2) * pow2(x, n/2);
}

int main(void)
{
    printRet(10,19,pow2);
    
    return 0;
}