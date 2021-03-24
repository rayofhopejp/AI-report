import numpy as np
import matplotlib.pyplot as plt
#日本語フォントを導入してください
#参考:https://qiita.com/knknkn1162/items/be87cba14e38e2c0f656
plt.rcParams["font.family"] = "IPAGothic" 
 
# 折れ線グラフを出力
x = np.array([i+1 for i in range(10)])
y1 = np.array([461.183144,0.001737,0.000602,0.000279,0.000077, 0.000029,0.000011,0.000006,0.000002,0.000006])
y2 = np.array([0.120077,0.000471,0.000241,0.000114,0.000067,0.000020,0.000008,0.000004,0.000004,0.000001])
plt.plot(x, y1)
plt.plot(x, y2)
ax = plt.gca()
ax.set_yscale("log")
plt.xlabel("手数")
plt.ylabel("秒数[s]")
plt.savefig("32graph.png")