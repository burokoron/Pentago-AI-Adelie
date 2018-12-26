#!/usr/bin/python
# -*- Coding: utf-8 -*-

import numpy as np

f1 = open("augpos.txt", "r")

pos = []

print("read_position...")
i = 0
line = f1.readline()
words = line.split(" ")
pos.append(words)
while line:
    line = f1.readline()
    words = line.split(" ")
    if(len(words) != 2):
        break
    pos.append(words)
    i += 1
    if(i % 10000 == 0):
        print(".", end = "", flush=True)
print("total_pos = %d" %(len(pos)))


print("\nsort_position...");
pos.sort()

f2 = open("delpos.txt", "w")

print("delete_same_position...")
for i in range(len(pos)):
    if(pos[i][0] != 0):
        val_sum = float(pos[i][1])
        same = 1
        for j in range(i+1, len(pos), 1):
            if(pos[i][0] == pos[j][0]):
                pos[j][0] = 0
                if(abs(float(pos[j][1])) < 200):
                    val_sum += float(pos[j][1])
                    same += 1
            else:
                break
        f2.write("%s %f\n" %(pos[i][0], val_sum / same))
    if(i % 10000 == 0):
        print(".", end = "", flush=True)

f1.close()
f2.close()