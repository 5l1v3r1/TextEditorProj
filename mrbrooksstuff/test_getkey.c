#include <stdio.h>

void main() {
	int code;
	printf("OK. gimme keys:\n");
	while (1) {
		if ((code = getkey()) != -2) {
			if (code >= ' ' && code <= '~')
				printf("%c\n", code);
			else
				printf("%d\n", code);
		}
	}
}
