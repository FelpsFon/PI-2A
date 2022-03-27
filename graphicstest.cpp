#include <graphics.h>

int main(){
  int gd=DETECT, gm;

  initgraph(&gd, &gm, (char*)"");
  circle(320, 240, 200);
  rectangle(100, 100, 400, 300);
  
  getch();
  closegraph();

  return 0;
}
