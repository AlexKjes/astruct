#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Jul  9 17:22:35 2018

@author: alex
"""

import astruct as a
import numpy as np

t = a.AVLTree()


for i in range(1000):
    rand = np.random.randint(0, 1000)
    t.push(rand, i)
    l = t.list()
    if not(len(l) == i+1):
        assert len(l) == i+1, "Tree size is not equal to number of inserted items"
    
    for j in range(len(l)-1):
        if not (l[j] <= l[j+1]):
            assert l[j][0] <= l[j+1][0], "List is not sorted"
            
assert t.balance_test(), "Tree is not propery balanced"

t.push(1000, 10)
assert 10 == t.search(1000)

t.delete(1000)

assert t.search(1000) == None


#print(t.list())

#t.print_asc()