#include <stdio.h>
#include "keyboard.h"

void main() {
	char buf[20];
	int nbuf = 0, c, i;
	int inrep[10];
	
	printf("Usage: press any set of keys and then Enter, and it will be sent to the display...\n");
	while (1) {
		if ((c = getkey()) != KEY_NOTHING) {
			if (c == KEY_ENTER) {
				for (i = 0; i < nbuf; ++i) printf("%c",buf[i]);
				if (buf[0] == 27 && buf[1] == '[' && buf[2] == '6' && buf[3] == 'n') {
					for (nbuf = 0; nbuf < 20; ++nbuf) {
						if ((c = getkey()) == KEY_NOTHING) break;
						buf[nbuf] = c;
					}
					printf("received: %d chars\n", nbuf);
					if (nbuf > 0) for (i = 1; i < nbuf; ++i) printf("%c",buf[i]);
				}
				nbuf = 0;
			}
			else if (c == 3) break;
			else if (nbuf < 20)
				buf[nbuf++] = c;
		}
	}
}
