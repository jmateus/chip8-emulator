/* Based on MinUnit: http://www.jera.com/techinfo/jtns/jtn002.html */

/** Original MinUnit code: 
* 	#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
*	#define mu_run_test(test) do { char *message = test(); tests_run++; \
*							if (message) return message; } while (0)
**/

#ifndef MIN_UNIT_H
#define MIN_UNIT_H

#include <stdio.h>

#define DEFAULT_STREAM stdout

FILE* outStream = NULL;

static int testsRun;
static int successfulTests;


void setupTests() {
	if(outStream == NULL) {
		outStream = DEFAULT_STREAM;
	}
}


void setupSuite() {
	testsRun = 0;
	successfulTests = 0;
}


#define mu_assert(message, test) do { \
	if(!(test)) { \
		fprintf(outStream, "[FAILED] %s\n", message); \
		testsRun++; \
	} \
	else { \
		fprintf(outStream, "[SUCCESS] %s\n", message); \
		testsRun++; \
		successfulTests++; \
	} \
} while(0);



#define BEGIN_TESTS void runTests() { \
	setupTests(); \
	fprintf(outStream, "##############################\n");\
	fprintf(outStream, "[INFO] STARTING TESTS\n"); \
	fprintf(outStream, "##############################\n\n");\


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


#define TEST(message, code) do { \
	fprintf(outStream, "\n[INFO] Start testing: %s\n", message); \
	code \
	fprintf(outStream, "[INFO] End testing: %s\n\n", message); \
} while(0);


#define END_TESTS }

#define END_TESTS_AUTO_RUN } \
int main(int argc, char **argv) { \
	runTests(); \
	\
	return 0; \
}

#endif
