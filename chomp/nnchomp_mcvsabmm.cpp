/*幅優先探索による3x5チョンプソルバー*/
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <vector>
#include <assert.h>
#include <math.h>
#include <time.h>

using namespace std;

#define HEIGHT 10
#define WIDTH 10

#define MY_TURN 0
#define YOUR_TURN 1

#define MY_WIN 0
#define YOUR_WIN 1

#define INF 1001001001

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
  Board *now=fin;
  while(now!=NULL){
    if(turn==0)printf("0 TURN\n");
    if(turn==1)printf("1 TURN\n");
    print_board(now);
    now=now->parent;
    turn=(turn+1)%2;
  }
}

bool canbite(Board *now,int bx,int by){
  assert(bx<=WIDTH && by<=HEIGHT);
  if(now->cell[by*WIDTH+bx]==-1)return false;//かじれない
  else return true;
}
bool anycanbite(Board *now){
  for(int y=0;y<HEIGHT;y++){
    for(int x=0;x<WIDTH;x++){
      if(canbite(now,x,y))return true;
    }
  }
  return false;
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

int board_evaluation(Board *now,int turn,int evaluation){
  if(evaluation==0){
    //ゲームが終わる（かじるところがなくなった）時MY_TURNなら100,YOUR_TURNなら-100とする
    if(!anycanbite(now)){
      if(turn==MY_TURN)return +100;
      if(turn==YOUR_TURN)return -100;
    }
  }else if(evaluation==1){
    //ゲームが終わる（かじるところがなくなった）時MY_TURNなら100,YOUR_TURNなら-100とする
    //残りの噛めるところが奇数だったらMY_TURNの時-(HEIGHT*WIDTH-残りの噛めるところ),YOUR_TURNの時-(HEIGHT*WIDTH-残りの噛めるところ)を返す
    //偶数の時、逆
    int count=0;
    for(int y=0;y<HEIGHT;y++){
      for(int x=0;x<WIDTH;x++){
        if(canbite(now,x,y))count++;
      }
    }
    if(count==0){
      if(turn==MY_TURN)return +100;
      if(turn==YOUR_TURN)return -100;
    }else{
      if(count%2==1)return -(HEIGHT*WIDTH-count)+turn*2*(HEIGHT*WIDTH-count);
      else return (HEIGHT*WIDTH-count)-turn*2*(HEIGHT*WIDTH-count);
    }
  }
  return 0;
}

int minmax_rec_ab(Board *now,int turn,int left_depth,int alpha, int beta,int evaluation){
  //MY_TURNの時はboard_evaluationの値がmaxのものを、
  //YOUR_TURNの時はboard_evaluationの値がminのものを選んでその値を返す
  if(now==NULL)return 0;
  if(left_depth==0)return board_evaluation(now,turn,evaluation);
  if(!anycanbite(now))return board_evaluation(now,turn,evaluation);//噛めない時、board_evaluationの値を返す
  if(turn==MY_TURN){//MAX手番
    int max_score=-INF;
    for(int y=0;y<HEIGHT;y++){
      for(int x=0;x<WIDTH;x++){
        Board *next=bite(now,x,y);
        if(next){
          int score=minmax_rec_ab(next,(turn+1)%2,left_depth-1,max_score,INF,evaluation);
          max_score=max(max_score,score);
          // beta-cut
          if(max_score>=beta){
              return max_score;
          }
          if(next)free(next);
        }
      }
    }
    return max_score;
  }else{ //MIN手番
    int min_score=INF;
    for(int y=0;y<HEIGHT;y++){
      for(int x=0;x<WIDTH;x++){
        Board *next=bite(now,x,y);
        if(next){
          int score=minmax_rec_ab(next,(turn+1)%2,left_depth-1,-INF,min_score,evaluation);
          min_score=min(min_score,score);
          //alpha-cut
          if(min_score<=alpha){
              return min_score;
          }
          if(next)free(next);
        }
      }
    }
    return min_score;
  }
}
Board *minmax_ab(Board *now,int left_depth,int evaluation){ //alpha-beta cut
  if(!anycanbite(now)){//ゲーム終了時はnullをかえす
    return NULL;
  }
  //left_depthが0になるまで探索し、maxな手を返す
  Board *ret=NULL;
  int max_score=-INF;
  for(int y=0;y<HEIGHT;y++){
    for(int x=0;x<WIDTH;x++){
      Board *next=bite(now,x,y);
      if(next){
        int score=minmax_rec_ab(next,YOUR_TURN,left_depth-1,max_score,INF,evaluation);
        if(score>max_score){
          if(ret)free(ret);
          ret=next;
          max_score=score;
        }
      }
    }
  }
  assert(ret);
  return ret;//maxになるboardを返す
}

vector<pair<int,int> > find_possible_canditate(Board *now){
  vector<pair<int,int> > ret;
  for(int y=0;y<HEIGHT;y++){
    for(int x=0;x<WIDTH;x++){
      if(canbite(now,x,y))ret.push_back(make_pair(x,y));
    }
  }
  return ret;
}
int playout_function(Board *now,int turn){
  if(!anycanbite(now)){
    if(turn==MY_TURN)return 1;
    if(turn==YOUR_TURN)return 0;
  }
  vector<pair<int,int> > possible_canditates=find_possible_canditate(now);
  //ランダムに選ぶ
  int random_number=rand()%possible_canditates.size();
  pair<int,int> selected=possible_canditates[random_number];
  Board *next=bite(now,selected.first,selected.second);
  int ret=playout_function(next,(turn+1)%2);
  if(next)free(next);
  return ret;
}
Board *MonteCarlo(Board *now,int playout_num){//今の盤面、プレイアウト回数を渡すと原始モンテカルロ探索を行う
  if(!anycanbite(now)){//ゲーム終了時はnullをかえす
    return NULL;
  }
  //全ての手についてプレイアウトをplayout_num回行い、評価する
  Board *ret=NULL;
  double max_win_rate=-INF;
  for(int y=0;y<HEIGHT;y++){
    for(int x=0;x<WIDTH;x++){
      Board *next=bite(now,x,y);
      if(next){//噛むことができたとき
        int win_time=0;
        for(int i=0;i<playout_num;i++){
          win_time+=playout_function(next,YOUR_TURN);
        }
        double win_rate=(double)win_time/playout_num;
        //printf("%d %d win_rate: %lf\n",y,x,win_rate);
        if(win_rate>max_win_rate){
          if(ret)free(ret);
          ret=next;
          max_win_rate=win_rate;
        }
      }
    }
  }
  assert(ret);
  return ret;//maxになるboardを返す
}
int main(int argc, char *argv[]){
  int evaluation=0;
  if(argc>=2)evaluation=atoi(argv[1]);
  srand( time(NULL) );
  Board B0;
  for(int y=0;y<HEIGHT;y++){
    for(int x=0;x<WIDTH;x++){
      B0.cell[y*WIDTH+x]=0;
    }
  }
  B0.cell[(HEIGHT-1)*WIDTH]=1;//左下が毒
  B0.parent=NULL;
  Board *now=&B0;
  Board *prev=NULL;
  int turn=0;
  int monte_carlo_win_rate=0;
  for(int i=0;i<100;i++){
    now=&B0;
    prev=NULL;
    turn=0;
    while(now){
      prev=now;
      if(turn==0)now=minmax_ab(now,5,evaluation);
      if(turn==1)now=MonteCarlo(now,1000);
      //printf("%d turn : duration %lf\n",turn,(double)(end-start)/CLOCKS_PER_SEC);
      //if(now)print_board(now);
      turn=(turn+1)%2;
    }
    //print_answer(prev,(turn+1)%2);//試合を逆順に出力
    if(turn==0)monte_carlo_win_rate+=1;
  }
  printf("後手　原始モンテカルロの勝率 %d percent\n",monte_carlo_win_rate);
  monte_carlo_win_rate=0;
  for(int i=0;i<100;i++){
    now=&B0;
    prev=NULL;
    turn=0;
    while(now){
      prev=now;
      if(turn==1)now=minmax_ab(now,5,evaluation);
      if(turn==0)now=MonteCarlo(now,1000);
      //printf("%d turn : duration %lf\n",turn,(double)(end-start)/CLOCKS_PER_SEC);
      //if(now)print_board(now);
      turn=(turn+1)%2;
    }
    //print_answer(prev,(turn+1)%2);//試合を逆順に出力
    if(turn==1)monte_carlo_win_rate+=1;
  }
  printf("先手　原始モンテカルロの勝率 %d percent\n",monte_carlo_win_rate);
  return 0;
}