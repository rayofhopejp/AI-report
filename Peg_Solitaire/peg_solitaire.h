#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <vector>
using namespace std;

#ifdef PROBLEM_A
  #define MAX_WIDTH 5
  #define MAX_HEIGHT 6

  typedef struct BOARD {
    char cell[MAX_WIDTH*MAX_HEIGHT];
    struct BOARD *parent;
  }Board;

  Board B0={/*problem1*/
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
    0, 0, 0, 0, 0,
    NULL
  };
  Board BG={
    0, 0, 0, 0, 0,
    0, 0, 0, 1, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    NULL
  };
#endif
#ifdef PROBLEM_B
  #define MAX_WIDTH 7
  #define MAX_HEIGHT 5

  typedef struct BOARD {
    char cell[MAX_WIDTH*MAX_HEIGHT];
    struct BOARD *parent;
  }Board;

  Board B0={
    0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0,
    1, 1, 0, 0, 0, 1, 1,
    0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0,
    NULL
  };
  Board BG={
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    NULL
  };
#endif
#ifdef PROBLEM_C
  #define MAX_WIDTH 5
  #define MAX_HEIGHT 7

  typedef struct BOARD {
    char cell[MAX_WIDTH*MAX_HEIGHT];
    struct BOARD *parent;
  }Board;

  Board B0={
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    1, 1, 1, 1, 1,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    NULL
  };
  Board BG={
    0, 0, 1, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    NULL
  };
#endif
#ifdef PROBLEM_D
  #define MAX_WIDTH 10
  #define MAX_HEIGHT 10

  typedef struct BOARD {
    char cell[MAX_WIDTH*MAX_HEIGHT];
    struct BOARD *parent;
  }Board;

  Board B0={
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,1,1,1,1,1,1,1,0,
    0,0,1,1,1,1,1,1,0,0,
    0,0,1,1,1,1,1,0,0,0,
    0,0,0,0,1,0,1,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    NULL
  };
  Board BG={
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,1,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    NULL
  };
#endif

void print_board(Board *b){
  for(int y=0;y<MAX_HEIGHT;y++){
    //枠線
    printf("+");
    for(int x=0;x<MAX_WIDTH;x++){
      printf("-+");
    }
    printf("\n");
    //本体
    for(int x=0;x<MAX_WIDTH;x++){
      printf("|");
      if(b->cell[y*MAX_WIDTH+x]==1){
        printf("*");
      }else if(b->cell[y*MAX_WIDTH+x]==0){
        printf(" ");
      }else if(b->cell[y*MAX_WIDTH+x]==-1){
        printf("/");
      }
    }
    printf("|\n");
  }
  
  //枠線
  printf("+");
  for(int x=0;x<MAX_WIDTH;x++){
    printf("-+");
  }
  printf("\n");
}
bool is_board_equal(Board *a,Board *b){
  for(int y=0;y<MAX_HEIGHT;y++){
    for(int x=0;x<MAX_WIDTH;x++){
      if(a->cell[y*MAX_WIDTH+x]!=b->cell[y*MAX_WIDTH+x])return false;
    }
  }
  return true;
}
void print_answer(Board *fin){
  Board *now=fin;
  int cnt=0;
  while(now!=NULL){
    printf("phase %d\n",cnt++);
    print_board(now);
    now=now->parent;
  }
}
void board_copy(Board *to,Board *from){
  for(int y=0;y<MAX_HEIGHT;y++){
    for(int x=0;x<MAX_WIDTH;x++){
      to->cell[y*MAX_WIDTH+x]=from->cell[y*MAX_WIDTH+x];
    }
  }
}

Board *up_board(Board *now,int x,int y){
  if(y>=2&&now->cell[(y-2)*MAX_WIDTH+x]==0&&now->cell[(y-1)*MAX_WIDTH+x]==1){
    Board *next=(Board *)malloc(sizeof(Board));
    if (next==NULL) {printf("memory overflow\n");exit(2);}
    next->parent=now;
    board_copy(next,now);
    next->cell[(y-2)*MAX_WIDTH+x]=1;
    next->cell[(y-1)*MAX_WIDTH+x]=0;
    next->cell[(y)*MAX_WIDTH+x]=0;
    return next;
  }else{
    return NULL;
  }
}
Board *down_board(Board *now,int x,int y){
  if(y<=MAX_HEIGHT-3&&now->cell[(y+2)*MAX_WIDTH+x]==0&&now->cell[(y+1)*MAX_WIDTH+x]==1){
    Board *next=(Board *)malloc(sizeof(Board));
    if (next==NULL) {printf("memory overflow\n");exit(2);}
    next->parent=now;
    board_copy(next,now);
    next->cell[(y+2)*MAX_WIDTH+x]=1;
    next->cell[(y+1)*MAX_WIDTH+x]=0;
    next->cell[(y)*MAX_WIDTH+x]=0;
    return next;
  }else{
    return NULL;
  }
}
Board *left_board(Board *now,int x,int y){
  if(x>=2&&now->cell[y*MAX_WIDTH+x-2]==0&&now->cell[y*MAX_WIDTH+x-1]==1){
    Board *next=(Board *)malloc(sizeof(Board));
    if (next==NULL) {printf("memory overflow\n");exit(2);}
    next->parent=now;
    board_copy(next,now);
    next->cell[y*MAX_WIDTH+x-2]=1;
    next->cell[y*MAX_WIDTH+x-1]=0;
    next->cell[y*MAX_WIDTH+x]=0;
    return next;
  }else{
    return NULL;
  }
}
Board *right_board(Board *now,int x,int y){
  if(x<=MAX_WIDTH-3&&now->cell[y*MAX_WIDTH+x+2]==0&&now->cell[y*MAX_WIDTH+x+1]==1){
    Board *next=(Board *)malloc(sizeof(Board));
    if (next==NULL) {printf("memory overflow\n");exit(2);}
    next->parent=now;
    board_copy(next,now);
    next->cell[y*MAX_WIDTH+x+2]=1;
    next->cell[y*MAX_WIDTH+x+1]=0;
    next->cell[y*MAX_WIDTH+x]=0;
    return next;
  }else{
    return NULL;
  }
}
