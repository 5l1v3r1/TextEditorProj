#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "keyboard.h"
#include "xterm_control.h"

#define CONTROL_C        3

void printDataMatrix(char data[22][82]){
  int i = 0;
  for (i; i < 22; i++){
    printf("%s", data[i]);
  }
}

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

  char data[22][82];

  FILE *fp;
  if (fp = fopen(argv[1], "r")){
        
    int a = 0;
    char line[82];
    //    line = (char *)malloc(82);
    while(fgets(line, 82, fp)){
      printf("%s", line);
      int i;
      //      if (strlen(line) == 1)
	//        cpl[a]++;
      //for (i = 0; i < strlen(line); i++){
      //data[a][b++] = line[i];
      //line = (char *)malloc(82)
      strcpy(data[a], line);
      cpl[a] = strlen(line);
      if (data[a][strlen(line)-1] == '\n'){
	data[a][strlen(line)-1] = '\0';
	cpl[a]--; //dont think this should be here??
      }
      a++;
      if (a >= 22)
        break;
    }
  }
  
  //  else{
  //    fp = fopen(argv[1], "w");
  //  }
  
  xt_par2(XT_SET_ROW_COL_POS,row=1,col=1);

  while (1) {
    while ((c = getkey()) == KEY_NOTHING) ;
    if(c == KEY_F2)      break;
    else if(c == KEY_F3){
      //      fclose(fp);

      //saving file
      fp = fopen(argv[1], "w");
      //fp = fopen("", "w");
      int i;
      //      a = 0;
      //      printf("HERE\n");
      //      printDataMatrix(data);
      for (i = 0; i < 22; i++){
	if (cpl[i] > 0){
	  data[i][(cpl[i])] = '\n';
	  data[i][(cpl[i])+1] = '\0';
	  fprintf(fp, "%s", data[i]);
	}
      }
	  //      while(fputs(data[a], fp)){
      //      for (a = 0; a < 22; a++){
      //	if (cpl[i]
      // ++a;
      //      }
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

      if(row < 22){
	//putchar('\n');
	//	if(col-1 == cpl[row-1])
	//	  data[row-1][col-1] = '\n';
        xt_par2(XT_SET_ROW_COL_POS,++row,col=1);
	if (cpl[row-2] == 0)
	  cpl[row-2]++;
      }
    }

    else if (c == KEY_DELETE){
      putchar(' ');
      data[row-1][col-1] = ' ';
      xt_par2(XT_SET_ROW_COL_POS,row,col);
      if(cpl[row-1] == (col-1))
        cpl[row-1]--;
    }
    else if (c == KEY_BACKSPACE){

      if (cpl[row-1] == (col-1))
	cpl[row-1]--;

      //cursor is not on first column
      if (col > 1){
        xt_par2(XT_SET_ROW_COL_POS,row,--col);
        putchar(' ');
        xt_par2(XT_SET_ROW_COL_POS,row,col);
      }
      //cursor is not on first row and is on first column
      else if(row > 1) {
        xt_par2(XT_SET_ROW_COL_POS,--row,col=(cpl[row-2]+1));
        putchar(' ');
        xt_par2(XT_SET_ROW_COL_POS,row,col);
      }
      //cursor is on first row and on first column
      else{
        row = col = 1;
        putchar(' ');
        xt_par2(XT_SET_ROW_COL_POS,row,col);//col = linelength[row]
      }
    }
    else if (c >= ' ' && c <= '~') {
      putchar(c);
      data[row-1][col-1] = c;
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

  fclose(fp);
  getkey_terminate();


  xt_par0(XT_CLEAR_SCREEN); 
  xt_par0(XT_CLEAR_SCREEN);
}
