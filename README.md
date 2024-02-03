# Quick Pow



### Development Environment
<details>
    
```
Architecture:            x86_64
  CPU op-mode(s):        32-bit, 64-bit
  Address sizes:         48 bits physical, 48 bits virtual
  Byte Order:            Little Endian
CPU(s):                  12
  On-line CPU(s) list:   0-11
Vendor ID:               AuthenticAMD
  Model name:            AMD Ryzen 5 5600X 6-Core Processor
    CPU family:          25
    Model:               33
    Thread(s) per core:  2
    Core(s) per socket:  6
    Socket(s):           1
    Stepping:            0
    Frequency boost:     enabled
    CPU max MHz:         4650.2920
    CPU min MHz:         2200.0000
    BogoMIPS:            7399.91
    Flags:               fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush mmx fxsr sse sse2 ht syscall nx mmxext fxsr_opt pdpe1gb rdtscp lm constant_ts
                         c rep_good nopl nonstop_tsc cpuid extd_apicid aperfmperf rapl pni pclmulqdq monitor ssse3 fma cx16 sse4_1 sse4_2 movbe popcnt aes xsave avx f16c rdrand lahf_
                         lm cmp_legacy svm extapic cr8_legacy abm sse4a misalignsse 3dnowprefetch osvw ibs skinit wdt tce topoext perfctr_core perfctr_nb bpext perfctr_llc mwaitx cpb
                          cat_l3 cdp_l3 hw_pstate ssbd mba ibrs ibpb stibp vmmcall fsgsbase bmi1 avx2 smep bmi2 erms invpcid cqm rdt_a rdseed adx smap clflushopt clwb sha_ni xsaveopt
                          xsavec xgetbv1 xsaves cqm_llc cqm_occup_llc cqm_mbm_total cqm_mbm_local clzero irperf xsaveerptr rdpru wbnoinvd arat npt lbrv svm_lock nrip_save tsc_scale v
                         mcb_clean flushbyasid decodeassists pausefilter pfthreshold avic v_vmsave_vmload vgif v_spec_ctrl umip pku ospke vaes vpclmulqdq rdpid overflow_recov succor 
                         smca fsrm
Virtualization features: 
  Virtualization:        AMD-V
Caches (sum of all):     
  L1d:                   192 KiB (6 instances)
  L1i:                   192 KiB (6 instances)
  L2:                    3 MiB (6 instances)
  L3:                    32 MiB (1 instance)
NUMA:                    
  NUMA node(s):          1
  NUMA node0 CPU(s):     0-11
Vulnerabilities:         
  Gather data sampling:  Not affected
  Itlb multihit:         Not affected
  L1tf:                  Not affected
  Mds:                   Not affected
  Meltdown:              Not affected
  Mmio stale data:       Not affected
  Retbleed:              Not affected
  Spec rstack overflow:  Mitigation; safe RET, no microcode
  Spec store bypass:     Mitigation; Speculative Store Bypass disabled via prctl
  Spectre v1:            Mitigation; usercopy/swapgs barriers and __user pointer sanitization
  Spectre v2:            Mitigation; Retpolines, IBPB conditional, IBRS_FW, STIBP always-on, RSB filling, PBRSB-eIBRS Not affected
  Srbds:                 Not affected
  Tsx async abort:       Not affected
```
</details>

---

* slow version with time comlexity O(n):
```c
uint64_t pow1(unsigned int x, unsigned int n)
{
    uint64_t ret = 1;
    while(n--){
        ret*=x;
    }
    return ret;
}
```

We can reduce the time complexity of the pow compuation from O(n) to O(log n) by the following equation:
$$
k^n = 
\begin{cases}
( k ^ { \frac{n}{2} } ) ^ 2 & \text{if n is even} \\
k \cdot ( k ^ {\frac{n-1}{2}} ) ^ 2, & \text{if n is odd}
\end{cases}
$$

There are two ways to implement the O(log n) method:
### Top-down approach:
We can easily divide the problem by recursion to achieve the goal.
```c
uint64_t pow2(unsigned int x, unsigned int n)
{
    if(n==0)return 1;
    if(n&1){
        return x * pow2(x,(n-1)/2) * pow2(x,(n-1)/2);
    }else{
        return pow2(x, n/2) * pow2(x, n/2);
    }
}
```
We can find that there are duplicate function calls when breaking down the problem.
If we move the multiplication operation to the argument, we can further halve the computation time.
```c
uint64_t pow2_1(unsigned int x, unsigned int n)
{
    if(n==0)return 1;
    if(n&1){
        return x * pow2_1(x*x,(n-1)/2);
    }else{
        return pow2_1(x*x, n/2);
    }
}
```
Now, everything looks great! However, when we input x=10 and n=19 into our function, we get the result 1874919424000. The issue with the above function is that the multiplication "x * x" might cause overflow.
|Rounds |0       |1          |2           |3            |4     |
|:-----:|:------:|:---------:|:----------:|:-----------:|:----:|
|  n    |19      |9          |4           |2            |1     |
|  x    |10      |10^2       |10^4        |10^8         |10^16 |
|  x\*x |10\*10  |10^2 \*10^2|10^4 \* 10^4|10^8 \* 10^8 |10^16 \* 10^16|

From the table above, it is evident that in the 4th round, 10^16 exceeds the maximum value for an unsigned int variable, which is 4,294,967,295, resulting in an overflow.

Upon observing the code mentioned above, we square x regardless of whether n is odd or even. Thus, we can isolate the multiplication and save it as variable k. For the if-else statement, ternary operator can provide more elegant representation as following:

```c
uint64_t pow3(unsigned int x, unsigned int n)
{
    if (n == 0)
        return 1;
    uint64_t k = pow4(x * x, n >> 1);
    return (n & 1) ? k * x : k;
}
```
For further imporvement, we can convert the above method into a branchless version.

```c
uint64_t pow3_1(unsigned int x, unsigned int n)
{
    if (n == 0)
        return 1;
    uint64_t k = pow3_1(x * x, n >> 1);
    return (n & UINT64_C(1)) * x * k + !(n & UINT64_C(1)) * k;
}
```
* iterative version:
Now, we can convert the recursive approach into an iterative version. In common case, the iterative version usually utilize smaller memory space(the cost of function calls) and faster execution time(compiler optimization).

We can deduce the iterative way as follow(take $x^n = 7^{10}$ as example):

1. $x_0 = 7, n_0 = 10 \implies x^n = x_{0}^{n_0} = 7^{10}$
2. $x_1 = x_0^2 = 7^2, n_1 = \frac{n_0}{2} =5 \implies x_1^{n_1}= (x_{0}^2)^{n_1} = (7^2)^5$
3. $x_2 = x_1^2 = 7^4, n_2 = \frac{n_1-1}{2} = 2, r_2 = r_1 * x_1 = 1 * 7^2 \implies x^n = r_2 * x_2^{n_2}$
4. $x_3 = x_2^2 = 7^8, n_3 = \frac{n_2}{2} = 1\implies x_3^{n_3} = x_2^{2} = 7^8$
5. when $n = 1, r = 7^2, x = 7^8 \implies r*x = 7^{10}$


```c 
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
```
Upon reviewing the code above, we can eliminate the if statement and adjust the repetition condition to check the value of n. When n equals 1, we perform r = r * x, achieving the same result as the return value mentioned previously. Otherwise, we apply brachless tactics and bitwise operation to improve  

```c 
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
```
### Bottom up:

In top-down method, we halve n at each rounds.

$$n_0=10\stackrel{n=\frac{n}{2}}\longrightarrow\ n_1=5\stackrel{n=\frac{n-1}{2}}\longrightarrow \
n_2=2\stackrel{n=\frac{n}{2}}\longrightarrow 
n_3=1\stackrel{n=\frac{n-1}{2}}\longrightarrow 
n_4=0
$$

For the bottom-up approach, we need to perform the symmetric operation to that of the top-down method.

$$n_0=0\stackrel{n*2 + 1 }\longrightarrow\ n_1=1\stackrel{n*2}\longrightarrow \
n_2=2\stackrel{n*2+1}\longrightarrow 
n_3=5\stackrel{n*2}\longrightarrow 
n_4=10
$$

We can generalize the transformation above as:
$$
n_{i+1} = 
\begin{cases}
2n_i & \text{if $n_{i+1}$ is even} \\
2n_i+1, & \text{if $n_{i+1}$ is odd}
\end{cases}
$$

$$
x^{n_{i+1}} = 
\begin{cases}
(x^{n_i})^2 & \text{if $n_{i+1}$ is even} \\
x*(x^{n_i})^2, & \text{if $n_{i+1}$ is odd}
\end{cases}
$$

However, there is a problem to determin $n_{i+1}$ if is odd or even. In order to track the variation of n, we can use a stack to simulate recursive function call.

```c
uint64_t pow5(unsigned int x, unsigned int n)
{
    int stack[100];
    int top = -1;
    while(n){
        stack[++top] = n;
        n >>= 1;
    }
    uint64_t r = 1;
    while(top >= 0){
        n = stack[top--];
        r *= r;
        if(n & 1){
            r *= x;
        }
    }
    return r;
}
```

The above version still needs addtional space to track n. When $n=50$, the bit representation at each round is as the following:

$$
n_0=000000(0)\stackrel{n*2+1}\longrightarrow\
n_1=000001(1)\stackrel{n*2+1}\longrightarrow\
$$

$$
n_0=000000(0)\stackrel{n*2+1}\longrightarrow\ 
n_1=000001(1)\stackrel{n*2+1}\longrightarrow\ 
n_2=000011(3)\stackrel{n*2}\longrightarrow\ 
n_3=000110(6)
$$

$$
\stackrel{n*2}\longrightarrow \
n_4=001100(12)\stackrel{n*2+1}\longrightarrow 
n_5=011001(25)\stackrel{n*2}\longrightarrow 
n_6=110010(50)
$$

Checking the value from the highest bit to the lowest bit precisely indicates whether $n_{i+1}$ is odd or even. The \_\_builtin_clz() is utilized to count the number of the leading zero. "!!" ensures that the result of the expression within the brackets is either 0 or 1.
```c
uint64_t pow5_1(unsigned int x, unsigned int n)
{
    int hi = 31 - __builtin_clz(n);
    
    uint64_t r = 1;
    for(int i = hi; i >= 0; --i){
        r *= r;
        r = r * x * !!(n & (UINT64_C(1) << i)) + r * !!!(n & (UINT64_C(1) << i));
    }
    return r;
}
```
