#!/usr/bin/python

import numpy as np

hamlet= np.ndarray(shape = (20, 12), dtype = float, order = 'C')
pangurban = np.ndarray(shape = (20, 12), dtype = float, order = 'C')
arabiannights = np.ndarray(shape = (20, 12), dtype = float, order = 'C')

def fillArray(name, list):
    for i in range(1, 21):
        f = open('output/' + name + '/' + str(i), 'r')
        for j in range(0, 12):
            list[i - 1][j] = float(f.readline())

fillArray("hamlet", hamlet)
fillArray("pangurban", pangurban)
fillArray("arabiannights", arabiannights)

for p in hamlet:
    print(p)
for p in pangurban:
    print(p)
for  p in arabiannights:
    print(p)
