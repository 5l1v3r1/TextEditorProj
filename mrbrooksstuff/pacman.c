#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "keyboard.h"
#include "xterm_control.h"

#define CONTROL_C       3

int score = 0;
int highscore = 0;
char data[50][23];
int row, col,c;

void printscores(){
  if (score > highscore)
    highscore = score;
  xt_par2(XT_SET_ROW_COL_POS, 1, 1);
  printf("WELCOME TO PACMAN! Score: %d    HighScore: %d", score, highscore);
  xt_par2(XT_SET_ROW_COL_POS, row, col);
}

void computeScore(){
  int totalStars = 200;
  int numStarsLeft;
  int a, b;
  for (a = 0; a < 50; a++){
    for (b = 0; b < 23; b++){
      if (data[a][b] == '*')
	numStarsLeft++;
    }
  }
  score = totalStars - numStarsLeft;
}
      
  
void setUpBoard(){
  xt_par0(XT_CLEAR_SCREEN);
  xt_par2(XT_SET_ROW_COL_POS,1,1);
  FILE *fp;
  fp = fopen("pacman.txt", "r");
  char line[23];
  int a = 0;
  while (fgets(line,23,fp)){
    printf("%s",line);
    strcpy(data[a],line);
    a++;
    if (a >= 50)
      break;
  }
  fclose(fp);
  xt_par2(XT_SET_ROW_COL_POS, row=36, col=10);
  putchar('<');
}

void play(){
  setUpBoard();

  
  while (1){
    while ((c = getkey()) == KEY_NOTHING);
    if (c == KEY_RIGHT){
      if (data[row][col + 1] == '#')
	;
      else{
	while (data[row][col] != '#'){
	  putchar(' ');
	  xt_par2(XT_SET_ROW_COL_POS, row, ++col);
	  putchar('<');
	  computeScore();
	  printscores();
	  usleep(100000);
	}
      }
    }
    if (c == KEY_LEFT){
      if (data[row][col - 1] == '#')
	;
      else{
	while (data[row][col] != '#'){
	  putchar(' ');
	  xt_par2(XT_SET_ROW_COL_POS, row, --col);
	  putchar('>');
	  computeScore();
	  printscores();
	  usleep(100000);
	}
      }
    }
    if (c == KEY_UP){
      if (data[row-1][col] == '#')
	;
      else{
	while (data[row][col] != '#'){
	  putchar(' ');
	  xt_par2(XT_SET_ROW_COL_POS, --row, col);
	  putchar('V');
	  computeScore();
	  printscores();
	  usleep(100000);
	}
      }
    }
    if (c == KEY_DOWN){
      if (data[row+1][col] == '#')
	;
      else{
	while (data[row][col] != '#'){
	  putchar(' ');
	  xt_par2(XT_SET_ROW_COL_POS, ++row, col);
	  putchar('A');
	  computeScore();
	  printscores();
	  usleep(100000);
	}
      }
    }
  }	
    getkey_terminate();
    xt_par0(XT_CLEAR_SCREEN);
}

void main(){
  xt_par0(XT_CLEAR_SCREEN);
  int score = 0;
  int highscore = 0;
  xt_par2(XT_SET_ROW_COL_POS, 1,1);
  printf("Welcome to Pacman! Press any arrow key to start a new game.");
  
  while (1){
    while ((c = getkey()) == KEY_NOTHING);
    if (c == KEY_LEFT || c == KEY_RIGHT || c == KEY_UP || c == KEY_DOWN)
      play();
    //setUpBoard();
    usleep(5000000);
    break;
  }
  getkey_terminate();
  xt_par0(XT_CLEAR_SCREEN);
}
