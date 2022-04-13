#include <stdio.h>
#include "term.h"
#include "card.h"
#include "draw.h"

int console_cols, console_rows;

void on_console_window_change() {
  tc_get_cols_rows(&console_cols, &console_rows);
}

void print_main_menu() {
  term_move_xy(0,0);
  printf("1 - Novo jogo\n");
  printf("2 - Pontuação\n");
  printf("3 - Creditos\n");
  printf("0 - Sair\n");
  printf("Opção: ");
}

int main(){
  tc_get_cols_rows(&console_cols, &console_rows);
  
	tc_echo_off();
  tc_canon_off();

  t_card card = {1, COPAS, TC_RED, 1};
  
  term_cls();
  draw_card(1,1, card);

  draw_empty_card(40,1);
  draw_empty_card(53,1);
  draw_empty_card(66,1);
  draw_empty_card(79,1);

  card.is_hidden = 0;
  draw_card(1,10, card);
  
  card.value++; card.naipe = PAUS; card.color = TC_BLK;
  card.is_hidden = 1;
  draw_card(14,10, card);
  card.is_hidden = 0;
  draw_card(15,11, card);

  
  card.value++; card.naipe = OUROS; card.color = TC_RED;
  card.is_hidden = 1;
  draw_card(27,10, card);
  draw_card(28,11, card);
  card.is_hidden = 0;
  draw_card(29,12, card);

  card.value++; card.naipe = ESPADAS; card.color = TC_BLK;
  card.is_hidden = 1;
  draw_card(40,10, card);
  draw_card(41,11, card);
  draw_card(42,12, card);
  card.is_hidden = 0;
  draw_card(43,13, card);

  card.value++; card.naipe = COPAS; card.color = TC_RED;
  card.is_hidden = 1;
  draw_card(53,10, card);
  draw_card(54,11, card);
  draw_card(55,12, card);
  draw_card(56,13, card);
  card.is_hidden = 0;
  draw_card(57,14, card);

  card.value++; card.naipe = PAUS; card.color = TC_BLK;
  card.is_hidden = 1;
  draw_card(66,10, card);
  draw_card(67,11, card);
  draw_card(68,12, card);
  draw_card(69,13, card);
  draw_card(70,14, card);
  card.is_hidden = 0;
  draw_card(71,15, card);

  card.value++; card.naipe = OUROS; card.color = TC_RED;
  card.is_hidden = 1;
  draw_card(79,10, card);
  draw_card(80,11, card);
  draw_card(81,12, card);
  draw_card(82,13, card);
  draw_card(83,14, card);
  draw_card(84,15, card);
  card.is_hidden = 0;
  draw_card(85,16, card);
    //cg_drawline(1,1,console_cols/2, console_rows/2);
    //animate_card(console_cols-7, 1, console_cols/2, console_rows/2, card);
    term_move_xy(console_cols, console_rows);
    puts("#");
  getchar();
	return 0;

}//main*/