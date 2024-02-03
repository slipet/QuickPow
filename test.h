#ifndef TEST
#define TEST
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pow.h"

#define PROBE_TIME(TIME_OBJECT, TIME_POINT)                               \
    ({                                                                    \
        TIME_OBJECT.clock_##TIME_POINT = clock();                         \
        clock_gettime(CLOCK_REALTIME, &TIME_OBJECT.precise_##TIME_POINT); \
        clock_gettime(CLOCK_MONOTONIC, &TIME_OBJECT.wall_##TIME_POINT);   \
    })

#define MESURE_TIME(TIME_OBJECT)                                          \
    ({                                                                    \
        TIME_OBJECT.clock_time =                                          \
            (double) (TIME_OBJECT.clock_end - TIME_OBJECT.clock_start) /  \
            CLOCKS_PER_SEC;                                               \
        TIME_OBJECT.precise_time =                                        \
            1000.0 * TIME_OBJECT.precise_end.tv_sec +                     \
            1e-6 * TIME_OBJECT.precise_end.tv_nsec -                      \
            (1000.0 * TIME_OBJECT.precise_start.tv_sec +                  \
             1e-6 * TIME_OBJECT.precise_start.tv_nsec);                   \
        TIME_OBJECT.wall_time = 1000.0 * TIME_OBJECT.wall_end.tv_sec +    \
                                1e-6 * TIME_OBJECT.wall_end.tv_nsec -     \
                                (1000.0 * TIME_OBJECT.wall_start.tv_sec + \
                                 1e-6 * TIME_OBJECT.wall_start.tv_nsec);  \
    })


typedef uint64_t (*powFunc)(unsigned int, unsigned int);
typedef struct timespec timespec;
typedef struct Time {
    clock_t clock_start, clock_end;
    timespec wall_start, wall_end;
    timespec precise_start, precise_end;
    double clock_time;
    double precise_time;
    double wall_time;
} Time;

typedef struct Result {
    uint64_t value;
    Time time;
} Result;

typedef struct Input {
    unsigned int x;
    unsigned int n;
} Input;

typedef struct TestCase {
    Input input;
    Result result;
} TestCase;

typedef struct Test {
    powFunc func;
    size_t num_cases;
    Time total_time;
    TestCase *testcase;
} Test;


void evaluate(Test *);
Input *initInput(size_t);
Test *initTest(size_t, Input *, powFunc);
void printTime(Time *);

#endif
