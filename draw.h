/*
  Copyright: IESB CG
  Author: Daniel da Silva Souza - 0912130071
  Date: 15/08/10 23:43
  cg_draw_line
*/
#ifndef DRAW_H
#define DRAW_H

//round() usado para arredondar retas com coeficiente angular e o círculo.
#include <math.h>
#include <unistd.h>
#include "term.h"
#include <stdio.h>
#include "card.h"

void draw_empty_card(int x, int y) {
  printf("%s%s", TC_BG_NRM, TC_WHT);
  for(int line = y; line <= y+4; line++) {
      term_move_xy(x,line);
      for(int j=x; j <= x+6; j++)
        printf("%s", HIDDEN_2);
    }
  printf("%s%s\n", TC_BG_NRM, TC_RST);
}

void draw_card_simples(int x, int y, t_card card) {
term_move_xy(x,y);

printf("%s%s%d%s", TC_BG_WHT, card.color, card.value, card.naipe);
//cor de fundo, cor do texto, valor da carta, náipe
}

void draw_card(int x, int y, t_card card) {
  term_move_xy(x,y);
  if(card.is_hidden) {
    static int i = 0;
    printf("%s%s", (i % 2 == 1) ? TC_BG_WHT : TC_BG_NRM, TC_BLU);
    for(int line = y; line <= y+4; line++) {
      term_move_xy(x,line);
      for(int j=x; j <= x+6; j++)
        printf("%s", (i % 2 == 1) ? HIDDEN : HIDDEN_3);
    }
    i++;
  } else {
    printf("%s%s", TC_BG_WHT, card.color);
    printf("%d      ", card.value);
    term_move_xy(x,y+1);
    printf("%s      ", card.naipe);
    term_move_xy(x,y+2);
    printf("   %s   ", card.naipe);
    term_move_xy(x,y+3);
    printf("      %s", card.naipe);
    term_move_xy(x,y+4);
    printf("      %d", card.value);
  }
  printf("%s%s\n", TC_BG_NRM, TC_RST);
}

void troca(int *a, int *b)
{
/* troca valores para percorrer o tamanho da taxa de variação entre o maior e o menor
para a função cg_drawline. Permitindo assim chamar a função com pontos distintos
P(x1,y1) e Q(x2,y2) sem distinção de ordem entre qual é o maior e o menor valor entre eles. */
  int aux = *a;
  *a = *b;
  *b = aux;
}

void cg_drawline(int x1, int y1, int x2, int y2)
{
/*
Equação geral da reta: ax + by + c = 0
para P(x1,y1) e Q(x2,y2):
coef a = y1-y2
coef b = x2-x1
coef c = x1y2-x2y1

se a = 0 -> eq da reta fica: by + c = 0 ou y = -c/b
e temos uma reta paralela ao eixo Ox

se b = 0 -> eq da reta fica: ax + c = 0 ou x = -c/a
e temos uma reta paralela ao eixo Oy

se b != 0 -> temos y = -[(a/b)*x] -c/b = mx + q
onde -a/b = (y2-y1)/(x2-x1) = m -> coeficiente angular
     -c/b = [(x2y1)-(x1y2)]/x2-x1 = q -> coeficiente linear     
         ::\> y = mx+q
*/
     float a = y1-y2,
           b = x2-x1,
           c = (x1*y2)-(x2*y1),
           x = 0,
           y = 0;
     
     //m = Coef Angular q = Coef Linear
     float m = -(a/b),
           q = -(c/b);
         
     if (b != 0)
     {
         //y = (m*x)+q;
         if (x1 > x2)troca(&x1,&x2);
         for(x=x1;x<=x2;x+=1)
         {
              y = round((m*x)+q);
              term_move_xy((int)x, (int)y);
              printf("@");
              fflush(0);
              usleep(10000);
         }
     }
     else
     {
         if (a == 0)
         {
             //y = -c/b ou by+c = 0;
             y = -c/b;
             if (x1 > x2)troca(&x1,&x2);
             for(x=x1;x<=x2;x+=1.0)
             {
                term_move_xy((int)x, (int)y);
                printf("@");
                fflush(0);
                usleep(10000);
             }
         }
         
         if (b == 0)
         {
             //x = -c/a ou ax + c = 0;
             x= -c/a;
             if (y1 > y2)troca(&y1,&y2);
             for(y=y1;y<=y2;y+=1.0)
             {
                term_move_xy((int)x, (int)y);
                printf("@");
                fflush(0);
                usleep(10000);
             }
         }
     }
}

void animate_card(int x1,int y1,int x2,int y2, t_card card) {
     float a = y1-y2,
           b = x2-x1,
           c = (x1*y2)-(x2*y1),
           x = 0,
           y = 0;
     
     //m = Coef Angular q = Coef Linear
     float m = -(a/b),
           q = -(c/b);
         
     if (b != 0)
     {
         //y = (m*x)+q;
         if (x1 > x2)troca(&x1,&x2);
         for(x=x1;x<=x2;x+=1)
         {
              y = round((m*x)+q);
              draw_card((int) x,(int) y, card);
              fflush(0);
              usleep(40000);
         }
     }
     else
     {
         if (a == 0)
         {
             //y = -c/b ou by+c = 0;
             y = -c/b;
             if (x1 > x2)troca(&x1,&x2);
             for(x=x1;x<=x2;x+=1.0)
             {
                draw_card((int) x,(int) y, card);
                fflush(0);
                usleep(10000);
             }
         }
         
         if (b == 0)
         {
             //x = -c/a ou ax + c = 0;
             x= -c/a;
             if (y1 > y2)troca(&y1,&y2);
             for(y=y1;y<=y2;y+=1.0)
             {
                draw_card((int) x,(int) y, card);
                fflush(0);
                usleep(10000);
             }
         }
     }
}

#endif