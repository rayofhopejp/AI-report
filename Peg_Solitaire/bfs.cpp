#include "peg_solitaire.h"


int node_count=0;

void bfs(Board *start){
  queue<Board *> q1;
  q1.push(start);
  while(!q1.empty()){
    Board *now=q1.front();q1.pop();
    node_count++;
    if(is_board_equal(now,&BG)){
      print_answer(now);
      printf("%d nodes\n",node_count);
      return;
    }
    //子を見つける
    for(int y=0;y<MAX_HEIGHT;y++){
      for(int x=0;x<MAX_WIDTH;x++){
        if(now->cell[y*MAX_WIDTH+x]!=1)continue;
        Board *next;
        //上
        next=up_board(now,x,y);
        if(next)q1.push(next);
        //下
        next=down_board(now,x,y);
        if(next)q1.push(next);
        //左
        next=left_board(now,x,y);
        if(next)q1.push(next);
        //右
        next=right_board(now,x,y);
        if(next)q1.push(next);
        }
    }
  }
}
int main(){
  bfs(&B0);
  return 0;
}