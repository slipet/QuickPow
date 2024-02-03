#include "test.h"

/**
 * @brief initializes an array of test cases.
 *
 * @param num_cases the number of test cases to initialize
 * @return allocated memory for the test cases
 */
Input *initInput(size_t num_cases)
{
    // assert that num_cases is greater than 0
    assert(num_cases > 0);
    Input *input = (Input *) malloc(sizeof(Input) * num_cases);

    srand(time(NULL));

    input[0].x = rand() % 10000000000;
    input[0].n = 0;

    for (size_t i = 1; i < num_cases; i++) {
        input[i].x = rand() % 10;
        input[i].n = rand() % 15;
    }

    return input;
}
/**
 * @brief initializes a test object with test cases and a function pointer to
 * the pow function to be tested.
 *
 * @param num_cases the number of test cases
 * @param input the test cases for the pow function
 * @param callPow the function pointer to the pow function to be tested
 * @return a pointer to the initialized test object
 */
Test *initTest(size_t num_cases, Input *input, powFunc callPow)
{
    Test *test = malloc(sizeof(Test));
    test->testcase = malloc(sizeof(TestCase) * num_cases);
    test->num_cases = num_cases;
    test->func = callPow;
    for (int i = 0; i < num_cases; i++) {
        test->testcase[i].input.x = input[i].x;
        test->testcase[i].input.n = input[i].n;
    }
    return test;
}



void evaluate(Test *test)
{
    PROBE_TIME(test->total_time, start);
    for (int i = 0; i < test->num_cases; i++) {
        PROBE_TIME(test->testcase[i].result.time, start);

        test->testcase[i].result.value =
            test->func(test->testcase[i].input.x, test->testcase[i].input.n);

        PROBE_TIME(test->testcase[i].result.time, end);
        MESURE_TIME(test->testcase[i].result.time);
        // printTime(&(test->testcase[i].result.time));
    }
    PROBE_TIME(test->total_time, end);
    MESURE_TIME(test->total_time);
    printf("======================================\n");
    printf("Total time: \n");
    printTime(&(test->total_time));
}

void printTime(Time *time)
{
    printf("Clock Time: %lf s\n", time->clock_time);
    printf("Precise Time: %lf ms\n", time->precise_time);
    printf("Wall Time: %lf ms\n", time->wall_time);
}
int main(void)
{
    size_t num_cases = 50;
    powFunc powArray[POW_FUNC_SIZE] = {pow1, pow2,   pow2_1, pow3,  pow3_1,
                                       pow4, pow4_1, pow5,   pow5_1};
    Input *input = initInput(num_cases);
    Test *testObj[POW_FUNC_SIZE];
    for (size_t i = 0; i < POW_FUNC_SIZE; ++i) {
        testObj[i] = initTest(num_cases, input, powArray[i]);
        evaluate(testObj[i]);
    }

    return 0;
}