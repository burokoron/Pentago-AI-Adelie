#!/usr/bin/python
# -*- Coding: utf-8 -*-

f1 = open("delpos1.txt", "r")

pos = []

print("read_position1...")
i = 0
line = f1.readline()
words = line.split(" ")
words[1] = float(words[1])
pos.append(words)
while line:
    line = f1.readline()
    words = line.split(" ")
    if(len(words) != 2):
        break
    words[1] = float(words[1])
    pos.append(words)
    i += 1
    if(i % 10000 == 0):
        print(".", end = "", flush=True)
print("total_pos = %d" %(len(pos)))
f1.close()

f1 = open("delpos2.txt", "r")

print("read_position2...")
i = 0
line = f1.readline()
words = line.split(" ")
words[1] = float(words[1])
pos.append(words)
while line:
    line = f1.readline()
    words = line.split(" ")
    if(len(words) != 2):
        break
    words[1] = float(words[1])
    pos.append(words)
    i += 1
    if(i % 10000 == 0):
        print(".", end = "", flush=True)
print("total_pos = %d" %(len(pos)))
f1.close()


print("\nsort_position...");
pos.sort()

f2 = open("delpos.txt", "w")

print("delete_same_position...")
for i in range(len(pos)):
    if(pos[i] != 0):
        val_sum = pos[i][1]
        same = 1
        for j in range(i+1, len(pos), 1):
            if(pos[i][0] == pos[j][0]):
                pos[j][0] = 0
                val_sum += pos[j][1]
                same += 1
            else:
                break
        f2.write("%s %f\n" %(pos[i][0], val_sum / same))
    if(i % 10000 == 0):
        print(".", end = "", flush=True)

f2.close()