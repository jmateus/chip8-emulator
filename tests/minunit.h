/* Based on MinUnit: http://www.jera.com/techinfo/jtns/jtn002.html */

/** Original MinUnit code: 
* 	#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
*	#define mu_run_test(test) do { char *message = test(); tests_run++; \
*							if (message) return message; } while (0)
**/

#ifndef MIN_UNIT_H
#define MIN_UNIT_H

#include <stdio.h>

// Default stream used to output test's messages
#define DEFAULT_STREAM stdout

// Stream used to output test's feedback. Can be override by the user. Default value is DEFAULT_STREAM.
FILE* outStream = NULL;

// Number of tests ran in a suite
static int testsRun;
// Number of successful tests ran in a suite
static int successfulTests;

// Total number of tests ran in a module
static int totalTests;
// Total number of successful tests rand in a module
static int totalSuccessfulTests;


// Initializes the values needed for a test module. A test module is composed of many test suites.
// Setups the output stream used (in the case it wasn't overriden by the user) and the counters of total tests
// and total sucessful tests.
void setupTests() {
	if(outStream == NULL) {
		outStream = DEFAULT_STREAM;
	}

	totalTests = 0;
	totalSuccessfulTests = 0;
}


// Initializes the values needed for a test suite: number of tests ran and number of sucessful tests
void setupSuite() {
	testsRun = 0;
	successfulTests = 0;
}


// Checks if the test is true.
#define mu_assert(message, test) do { \
	if(!(test)) { \
		fprintf(outStream, "[FAILED] %s\n", message); \
		testsRun++; \
		totalTests++; \
	} \
	else { \
		fprintf(outStream, "[SUCCESS] %s\n", message); \
		testsRun++; \
		totalTests++; \
		successfulTests++; \
		totalSuccessfulTests++; \
	} \
} while(0);


// Macro used to initialize a test module.
// It generates a function called runTests that contains every test of the module.
#define BEGIN_TESTS void runTests() { \
	setupTests(); \
	fprintf(outStream, "##############################\n");\
	fprintf(outStream, "[INFO] STARTING TESTS\n"); \
	fprintf(outStream, "##############################\n\n");\


// Macro used to declare a new test suite. Name is the name of the suite. Code is everything done in the suite.
#define BEGIN_SUITE(name, code) do { \
	fprintf(outStream, "==============================\n");\
	fprintf(outStream, "[INFO] Starting %s\n", name);\
	fprintf(outStream, "==============================\n\n");\
	setupSuite(); \
	\
	code \
	\
	fprintf(outStream, "\n[FINISHED] %d out of %d tests were successful\n\n", successfulTests, testsRun); \
} while(0);


// Helper macro to create a scope for one test.
#define TEST(message, code) do { \
	fprintf(outStream, "\n[INFO] Start testing: %s\n", message); \
	code \
	fprintf(outStream, "[INFO] End testing: %s\n\n", message); \
} while(0);


// Macro used to signalize the end of a test module.
#define END_TESTS fprintf(outStream, "##############################\n");\
	fprintf(outStream, "[INFO] ENDED TESTS\n"); \
	fprintf(outStream, "\n[FINISHED] %d out of %d tests were successful\n", totalSuccessfulTests, totalTests); \
	fprintf(outStream, "##############################\n\n"); }


// The same as the END_TESTS macro, but it algo generates a main function. 
// The main function simply calls runTests and returns zero.
#define END_TESTS_AUTO_RUN END_TESTS \
	int main(int argc, char **argv) { \
		runTests(); \
		\
		return 0; \
	}


// Helper macro to statically initialize an array, structure or union. 
// If it isn't used, the macro will interpret the elements as arguments, throwing an error during compilation.
// source: http://stackoverflow.com/questions/5503362/passing-array-literal-as-macro-argument
#define STATIC_INIT(...) __VA_ARGS__ 

#endif
