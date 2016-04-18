#!/usr/bin/python

import numpy as np

hamlet= np.ndarray(shape = (20, 12), dtype = np.float64, order = 'C')
pangurban = np.ndarray(shape = (20, 12), dtype = np.float64, order = 'C')
arabiannights = np.ndarray(shape = (20, 12), dtype = np.float64, order = 'C')

def fillArray(name, list):
    for i in range(1, 21):
        f = open('output/' + name + '/' + str(i), 'r')
        for j in range(0, 12):
            list[i - 1][j] = float(f.readline())

def printMeanAndStdDev(name, list):
    means = np.mean(list, axis = 0, dtype = np.float64)
    stddevs = np.std(list, axis = 0, dtype = np.float64, ddof = 1)
    print(name + " - 1 process, 100b buffer mean: " + str(means[0]))
    print(name + " - 1 process, 100b buffer std dev: " + str(stddevs[0]))
    print(name + " - 2 processes, 100b buffer mean: " + str(means[1]))
    print(name + " - 2 processes, 100b buffer std dev: " + str(stddevs[1]))
    print(name + " - 4 processes, 100b buffer mean: " + str(means[2]))
    print(name + " - 4 processes, 100b buffer std dev: " + str(stddevs[2]))
    print(name + " - 8 processes, 100b buffer mean: " + str(means[3]))
    print(name + " - 8 processes, 100b buffer std dev: " + str(stddevs[3]))
    print(name + " - 1 process, 1024b buffer mean: " + str(means[4]))
    print(name + " - 1 process, 1024b buffer std dev: " + str(stddevs[4]))
    print(name + " - 2 processes, 1024b buffer mean: " + str(means[5]))
    print(name + " - 2 processes, 1024b buffer std dev: " + str(stddevs[5]))
    print(name + " - 4 processes, 1024b buffer mean: " + str(means[6]))
    print(name + " - 4 processes, 1024b buffer std dev: " + str(stddevs[6]))
    print(name + " - 8 processes, 1024b buffer mean: " + str(means[7]))
    print(name + " - 8 processes, 1024b buffer std dev: " + str(stddevs[7]))
    print(name + " - 1 process, 10240b buffer mean: " + str(means[8]))
    print(name + " - 1 process, 10240b buffer std dev: " + str(stddevs[8]))
    print(name + " - 2 processes, 10240b buffer mean: " + str(means[9]))
    print(name + " - 2 processes, 10240b buffer std dev: " + str(stddevs[9]))
    print(name + " - 4 processes, 10240b buffer mean: " + str(means[10]))
    print(name + " - 4 processes, 10240b buffer std dev: " + str(stddevs[10]))
    print(name + " - 8 processes, 10240b buffer mean: " + str(means[11]))
    print(name + " - 8 processes, 10240b buffer std dev: " + str(stddevs[11]))

fillArray("hamlet", hamlet)
fillArray("pangurban", pangurban)
fillArray("arabiannights", arabiannights)
printMeanAndStdDev("hamlet", hamlet)
printMeanAndStdDev("pangurban", pangurban)
printMeanAndStdDev("arabiannights", arabiannights)