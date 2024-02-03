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

    srand(TIME_SEED);

    input[0].x = 2;
    input[0].n = 30;

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

/**
 * @brief Evaluates the performance of the pow function.
 *
 * This function measures the performance of the pow function by running it on a
 * set of test cases. The performance metrics, including clock time, precise
 * time, and wall time, are recorded for each test case and printed at the end.
 *
 * @param test The test object containing the test cases and the pow function to
 * be tested.
 */
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

void validate(Test *testObj[], size_t size, size_t num_cases)
{
    for (size_t i = 0; i < num_cases; ++i) {
        uint64_t expected = testObj[0]->testcase[i].result.value;
        for (size_t j = 1; j < size; ++j) {
            assert(testObj[j]->testcase[i].result.value == expected);
        }
    }
}

void printTime(Time *time)
{
    printf("Clock Time: %lf s\n", time->clock_time);
    printf("Precise Time: %lf ms\n", time->precise_time);
    printf("Wall Time: %lf ms\n", time->wall_time);
}
int main(void)
{
    size_t num_cases = 200;
    powFunc powArray[POW_FUNC_SIZE] = {pow1, pow2,   pow2_1, pow3,  pow3_1,
                                       pow4, pow4_1, pow5,   pow5_1};
    Input *input = initInput(num_cases);
    Test *testObj[POW_FUNC_SIZE];
    for (size_t i = 0; i < POW_FUNC_SIZE; ++i) {
        testObj[i] = initTest(num_cases, input, powArray[i]);
        evaluate(testObj[i]);
    }
    validate(testObj, POW_FUNC_SIZE, num_cases);

    return 0;
}