# astruct
Data structure package in cpython for Python3. Currently only contains AVL tree for ordered
 fast insert, delete and search. Current version is working, but more features and optimalization is planned. 

## Install

1. git clone https://github.com/AlexKjes/astruct 
2. python3 setup.py install
3. export PYTHONPATH:${PYTHONPATH}:<\path to astruct\>


## Usage


```python
# Initialize
tree = AVLTree()

# Add key value node to tree
tree.push(key, value)

# Add sequnece
arr = np.random.randint(0, 100, (100, 2))
tree.push(arr)

# Search: Returns value of first node where key is a match.
tree.search(key)

# Delete: Delets first found node matching key. Returns True if item is deleted, False if key is not found
tree.delete(key)

# Get subsets of tree
"""
Returns a tree containing values in range min to max
args:
	min : minimum value or None
	max : maximum value or None
kwargs:
	minOp: >= by default
	maxOp: <= by default	
"""
tree.range(10, 30, minOp=">", maxOp="<")


# Returns a tree of the n higest nodes or a copy of it self if n>=len(self)
tree.n_max(30) 

# Returns a tree of the n lowest nodes or a copy of it self if n>=len(self)
tree.n_min(30) 

# Get tree as list [(key, value)]
list(tree)

```

