import sys
import numpy as np
import random
#パラメータたち
param_P=500#カエルの数
param_m=10#memeplexesの数
param_ite=100#memeplexes内での進化の回数
param_pm=0.06 #突然変異の割合


def calc_fitness(x,b,w,p):
    param_rho=0
    for i in range(len(w)):
        param_rho=max(param_rho,p[i]/w[i])
    return np.dot(p,x)-( max(0, param_rho*( np.dot(w,x)-b)) )**2
def local_search(P,b,w,p): 
    N=len(w)
    #P[i] i=0,m,2m...がmemeplex(0) ... i=m-1,2m-1,3m-1...Nがmemeplex(m-1)
    for memeplex in range(param_m):
        for ite in range(param_ite):
            Xg=-1
            Xg_score=-1001001001001001001001
            Xb=-1
            Xb_score=-1001001001001001001001
            Xw=-1
            Xw_score=1001001001001001001001
            #Xb,Xw,Xgの更新
            for i in range(param_P):
                score=calc_fitness(P[i],b,w,p)
                if(score>Xg_score):
                    Xg_score=score
                    Xg=i
                if(i%param_m==memeplex):
                    #X_b,X_gの更新
                    if(score>Xb_score):
                        Xb_score=score
                        Xb=i
                    if(score<Xw_score):
                        Xw_score=score
                        Xw=i
            assert(Xg>=0 and Xb>=0 and Xw>=0 )
            #XwをXw_newに
            Di= np.random.randint(0,2,N) * (P[Xb]-P[Xw])
            Xw_new=P[Xw]+Di
            if calc_fitness(Xw_new,b,w,p)>Xw_score:
                P[Xw]=Xw_new
                continue #to next iteration
            Di= np.random.randint(0,2,N) * (P[Xg]-P[Xw])
            Xw_new=P[Xw]+Di
            if calc_fitness(Xw_new,b,w,p)>Xw_score:
                P[Xw]=Xw_new
                continue #to next iteration
            #randomly change
            Xw_new= np.random.randint(0,2,N)
            P[Xw]=Xw_new
    return 0
            
    return 0
def MDSFLA(b,w,p):
    #Pをランダムに生成
    N=len(w)
    P=[]
    for i in range(param_P):
        P.append( np.random.randint(0,2,N) )
    P=sorted(P,key=lambda x:calc_fitness(x,b,w,p),reverse=True)
    best=P[0]
    count=0
    while(True):
        #適合度を計算
        P=sorted(P,key=lambda x:calc_fitness(x,b,w,p),reverse=True)
        #local search
        local_search(P,b,w,p)
        #mutation
        for i in range(param_P):
            if(random.random()<param_pm):
                P[i]=np.random.randint(0,2,N)
        P=sorted(P,key=lambda x:calc_fitness(x,b,w,p),reverse=True)
        if(calc_fitness(best,b,w,p)==calc_fitness(P[0],b,w,p)):
            count+=1
            if(count>param_ite//15):
                break
        else:
            count=0
        best=P[0]
        #print(best,np.dot(best,profits), np.dot(best,weights),capacity)
    return P[0]

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
        answer=MDSFLA(capacity,weights,profits)
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
    