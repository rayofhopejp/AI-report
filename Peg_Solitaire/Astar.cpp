#include "peg_solitaire.h"

int node_count=0;
int calc_h(Board *now){
  /*
  //案1:残りのコマ数
  int left=0;//残りのコマ数
  for(int y=0;y<MAX_HEIGHT;y++){
    for(int x=0;x<MAX_WIDTH;x++){
        if(now->cell[y*MAX_WIDTH+x]==1){
            left++;
        }
    }
  }
  return left;
  */
 
  /*
  //案2:BGと違うところにあるコマの数-1
  int cnt=0;
  for(int y=0;y<MAX_HEIGHT;y++){
    for(int x=0;x<MAX_WIDTH;x++){
        if(now->cell[y*MAX_WIDTH+x]==1&&BG.cell[y*MAX_WIDTH+x]==0){
            cnt++;
        }
    }
  }
  return cnt;
  */
  /*
  //案3；Xからの距離の合計（ここでの距離はL1ノルムを指す）
  int goalx=0;int goaly=0;
  for(int y=0;y<MAX_HEIGHT;y++){
    for(int x=0;x<MAX_WIDTH;x++){
        if(BG.cell[y*MAX_WIDTH+x]==1){
            goalx=x;goaly=y;
        }
    }
  }
  int norm=0;
  for(int y=0;y<MAX_HEIGHT;y++){
    for(int x=0;x<MAX_WIDTH;x++){
        if(now->cell[y*MAX_WIDTH+x]==1){
            norm+=abs(y-goaly)+abs(x-goalx);
        }
    }
  }
  return norm;
  */
  /**/
  //案４；Xからの距離の合計（ここでの距離はL1ノルムに手を加えたものを指す）
  int goalx=0;int goaly=0;
  for(int y=0;y<MAX_HEIGHT;y++){
    for(int x=0;x<MAX_WIDTH;x++){
        if(BG.cell[y*MAX_WIDTH+x]==1){
            goalx=x;goaly=y;
        }
    }
  }
  int norm=0;
  for(int y=0;y<MAX_HEIGHT;y++){
    for(int x=0;x<MAX_WIDTH;x++){
        if(now->cell[y*MAX_WIDTH+x]==1){
            norm+=abs(y-goaly)/2+abs(x-goalx)/2;
        }
    }
  }
  return norm;
  /**/
}
void astar(Board *start){
  priority_queue<pair<int,pair<int,Board *>>,vector<pair<int,pair<int,Board *>>>, greater<pair<int,pair<int,Board *>>> > q1;
  q1.push(make_pair(calc_h(start),make_pair(0,start)));
  while(!q1.empty()){
    pair<int,pair<int,Board *>> now_infos=q1.top();q1.pop();
    int f_now=now_infos.first;
    int depth_now=now_infos.second.first;
    Board *now=now_infos.second.second;
    node_count++;
    if(node_count%10000==0)printf("%d nodes\n",node_count);
    if(is_board_equal(now,&BG)){print_answer(now);printf("%d nodes\n",node_count);return;}
    //子を見つける
    for(int y=0;y<MAX_HEIGHT;y++){
      for(int x=0;x<MAX_WIDTH;x++){
        if(now->cell[y*MAX_WIDTH+x]!=1)continue;
        Board *next;
        //上
        next=up_board(now,x,y);
        if(next)q1.push(make_pair(depth_now+1+calc_h(next),make_pair(depth_now+1,next)));
        //下
        next=down_board(now,x,y);
        if(next)q1.push(make_pair(depth_now+1+calc_h(next),make_pair(depth_now+1,next)));
        //左
        next=left_board(now,x,y);
        if(next)q1.push(make_pair(depth_now+1+calc_h(next),make_pair(depth_now+1,next)));
        //右
        next=right_board(now,x,y);
        if(next)q1.push(make_pair(depth_now+1+calc_h(next),make_pair(depth_now+1,next)));
        }
    }
  }
}
int main(){
  astar(&B0);
  return 0;
}