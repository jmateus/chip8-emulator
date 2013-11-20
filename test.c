#include <stdio.h>

int main() {
	printf("char: %d\n", sizeof(char));
	printf("short: %d\n", sizeof(short));
	printf("int: %d\n", sizeof(int));
	printf("long: %d\n", sizeof(long));

	while(1) {
		char c = getch(stdin);

		printf("key: %d\n", (int) c);

	}
}