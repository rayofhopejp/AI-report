/*幅優先探索による3x5チョンプソルバー*/
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <vector>
#include <assert.h>
using namespace std;

#define HEIGHT 3
#define WIDTH 5

#define MY_TURN 0
#define YOUR_TURN 1

#define MY_WIN 0
#define YOUR_WIN 1

typedef struct BOARD {
    char cell[WIDTH*HEIGHT];
    struct BOARD *parent;
}Board;

void print_board(Board *b){
  for(int y=0;y<HEIGHT;y++){
    //枠線
    printf("+");
    for(int x=0;x<WIDTH;x++){
      printf("-+");
    }
    printf("\n");
    //本体
    for(int x=0;x<WIDTH;x++){
      printf("|");
      if(b->cell[y*WIDTH+x]==1){//毒
        printf("*");
      }else if(b->cell[y*WIDTH+x]==0){//板
        printf(" ");
      }else if(b->cell[y*WIDTH+x]==-1){//食べられている
        printf("/");
      }
    }
    printf("|\n");
  }
  //枠線
  printf("+");
  for(int x=0;x<WIDTH;x++){
    printf("-+");
  }
  printf("\n");
}
void print_answer(Board *fin,int turn){
  printf("=============ANSWER FOUND...=============\n");
  Board *now=fin;
  while(now!=NULL){
    if(turn==MY_TURN)printf("MY TURN\n");
    if(turn==YOUR_TURN)printf("YOUR TURN\n");
    print_board(now);
    now=now->parent;
    turn=(turn+1)%2;
  }
}
bool canbite(Board *now,int bx,int by){
  if(now->cell[by*WIDTH+bx]==-1)return false;//かじれない
  else return true;
}
Board *bite(Board *now,int bx,int by){
  if(!canbite(now,bx,by))return NULL;//かじれない
  Board *next=(Board *)malloc(sizeof(Board));
  if(next==NULL){printf("memory overflow\n");exit(2);}
  next->parent=now;
  for(int y=0;y<HEIGHT;y++){
    for(int x=0;x<WIDTH;x++){
      if(x>=bx && y<=by){//かじるところ
        next->cell[y*WIDTH+x]=-1;
      }else{
        next->cell[y*WIDTH+x]=now->cell[y*WIDTH+x];
      }
    }
  }
  return next;
}

int max_depth=0;
Board *max_board=NULL;
int dfs(Board *now,int turn,int depth){
  //MY_TURNの時は下を全部調べて勝っている直下があれば勝っていることとする（それをすればいいので）
  //YOUR_TURNの時は下を全部調べて全てが勝っていないと勝っていることにならない(一つでも負けていれば負けている)
  if(turn==MY_TURN){
    for(int y=0;y<HEIGHT;y++){
      for(int x=0;x<WIDTH;x++){
        if(y==HEIGHT-1 && x==0)continue;
        BOARD *next=bite(now,x,y);
        if(next&&dfs(next,(turn+1)%2,depth+1)==MY_WIN){
          if(depth==0){printf("必勝する手\n");print_board(next);}
          return MY_WIN;
        }
      }
    }
    return YOUR_WIN;//自分が左下しか噛めないまたはどこを噛んでも自分は負ける
  }else{
    bool istherebite=false;
    for(int y=0;y<HEIGHT;y++){
      for(int x=0;x<WIDTH;x++){
        if(y==HEIGHT-1 && x==0)continue;
        BOARD *next=bite(now,x,y);
        if(next)istherebite=true;
        if(next&&dfs(next,(turn+1)%2,depth+1)==YOUR_WIN)return YOUR_WIN;
      }
    }
    if(!istherebite){//相手が左下しか噛めない時
      if(depth>max_depth)max_board=now;
    }
    return MY_WIN;//相手が左下しか噛めないまたはどこを噛んでも相手は負ける
  }
}
int main(){
  Board B0;
  for(int y=0;y<HEIGHT;y++){
    for(int x=0;x<WIDTH;x++){
      B0.cell[y*WIDTH+x]=0;
    }
  }
  B0.cell[(HEIGHT-1)*WIDTH]=1;//左下が毒
  B0.parent=NULL;
  if(dfs(&B0,MY_TURN,0)==MY_WIN)printf("there is 必勝法\n");
  else printf("there is no 必勝法");
  print_answer(max_board,YOUR_TURN);//最長手数の試合をプリント
  return 0;
}



