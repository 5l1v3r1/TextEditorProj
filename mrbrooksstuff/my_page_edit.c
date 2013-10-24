#include <stdio.h>

#include "keyboard.h"
#include "xterm_control.h"

#define CONTROL_C	3

void writefile();

void main(int argc, char *argv[]) {
  
  //if given different than: my_page_edit <filename>
  if(argc != 2){ 
    printf("USAGE: ./my_page_edit <file-name>\n");
    return;
  }

  int c, row, col;
  
  int charsperline[22];
  int i;
  for (i = 0; i < 22; i++)
    charsperline[i] = 0;
  
  char *statement = "Active keys:  F2-exit  F3-save & exit   Arrow Keys  Backspace  Delete ";
  xt_par2(XT_SET_ROW_COL_POS,row=23,col=1);
  xt_par0(XT_CLEAR_SCREEN);
  printf("%s",statement);
  xt_par2(XT_SET_ROW_COL_POS,row=1,col=1);

  FILE *fp;
  if (fp = fopen(argv[1], "r")){
    int c;
    while (c = fgetc(fp)){
      printf("%c", c);
      if (c == '\n')
	row ++;
      else(charsperline[row-1]++);
      if (row > 22)
	break;
  }

  else fp = fopen(argv[1], "w");

  while (1) {
    while ((c = getkey()) == KEY_NOTHING) ;
    if(c == KEY_F2) break;
    else if(c == KEY_F3){
      writefile();
      break;
    }
    //mod for line suppression
    else if (c == KEY_UP && row > 1) 
      xt_par2(XT_SET_ROW_COL_POS,--row,col);

    //mod for line suppression
    else if (c == KEY_DOWN && row < 22)
      xt_par2(XT_SET_ROW_COL_POS,++row,col);

    //mod for line suppression
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
	if (charsperline[row] == (col-1))
	    charsperline[row]--;
	xt_par2(XT_SET_ROW_COL_POS,row,--col);
	putchar(' ');
	xt_par2(XT_SET_ROW_COL_POS,row,col);
      }
      else if(row > 1) {
	charsperline[row] = 0;
	xt_par2(XT_SET_ROW_COL_POS,--row,col=80);
	putchar(' ');
	xt_par2(XT_SET_ROW_COL_POS,row,col);
      }
      else{
	row = col = 1;
	charsperline[row] = 0;
	putchar(' ');
	xt_par2(XT_SET_ROW_COL_POS,row,col);//col = linelength[row]
      }
    }
    else if (c >= ' ' && c <= '~') {
      putchar(c);
      if (col < 80){
	++col;
	charsperline[row]++;
      }
      else{
	charsperline[row] = 80;
	xt_par2(XT_SET_ROW_COL_POS,++row,col=1);
      }
    }

  fclose(fp);
  getkey_terminate();
}

void writefile(){
  FILE *fp = fopen(argv[1], "w");
  int row, col;
  row = col = 1;
  xt_par2(XT_SET_ROW_COL_POS, row, col); 
  //mechanism to read everything from the editor into the file...
}
  
