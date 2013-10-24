#include <stdio.h>

#include "keyboard.h"
#include "xterm_control.h"

#define CONTROL_C	3

void main() {
	int c, row, col;
	char *greeting = "Arrow keys work, and F10 terminates...";
	xt_par2(XT_SET_ROW_COL_POS,row=1,col=1);
	row = col = 1;
	xt_par0(XT_CLEAR_SCREEN);
	printf("%s",greeting);
	xt_par2(XT_SET_ROW_COL_POS,row=2,col=1);
	
	while (1) {
		while ((c = getkey()) == KEY_NOTHING) ;
		if(c == KEY_F10) break;
		else if (c == KEY_UP && row > 1) 
			xt_par2(XT_SET_ROW_COL_POS,--row,col);
		else if (c == KEY_DOWN && row < 24)
			xt_par2(XT_SET_ROW_COL_POS,++row,col);
		else if (c == KEY_RIGHT && col < 80)
			xt_par2(XT_SET_ROW_COL_POS,row,++col);
		else if (c == KEY_LEFT && col > 1)
			xt_par2(XT_SET_ROW_COL_POS,row,--col);
		else if (c == KEY_ENTER) 
			xt_par2(XT_SET_ROW_COL_POS,++row,col=1);
		else if (c >= ' ' && c <= '~') {
			putchar(c);
			if (col < 80)
				++col;
			else
				xt_par2(XT_SET_ROW_COL_POS,++row,col=1);
		}
	}
	getkey_terminate();
}

		
		