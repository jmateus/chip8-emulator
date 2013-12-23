/* MinUnit: http://www.jera.com/techinfo/jtns/jtn002.html */

#ifndef MIN_UNIT_H
#define MIN_UNIT_H

#include <stdio.h>

/*#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; \
							if (message) return message; } while (0)*/

extern int testsRun;
extern int successfulTests;
extern FILE* outStream;


#define mu_assert(message, test) do { \
	if(!(test)) {
		fprintf(outStream, "TEST FAILED: ")
	}
	else {
		
	}
}


#endif