# astruct
Data structure package in cpython for Python-3.6.x >. Currently only contains AVL tree for sorted
 fast insert, delete and search. Current version is working, but more features and optimalization is planned. 

## Install

clone, run setup.py and add directory to path.


## Usage


```python
# Initialize
tree = AVLTree()

# Add: key value node to tree
tree.push(key, value)

# Search: Returns value of first node where key is a match.
tree.search(key)

# Delete: Delets first found node matching key. Returns True if item is deleted, False if key is not found
tree.delete(key)


```

