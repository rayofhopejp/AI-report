/*幅優先探索による3x5チョンプソルバー*/
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <vector>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <map>

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

int board_evaluation(Board *now,int turn){
  //ゲームが終わる（かじるところがなくなった）時MY_TURNなら100,YOUR_TURNなら-100とする
  if(!anycanbite(now)){
    if(turn==MY_TURN)return +100;
    if(turn==YOUR_TURN)return -100;
  }
  return 0;
}

int minmax_rec_ab(Board *now,int turn,int left_depth,int alpha, int beta){
  //MY_TURNの時はboard_evaluationの値がmaxのものを、
  //YOUR_TURNの時はboard_evaluationの値がminのものを選んでその値を返す
  if(now==NULL)return 0;
  if(left_depth==0)return board_evaluation(now,turn);
  if(!anycanbite(now))return board_evaluation(now,turn);//噛めない時、board_evaluationの値を返す
  if(turn==MY_TURN){//MAX手番
    int max_score=-INF;
    for(int y=0;y<HEIGHT;y++){
      for(int x=0;x<WIDTH;x++){
        Board *next=bite(now,x,y);
        if(next){
          int score=minmax_rec_ab(next,(turn+1)%2,left_depth-1,max_score,INF);
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
          int score=minmax_rec_ab(next,(turn+1)%2,left_depth-1,-INF,min_score);
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
Board *minmax_ab(Board *now,int left_depth){ //alpha-beta cut
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
        int score=minmax_rec_ab(next,YOUR_TURN,left_depth-1,max_score,INF);
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

char *zip_board(Board *board,int turn){
  char *ret=(char*)malloc((1+WIDTH+HEIGHT)*sizeof(char));
  ret[0]='0'+turn%2;
  int itr=1;
  for(int y=0;y<HEIGHT;y++){
    for(int x=0;x<WIDTH;x++){
      if(canbite(board,x,y))ret[itr]='0';
      else ret[itr]='1';
      itr++;
    }
  }
  return ret;
}
double calculate_ucb(int total_playout,int playout,int wintime,double c){
  if(playout==0)return (double)INF;
  assert(total_playout>0);
  return (double)wintime/playout+c*sqrt(2.0*(double)log(total_playout)/playout);
}
typedef struct SCORES{
  int playout=0;
  int wintime=0;
}Scores;

Scores UCTexpand(Board *now,int playout_num,double c,int turn){
  //可能な手を全て列挙
  vector<pair<int,int> > possible_canditates=find_possible_canditate(now);
  //可能な板、プレーアウト回数、勝率を全て列挙
  vector<pair<Scores,Board*>> possible_next;
  for(int i=0;i<possible_canditates.size();i++){
    pair<int,int> p=possible_canditates[i];
    Scores score;
    score.playout=0;
    score.wintime=0;
    possible_next.push_back(make_pair(score,bite(now,p.first,p.second)));
  }
  int playout_sum=playout_num*possible_canditates.size();
  for(int total_playout=0;total_playout<playout_sum;total_playout++){
    //UCB値が高いものを選ぶ
    //ただし、一定以上playoutが行われた時はそこでexpand
    //ここでは、playout_sum/3.0とする
    Board *next;
    double max_ucb=-INF;
    int max_itr=-1;
    for(int i=0;i<possible_next.size();i++){
      pair<Scores,Board*> p=possible_next[i];
      if(p.first.playout>(double)playout_sum/3.0){
        Scores score=UCTexpand(p.second,playout_sum-total_playout,c,(turn+1)%2);
        possible_next[i].first.playout+=score.playout;
        possible_next[i].first.wintime+=score.wintime;
        total_playout=playout_sum;
        break;
      }
      double ucb=calculate_ucb(total_playout,p.first.playout,p.first.wintime,c);
      if(ucb>max_ucb){
        next=p.second;
        max_ucb=ucb;
        max_itr=i;
      }
    }
    if(total_playout==playout_sum)break;
    int ifwin=playout_function(next,(turn+1)%2);
    possible_next[max_itr].first.playout+=1;
    possible_next[max_itr].first.wintime+=ifwin;
  }
  //全ての手の勝率を足し合わせる
  Scores score;
  for(int i=0;i<possible_next.size();i++){
    if(possible_next[i].first.playout<=0){
      if(possible_next[i].second)free(possible_next[i].second);
      continue;
    }
    score.playout+=possible_next[i].first.playout;
    score.wintime+=possible_next[i].first.wintime;
    if(possible_next[i].second)free(possible_next[i].second);
  }
  return score;
}
Board *UCT(Board *now,int playout_num,double c){ //UCT法
  if(!anycanbite(now)){//ゲーム終了時はnullをかえす
    return NULL;
  }
  //"0を噛まれていない、1を噛まれている、左上から"(turns)01010..."のようにしたもので盤面を記録
  map<char[1+WIDTH*HEIGHT],pair<int,int>> map_uct;//プレーアウト回数、勝利数
  //可能な手を全て列挙
  vector<pair<int,int> > possible_canditates=find_possible_canditate(now);
  //可能な板、プレーアウト回数、勝率を全て列挙
  vector<pair<Scores,Board*>> possible_next;
  for(int i=0;i<possible_canditates.size();i++){
    pair<int,int> p=possible_canditates[i];
    Scores score;
    score.playout=0;
    score.wintime=0;
    possible_next.push_back(make_pair(score,bite(now,p.first,p.second)));
  }
  int playout_sum=playout_num*possible_canditates.size();
  for(int total_playout=0;total_playout<playout_sum;total_playout++){
    //UCB値が高いものを選ぶ
    //ただし、一定以上playoutが行われた時はそこでexpand
    //ここでは、playout_sum/3.0とする
    Board *next;
    double max_ucb=-INF;
    int max_itr=-1;
    for(int i=0;i<possible_next.size();i++){
      pair<Scores,Board*> p=possible_next[i];
      if(p.first.playout>playout_sum/3){
        Scores score=UCTexpand(p.second,playout_sum-total_playout,c,YOUR_TURN);
        possible_next[i].first.playout+=score.playout;
        possible_next[i].first.wintime+=score.wintime;
        total_playout=playout_sum;
        break;
      }
      double ucb=calculate_ucb(total_playout,p.first.playout,p.first.wintime,c);
      if(ucb>max_ucb){
        next=p.second;
        max_ucb=ucb;
        max_itr=i;
      }
    }
    if(total_playout==playout_sum)break;
    int ifwin=playout_function(next,YOUR_TURN);
    possible_next[max_itr].first.playout+=1;
    possible_next[max_itr].first.wintime+=ifwin;
  }
  //最も勝率が高いものを選ぶ
  double max_win_rate=-INF;
  int max_itr=-1;
  for(int i=0;i<possible_next.size();i++){
    if(possible_next[i].first.playout<=0){
      if(possible_next[i].second)free(possible_next[i].second);
      continue;
    }
    double winrate=(double)possible_next[i].first.wintime/possible_next[i].first.playout;
    if(winrate>max_win_rate){
      max_win_rate=winrate;
      if(max_itr>=0&&possible_next[max_itr].second)free(possible_next[max_itr].second);
      max_itr=i;
    }else{
      if(possible_next[i].second)free(possible_next[i].second);
    }
  }
  return possible_next[max_itr].second;
}
int main(int argc, char *argv[]){
  double c=0.5;
  int prayout_num=100;
  if(argc>=2)prayout_num=atoi(argv[1]);
  if(argc>=3)c=atof(argv[2]);
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
  int UCT_win_rate=0;
  for(int i=0;i<100;i++){
    now=&B0;
    prev=NULL;
    turn=0;
    while(now){
      prev=now;
      if(turn==0)now=UCT(now,prayout_num,c);
      if(turn==1)now=MonteCarlo(now,prayout_num);
      //printf("%d turn : duration %lf\n",turn,(double)(end-start)/CLOCKS_PER_SEC);
      //if(now)print_board(now);
      turn=(turn+1)%2;
    }
    //print_answer(prev,(turn+1)%2);//試合を逆順に出力
    if(turn==1)UCT_win_rate+=1;
  }
  printf("先手　UCTの勝率 %d percent\n",UCT_win_rate);
  UCT_win_rate=0;
  for(int i=0;i<100;i++){
    now=&B0;
    prev=NULL;
    turn=0;
    while(now){
      prev=now;
      if(turn==1)now=UCT(now,prayout_num,c);
      if(turn==0)now=MonteCarlo(now,prayout_num);
      //printf("%d turn : duration %lf\n",turn,(double)(end-start)/CLOCKS_PER_SEC);
      //if(now)print_board(now);
      turn=(turn+1)%2;
    }
    //print_answer(prev,(turn+1)%2);//試合を逆順に出力
    if(turn==0)UCT_win_rate+=1;
  }
  printf("後手 UCTの勝率 %d percent\n",UCT_win_rate);
  return 0;
}