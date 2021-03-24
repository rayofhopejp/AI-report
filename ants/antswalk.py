import numpy as np
from random import random
import matplotlib.pyplot as plt
from math import *
Stride = 10 #歩幅

#線分交差判定
def judge(p1,p2,p3,p4):#p1-p2 vs p3-p4
    t1 = (p1[0] - p2[0]) * (p3[1] - p1[1]) + (p1[1] - p2[1]) * (p1[0] - p3[0])
    t2 = (p1[0] - p2[0]) * (p4[1] - p1[1]) + (p1[1] - p2[1]) * (p1[0] - p4[0])
    t3 = (p3[0] - p4[0]) * (p1[1] - p3[1]) + (p3[1] - p4[1]) * (p3[0] - p1[0])
    t4 = (p3[0] - p4[0]) * (p2[1] - p3[1]) + (p3[1] - p4[1]) * (p3[0] - p2[0])
    return t1*t2<0 and t3*t4<0
#対称移動
def reflection(p1,p2,p3):#p1をp2-p3に対して対称移動させる
    p1 = np.array(p1)
    p2 = np.array(p2)
    p3 = np.array(p3)
    N = -1  
    v = (p3 - p2) / np.linalg.norm(p3 - p2)  # 直線の方向を表す単位ベクトル
    h = (p1 - p2) @ v * v + p2  # 点 P から直線に対して下ろした垂線
    Q = h + (p1 - h) * N  # N 倍した点
    return Q[0],Q[1]
#距離の計算
def distance(p1,p2):
    return np.sqrt((p1[0]-p2[0])**2+(p1[1]-p2[1])**2)

def randomwalk(N,walls,start):
    x_list=[0]
    y_list=[0]
    x=start[0]
    y=start[1]
    n=0
    while(n<N):
        theta=2.0*pi*random()#角度θ(0~2pi)
        nx = x+Stride*cos(theta) # x方向への移動。cos(θ)。
        ny = y+Stride*sin(theta) # y方向への移動。sin(θ)
        #壁と交わっていたらその壁に対して対称に反射する
        while(True):
            ischange=False
            for wall in walls:#wall:[p1,p2]
                if(judge([x,y],[nx,ny],wall[0],wall[1])):#交わっている
                    nx,ny=reflection([nx,ny],wall[0],wall[1])
                    ischange=True
            if(not ischange):
                break
        x_list.append (nx) # xの値をx_listに格納していく
        y_list.append(ny) # yの値をx_listに格納していく
        n+=distance([x,y],[nx,ny])
        x=nx
        y=ny
    return x_list,y_list #動いた履歴を返す

def randomwalk_with_count_collision(N,walls,start,first_walk):
    x_list=[0]
    y_list=[0]
    x=start[0]
    y=start[1]
    n=0
    collision=0
    while(n<N):
        theta=2.0*pi*random()#角度θ(0~2pi)
        nx = x+Stride*cos(theta) # x方向への移動。cos(θ)。
        ny = y+Stride*sin(theta) # y方向への移動。sin(θ)
        #壁と交わっていたらその壁に対して対称に反射する
        while(True):
            ischange=False
            for wall in walls:#wall:[p1,p2]
                if(judge([x,y],[nx,ny],wall[0],wall[1])):#交わっている
                    nx,ny=reflection([nx,ny],wall[0],wall[1])
                    ischange=True
            if(not ischange):
                break
        x_list.append (nx) # xの値をx_listに格納していく
        y_list.append(ny) # yの値をx_listに格納していく
        n+=distance([x,y],[nx,ny])
        #衝突回数を調べる
        for i in range( len(first_walk[0])-1 ):
            bx1=first_walk[0][i]
            bx2=first_walk[0][i+1]
            by1=first_walk[1][i]
            by2=first_walk[1][i+1]
            if(judge([x,y],[nx,ny],[bx1,by1],[bx2,by2])):
                collision+=1
        x=nx
        y=ny
    return x_list,y_list,collision #動いた履歴を返す


L=10000
S=10000





#standard  nest
standard=[
    [[-50,-10],[50,-10]],
    [[50,-10],[50,90]],
    [[50,90],[-50,90]],
    [[-50,90],[-50,-10]]
    ]
#half-size
halfsize=[
    [[-35.5,-10],[35.5,-10]],
    [[35.5,-10],[35.5,61]],
    [[35.5,61],[-35.5,61]],
    [[-35.5,61],[-35.5,-10]]
    ]
#half-size same length
halfsize2=[
    [[-14.5,-10],[14.5,-10]],
    [[14.5,-10],[14.5,161]],
    [[14.5,161],[-14.5,161]],
    [[-14.5,161],[-14.5,-10]]
    ]
#standard nest + wall
standard2=[
    [[-50,-10],[50,-10]],
    [[50,-10],[50,90]],
    [[50,90],[-50,90]],
    [[-50,90],[-50,-10]],
    [[0,0],[0,80]]#壁
    ]
#half-size + hole
halfsize3=[
    [[-35.5,-10],[35.5,-10]],
    [[35.5,-10],[35.5,61]],
    [[35.5,61],[-35.5,61]],
    [[-35.5,61],[-35.5,-10]],
    [[0,-10],[0,61]],#穴
    [[-35.5,13],[35.5,13]],#穴
    [[-35.5,36],[35.5,36]]#穴
    ]
try_size=100 #試行回数
walls_set=[standard,halfsize,halfsize2,standard2,halfsize3]
labels=["(a)","(b)","(c)","(d)","(e)"]
#plot
fig=plt.figure()
axs=[]
for i in range(5):
    axs.append( fig.add_subplot(2, 3, i+1) )
for label,walls,i in zip(labels,walls_set,list(range(5))):
    x_list1,y_list1=randomwalk(L,walls,[0,0])
    x_list2,y_list2,collision=randomwalk_with_count_collision(S,walls,[0,0],[x_list1,y_list1])
    axs[i].plot(x_list1,y_list1) # (x,y)のプロット
    axs[i].plot(x_list2,y_list2,color="red") # (x,y)のプロット
    for wall in walls:
        x,y=zip(*wall)
        axs[i].plot(x,y,color="black") # (x,y)のプロット
    axs[i].set_xlim([-50,50])  # x軸の範囲
    axs[i].set_ylim([-10,180]) # y軸の範囲
    axs[i].set_title(label)
plt.show()
#caluculate
for label,walls in zip(labels,walls_set):
    K_sum=0
    for i in range(try_size):
        x_list1,y_list1=randomwalk(L,walls,[1,0])
        x_list2,y_list2,collision=randomwalk_with_count_collision(S,walls,[1,0],[x_list1,y_list1])
        K_sum+=2*S*L/(pi*collision)
        #print("collison",collision)
        #print("theoretical collison",2*S*L/(pi*10000) )
        #print(i,end=",")
    print(label,K_sum/try_size)
