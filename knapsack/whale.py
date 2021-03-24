import sys
import numpy as np
import random
#パラメータたち
param_W=200 #クジラの数
param_maxev=1000 #最大評価数
param_search=0.1#突然変異の割合
def ifvalid(x,b,w):
    return b-np.dot(x,w)>=0
def calc_fitness(x,p):
    return np.dot(x,p)
def OWOA(b,w,p):
    N=len(w)
    Vs=[]
    #validなVs(クジラたち)をランダムに生成
    for i in range(param_W):
        v=np.random.randint(0,2,N,dtype=int)
        while(not ifvalid(v,b,w)):
            v=np.random.randint(0,2,N)
        Vs.append( v )
    Vs=sorted(Vs,key=lambda x:calc_fitness(x,p),reverse=True)
    bestvector=Vs[0]
    for T in range(param_maxev):
        a=2-2*(T/param_maxev)
        #param_maxev回繰り返す
        for v_ in Vs:
            v=[]
            for i in v_:
                if i<=0.5:
                    v.append(0)
                else:
                    v.append(1)
            v=np.array(v)
            #opposite locationを計算する
            sv=calc_fitness(v,p)
            sb=calc_fitness(bestvector,p)
            vo=np.ones(N,dtype=int)-v
            if(ifvalid(vo,b,w)):
                so=calc_fitness(vo,p)
                if(so>sv and so>sb):
                    bestvector=vo
            if(ifvalid(v,b,w) and sv>sb):
                bestvector=v
        #Vsの値をOWOAを用いてbestvectorに近づける
        for i in range(len(Vs)):
            if(random.random()<0.5):
                r1=np.random.random(N)
                r2=np.random.random(N)
                A=2*a*r1-a
                C=2*r2
                D=C*bestvector - Vs[i]
                Vs[i]=bestvector-A*D
            else:
                D=bestvector - Vs[i]
                l=random.random()*2-1
                b=0.8
                Vs[i]=bestvector+D*np.exp(b*l)*np.cos(2*np.pi*l)
            if(random.random()<param_search):
                Xrand=np.random.randint(0,2,N)
                Vs[i]=Xrand
        #print(bestvector)
    return bestvector

def readknapsacks(problem):
    c_file="./problems/"+problem+"_c.txt"
    with open(c_file,"r") as f:
        lines=f.readlines()
        capacity=int(lines[0])
    w_file="./problems/"+problem+"_w.txt"
    with open(w_file,"r") as f:
        lines=f.readlines()
        weights=np.array( list(map(int,lines)) )
    p_file="./problems/"+problem+"_p.txt"
    with open(p_file,"r") as f:
        lines=f.readlines()
        profits=np.array( list(map(int,lines)) )
    return capacity,weights,profits

if __name__=="__main__":
    if(len(sys.argv)<2):
        exit(1)
    capacity,weights,profits=readknapsacks(sys.argv[1])
    
    point_max=0
    point_min=1001001001001001
    point_ave=0
    point_q_sum=0
    count=0
    for i in range(10):
        answer=OWOA(capacity,weights,profits)
        profitsum=np.dot(answer,profits)
        weightsum=np.dot(answer,weights)
        print(answer,profitsum,weightsum,capacity)
        if(weightsum<=capacity):
            count+=1
            point_max=max(profitsum,point_max)
            point_min=min(profitsum,point_min)
            point_ave+=profitsum
            point_q_sum+=profitsum**2
    point_ave/=count
    point_std=np.sqrt(point_q_sum/count-point_ave**2)
    
    print( count,point_max,point_min,point_ave,point_std )
    