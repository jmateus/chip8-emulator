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
#define MIN_UNIT_DEFAULT_STREAM stdout

// TODO: Use ANSI colors to print log messages
#define MIN_UNIT_COLOR_RED ""
#define MIN_UNIT_COLOR_GREEN ""
#define MIN_UNIT_COLOR_RESET ""

// Stream used to output test's feedback. Can be override by the user. Default value is MIN_UNIT_DEFAULT_STREAM.
FILE* outStream = NULL;

// Number of tests ran in a suite
static int testsRun;
// Number of successful tests ran in a suite
static int successfulTests;

// Total number of tests ran in a module
static int totalTests;
// Total number of successful tests rand in a module
static int totalSuccessfulTests;

// Print detailed logs
static int detailedLogs = 1;


// Initializes the values needed for a test module. A test module is composed of many test suites.
// Setups the output stream used (in the case it wasn't overriden by the user) and the counters of total tests
// and total sucessful tests.
void setupTests() {
	if(outStream == NULL) {
		outStream = MIN_UNIT_DEFAULT_STREAM;
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
		if(detailedLogs) { \
			fprintf(outStream, MIN_UNIT_COLOR_RED "[FAILED] " MIN_UNIT_COLOR_RESET "%s\n", message); \
		} \
		testsRun++; \
		totalTests++; \
	} \
	else { \
		if(detailedLogs) { \
			fprintf(outStream, MIN_UNIT_COLOR_GREEN "[SUCCESS] " MIN_UNIT_COLOR_RESET "%s\n", message); \
		} \
		testsRun++; \
		totalTests++; \
		successfulTests++; \
		totalSuccessfulTests++; \
	} \
} while(0);


// Macro used to initialize a test module.
// It generates a function called runTests that contains every test of the module.
#define BEGIN_TESTS(name) void runTests() { \
	setupTests(); \
	fprintf(outStream, "##############################\n");\
	fprintf(outStream, "[INFO] STARTING TESTS: " name "\n"); \
	fprintf(outStream, "##############################\n\n");\


// Macro used to declare a new test suite. Name is the name of the suite. Code is everything done in the suite.
#define BEGIN_SUITE(name, code) do { \
	if(detailedLogs) { \
		fprintf(outStream, "==============================\n");\
		fprintf(outStream, "[INFO] Starting %s\n", name);\
		fprintf(outStream, "==============================\n\n");\
	} \
	setupSuite(); \
	\
	code \
	\
	if(detailedLogs) { \
		fprintf(outStream, "\n%s[FINISHED]%s %d out of %d tests were successful\n\n", \
			successfulTests == testsRun ? MIN_UNIT_COLOR_GREEN : MIN_UNIT_COLOR_RED, \
			MIN_UNIT_COLOR_RESET, successfulTests, testsRun); \
	} \
} while(0);


// Helper macro to create a scope for one test.
#define TEST(message, code) do { \
	if(detailedLogs) { \
		fprintf(outStream, "\n[INFO] Start testing: %s\n", message); \
	} \
	code \
	if(detailedLogs) { \
		fprintf(outStream, "[INFO] End testing: %s\n\n", message); \
	} \
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
		if(argc > 1) { \
			if(argv[1][0] == '-' && argv[1][1] == 's') { \
				detailedLogs = 0; \
			} \
		} \
		runTests(); \
		\
		return 0; \
	}


// Helper macro to statically initialize an array, structure or union. 
// If it isn't used, the macro will interpret the elements as arguments, throwing an error during compilation.
// source: http://stackoverflow.com/questions/5503362/passing-array-literal-as-macro-argument
#define STATIC_INIT(...) __VA_ARGS__ 

#endif
