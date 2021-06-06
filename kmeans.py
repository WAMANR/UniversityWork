import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

q = 3
k = 4


## Code repris du premier tp
centre1=np.array([3,3])
centre2=np.array([-3,-3])
centre3=np.array([3,-3])
centre4=np.array([-3,3])
sigma=np.array([[1.5,0],[0,1.5]])
taille=200
cluster1=np.random.multivariate_normal(centre1,sigma,taille)
cluster2=np.random.multivariate_normal(centre2,sigma,taille)
cluster3=np.random.multivariate_normal(centre3,sigma,taille)
cluster4=np.random.multivariate_normal(centre4,sigma,taille)
cluster = np.concatenate((cluster1, cluster2, cluster3, cluster4))

## Fonction minkowski
def minkowski(point1,point2):
    return pow(pow(abs(point2[0]-point1[0]), q) + pow(abs(point2[1]-point1[1]), q), 1/q)

## Génération de notre centre de gravité
def make_center():
    return ((-6) - 6) * np.random.random_sample((k, 2)) + 6



def min_distance(point,centre):
    min = 100  
    index = 0
    for c in centre:
        tmp = minkowski(point,c)
        if tmp < min:
            min = tmp
            ck = index
        index = index + 1
    return ck

def k_means():
    uk = make_center()
    cl = [[] for i in range(k)]
    ik = 0
    jw = []
    j = 0
    uk1 = make_center()

    while 1:

        cl = [[] for i in range(k)]
        for point in cluster:
            ck = min_distance(point,uk)
            cl[ck].append(point)

        for liste in cl:
            xs = 0
            ys = 0
            for coord in liste:
                xs += coord[0]
                ys += coord[1]
            if len(liste) != 0:
                xk = (1/len(liste)) * xs
                yk = (1/len(liste)) * ys
                uk[ik] = [xk,yk]
            ik+=1
        end = 1
        for i in range(0,k):
            for j in range(0,2):
                if uk[i][j] != uk1[i][j]:
                    end = 0
        if end == True:
            break
        ik = 0
        uk1 = uk

    for liste in cl:
        plt.scatter([point[0] for point in liste], [point[1] for point in liste])
    plt.show()

k_means()