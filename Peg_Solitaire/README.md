## 概要
ペグソリティアを解くためのソースコードです。

## 実行方法(例)
D問題はA*以外の解法では最後まで実行することができません。

A問題/bfs
```
g++ -Wall -std=c++14 -DPROBLEM_A bfs.cpp
./a.out
```
B問題/dfs
```
g++ -Wall -std=c++14 -DPROBLEM_B dfs.cpp
./a.out
```
C問題/astar
```
g++ -Wall -std=c++14 -DPROBLEM_C Astar.cpp
./a.out
```
D問題/astar
```
g++ -Wall -std=c++14 -DPROBLEM_D Astar.cpp
./a.out
```

## 実行結果(展開ノード数)の比較
### A問題
DFS:12 nodes
BFS:13 nodes
A*:4 nodes
### B問題
DFS:354 nodes
BFS:282 nodes
A*:242 nodes
### C問題
DFS:97 nodes
BFS:81 nodes
A*:8 nodes
### D問題
DFS:終了しない
BFS:終了しない
A*:20 nodes
