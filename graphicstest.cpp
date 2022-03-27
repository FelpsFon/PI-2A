#include <graphics.h>

/*
  rectangle(X-coordinate of top-left corner, Y-coordinate of top-left corner, X-coordinate of bottom-right corner, Y-coordinate of bottom-right corner)

  circle(X-coordinate of center, Y-coordinate of center, radius of the circle)

  arc(X-coordinate of center, Y-coordinate of center, starting angle, end angle, radius of the arc)

  line(X-coordinate of point 1, Y-coordinate of point 1, X-coordinate of point 2, Y-coordinate of point 2)

  outtextxy(X-coordinate of point, Y-coordinate of point, string)

  setcolor(color)

  setfillsyle(style, color)

  enum colors { BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, LIGHTGRAY, DARKGRAY,
              LIGHTBLUE, LIGHTGREEN, LIGHTCYAN, LIGHTRED, LIGHTMAGENTA, YELLOW, WHITE };

  enum fill_styles { EMPTY_FILL, SOLID_FILL, LINE_FILL, LTSLASH_FILL, SLASH_FILL,
                    BKSLASH_FILL, LTBKSLASH_FILL, HATCH_FILL, XHATCH_FILL, INTERLEAVE_FILL,
                    WIDE_DOT_FILL, CLOSE_DOT_FILL, USER_FILL };
*/

int main(){
  int gd=DETECT, gm;

  initgraph(&gd, &gm, (char*)"");

  setfillstyle(SOLID_FILL, BLACK);
  rectangle(90, 10, 550, 470); //borda
  floodfill(91, 11, WHITE);

  setfillstyle(SOLID_FILL, RED);
  rectangle(100, 20, 540, 460); //capa
  floodfill(101, 21, WHITE);

  setcolor(BLACK);
  circle(320, 240, 200); //rosto
  circle(320, 240, 201);
  circle(320, 240, 202);
  circle(320, 240, 203);

  setfillstyle(SOLID_FILL, BLACK);
  circle(225, 175, 20); //olho esquerdo
  floodfill(226, 176, BLACK);

  setfillstyle(SOLID_FILL, BLACK);
  circle(425, 175, 20); //olho direito
  floodfill(426, 176, BLACK);

  arc(477, 266, 190, 330, 15); //bochecha

  arc(320, 240, 210, 345, 160); //sorriso
  arc(321, 241, 210, 345, 160);

  line(200, 125, 250, 150); //sombrancelha esquerda
  line(202, 127, 252, 152);

  line(450, 125, 400, 150); //sombrancelha direita
  line(451, 126, 401, 151);
  
  getch();
  closegraph();

  return 0;
}