#include <stdio.h>
#include <string.h>

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
  
  int cpl[22];  // chars per line
  int i;
  for (i = 0; i < 22; i++)
    cpl[i] = 0;
  
  char *statement = "Active keys:  F2-exit  F3-save & exit   Arrow Keys  Backspace  Delete ";
  xt_par2(XT_SET_ROW_COL_POS,row=23,col=1);
  xt_par0(XT_CLEAR_SCREEN);
  printf("%s",statement);
  xt_par2(XT_SET_ROW_COL_POS,row=1,col=1);

  int data[22][81];

  FILE *fp;
  if (fp = fopen(argv[1], "r")){
    /*
    int c;
    while (fgetc(fp) != EOF){
      c = fgetc(fp);
      printf("%c", c);
      if (c == '\n')
	row ++;
      else(cpl[row-1]++);
      if (row > 22)
	break;
	}
    */
	
    int a,b,d;
    a = b = 0;
    char line[81];
    while(fgets(line, 80, fp)){
      printf("%s", line);
      // d = line;
      //   while(d){
      //	data[a][b++] = d;
      //	++d;
      //      }
      int i;
      if (strlen(line) == 1)
	cpl[a]++;
      for (i = 0; i < strlen(line); i++){
	data[a][b++] = line[i];
	cpl[a]++;
      }
      cpl[a]--;
      a++;
      if (a == 22)
	break;
    }
    /*
    while((d = fgetc(fp)) != EOF){
      if(a = 22)
	break;
      data[a][b++] = d;
      if(d == '\n'){
	++a;
	b = 0;
      }
      if(b == 80){
	data[a++][b] = '\n';
	b = 0;
      }
    }
    
    a = b = 0;
    while(a < 22 || b < 80){
      if(b == 80 || data[a][b] == '\n'){
	//putchar('\n');
	xt_par2(XT_SET_ROW_COL_POS,row=(a+2),col=1);
	b = 0;
	++a;
      } else
	//putchar((char)(data[a][b++]));
	putchar(data[a][b++]);
    }
    */
  }

  else fp = fopen(argv[1], "w");

  xt_par2(XT_SET_ROW_COL_POS,row=1,col=1);

  while (1) {
    while ((c = getkey()) == KEY_NOTHING) ;
    if(c == KEY_F2) break;
    else if(c == KEY_F3){
      writefile(argv[1]);
      break;
    }

    else if (c == KEY_UP && row > 1){
      xt_par2(XT_SET_ROW_COL_POS,--row,col);
      if (cpl[row-1] < col){
	col = cpl[row-1] + 1;
	xt_par2(XT_SET_ROW_COL_POS, row, col);
      }
    }
    

    else if (c == KEY_DOWN && row < 22){
      if (cpl[row] != 0){
	xt_par2(XT_SET_ROW_COL_POS,++row,col);
	if (cpl[row-1] < col){
	  col = cpl[row-1] + 1;
	  xt_par2(XT_SET_ROW_COL_POS, row, col);
	}
      }
    }

    else if (c == KEY_RIGHT && col < 80){
      if (cpl[row-1] >= col)
	xt_par2(XT_SET_ROW_COL_POS,row,++col);
      else if (row < 22 && cpl[row] > 0){
	col = 1;
	xt_par2(XT_SET_ROW_COL_POS, ++row, col);
      }
    }
      
    else if (c == KEY_LEFT){
      if (col > 1)
	xt_par2(XT_SET_ROW_COL_POS,row,--col);
      else if(row > 1){
	col = cpl[row-2] + 1;
	xt_par2(XT_SET_ROW_COL_POS, --row, col);
      }
    }
 

    else if (c == KEY_ENTER){
      putchar('\n');
      if (cpl[row-1] == 0)
	  cpl[row-1]++;
      if(row < 22)
	xt_par2(XT_SET_ROW_COL_POS,++row,col=1);
    }
    else if (c == KEY_DELETE){
      putchar(' ');
      xt_par2(XT_SET_ROW_COL_POS,row,col);
      if(cpl[row-1] == (col-1))
	cpl[row-1]--;
    }
    else if (c == KEY_BACKSPACE){

      if (cpl[row-1] == (col-1))
      cpl[row-1]--;

      //cursor is not on first column
      if (col > 1){
	//if (cpl[row-1] == (col-1))
	//    cpl[row-1]--;
	xt_par2(XT_SET_ROW_COL_POS,row,--col);
	putchar(' ');
	xt_par2(XT_SET_ROW_COL_POS,row,col);
      }
      //cursor is not on first row and is on first column
      else if(row > 1) {
	//cpl[row-1] = 0;
	xt_par2(XT_SET_ROW_COL_POS,--row,col=(cpl[row-2]+1));
	putchar(' ');
	xt_par2(XT_SET_ROW_COL_POS,row,col);
      }
      //cursor is on first row and on first column
      else{
	row = col = 1;
	//cpl[row-1] = 0;
	putchar(' ');
	xt_par2(XT_SET_ROW_COL_POS,row,col);//col = linelength[row]
      }
    }
    else if (c >= ' ' && c <= '~') {
      putchar(c);
      if (col < 80){
	++col;
	cpl[row-1]++;
      }
      else if(row < 22){
	cpl[row-1] = 80;
	xt_par2(XT_SET_ROW_COL_POS,++row,col=1);
      }
      else
	xt_par2(XT_SET_ROW_COL_POS,row,col);
    }
  }
  //  xt_par0(XT_CLEAR_SCREEN);

  fclose(fp);
  getkey_terminate();
}

void writefile(char* filename){
  FILE *fp = fopen(filename, "w");
  int row, col;
  row = col = 1;
  xt_par2(XT_SET_ROW_COL_POS, row, col); 
  //mechanism to read everything from the editor into the file...
}
  
