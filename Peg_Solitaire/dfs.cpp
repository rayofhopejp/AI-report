#include "peg_solitaire.h"

int node_count=0;

void dfs(Board *now,int depth){
  //ノードの展開数をインクリメント
  node_count++;
  //nowについて、
  //BGと一致していればanswerをprintする。
  if(is_board_equal(now,&BG)){
    print_answer(now);
    printf("depth:%d\n%d nodes\n",depth,node_count);
    return;
  }
  //可能な手を調べる
  //可能な手があれば、そのboardをmallocして生成し、dfsに突っ込む。
  //そのときそのparentはnowとする。
  for(int y=0;y<MAX_HEIGHT;y++){
    for(int x=0;x<MAX_WIDTH;x++){
      if(now->cell[y*MAX_WIDTH+x]!=1)continue;
      //玉がある場合移動させる
      Board *next;
      //上
      next=up_board(now,x,y);
      if(next)dfs(next,depth+1);
      //下
      next=down_board(now,x,y);
      if(next)dfs(next,depth+1);
      //左
      next=left_board(now,x,y);
      if(next)dfs(next,depth+1);
      //右
      next=right_board(now,x,y);
      if(next)dfs(next,depth+1);
    }
  }
  
}
int main(){
  dfs(&B0,1);
  return 0;
}