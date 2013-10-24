#include <stdio.h>

#include "keyboard.h"
#include "xterm_control.h"

#define CONTROL_C	3

void main(int argc, char *argv[]) {
  
  //if given different than: my_page_edit <filename>
  if(argc != 2){ 
    printf("USAGE: ./my_page_edit <file-name>\n");
    return;
  }

  FILE *fp;
  if(! (fp = fopen(argv[1], "r+")))
    fp = fopen(argv[1], "w+");

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
    else if (c == KEY_DOWN && row < 22)
      xt_par2(XT_SET_ROW_COL_POS,++row,col);
    else if (c == KEY_RIGHT && col < 80)
      xt_par2(XT_SET_ROW_COL_POS,row,++col);
    else if (c == KEY_LEFT && col > 1)
      xt_par2(XT_SET_ROW_COL_POS,row,--col);
    else if (c == KEY_ENTER) 
      xt_par2(XT_SET_ROW_COL_POS,++row,col=1);
    else if (c == KEY_DELETE){
      putchar(' ');
      xt_par2(XT_SET_ROW_COL_POS,row,col);
    }
    else if (c == KEY_BACKSPACE){
      //putchar(' ');
      if (col > 1){
	xt_par2(XT_SET_ROW_COL_POS,row,--col);
	putchar(' ');
	xt_par2(XT_SET_ROW_COL_POS,row,col);
      }
      else if(row > 1) {
	xt_par2(XT_SET_ROW_COL_POS,--row,col=80);
	putchar(' ');
	xt_par2(XT_SET_ROW_COL_POS,row,col);
      }
      else{
	row = col = 1;
	putchar(' ');
	xt_par2(XT_SET_ROW_COL_POS,row,col);//col = linelength[row]
      }
    }
    else if (c >= ' ' && c <= '~') {
      putchar(c);
      if (col < 80)
	++col;
      else
	xt_par2(XT_SET_ROW_COL_POS,++row,col=1);
    }
  }

  fclose(fp);
  getkey_terminate();
}

		
		
