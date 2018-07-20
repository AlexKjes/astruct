#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Jul  9 17:22:35 2018

@author: alex
"""




import astruct as a
import numpy as np
import time


t = a.AVLTree()
while (True):
    
    arr = np.random.randint(0, 100, (100, 2))
    t.push(arr)
    
    t.delete(np.random.randint(0, 100, int(len(t)*.1)))
    
    print(len(list(t)) == len(t))
    """
        if not t.height_test() or not t.balance_test():
            print(i)
            break
          
    if not t.balance_test() or not t.height_test():
        break
     
    """
#print(list(t))

#¤input()

"""
t.push(np.random.randint(0, 1000, (10, 2)))
t.push(np.random.randint(0, 1000, (10, 2)))

for i in range(1000):
    rand = np.random.randint(0, 1000)
    t.push(rand, i)
    l = t.list()
    if not(len(l) == i+1+20):
        assert len(l) == i+1+20, "Tree size is not equal to number of inserted items"
    
    for j in range(len(l)-1):
        if not (l[j] <= l[j+1]):
            assert l[j][0] <= l[j+1][0], "List is not sorted"
            
assert t.balance_test(), "Tree is not propery balanced"

t.push(1000, 10)
assert 10 == t.search(1000)
t.delete(1000)
assert t.search(1000) == None, "Node not deleted"
arr = np.random.randint(0, 1000, (500, 2))
t.push(arr)
t.delete(arr[:, 0])

print(t.balance_test())


arr = [[10, 3], [30, 400], [1000, 50], [1, 43], [143, 2], [53, 1], [545, 43526], [2456, "what ever"]]

t.push(arr)
t.delete([k for k, v in arr])
#print(list(t))



# Speed test
arr = np.random.rand(10000, 2)
tt = a.AVLTree()
start_time = time.time()
for i in range(10000):
    tt.push(arr[i][0], arr[i][1])
        
print("Single insert time: {} i/s".format(10000/(time.time()-start_time)))

arr = arr.reshape((100, 100, 2))
tt = a.AVLTree()
start_time = time.time()
for i in range(100):
    tt.push(arr[i])
        
print("100*100 batch insert time: {} i/s".format(10000/(time.time()-start_time)))


arr = arr.reshape(10000, 2)
tt = a.AVLTree()
start_time = time.time()
tt.push(arr)

print("10k batch insert time: {} i/s".format(10000/(time.time()-start_time)))


"""

