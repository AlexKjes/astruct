#include "avln.c"
#include "avl.h"



#ifndef AVLTREE
#define AVLTREE


static int avl_init(AVLTree_O* self, PyObject *args, PyObject *kwds) {

	self->root = Py_BuildValue("");
	self->size = 0;
	Py_INCREF(self);	

	return 0;

};


static void avl_dealloc(AVLTree_O* self){

	Py_XDECREF(self->root);
    Py_TYPE(self)->tp_free((PyObject *) self);

}

// recursively appending node to tree	
static void findMeAParent(AVLTree_O* tree, PyObject* root, PyObject* orphan){
	
	AVLNode_O* rt = (AVLNode_O*) root;
	AVLNode_O* orphy = (AVLNode_O*) orphan;	

	if (root == Py_None) {
		tree->root = orphan;
		return;
	} 

	if (PyObject_RichCompareBool(rt->key, orphy->key, Py_GE)){
		if (rt->left == Py_None) {
			Py_INCREF(rt);
			rt->left = orphan;
			orphy->parent = root;
			propagateHeight(tree, rt);
		} else { findMeAParent(tree, rt->left, orphan); }
	} else {
		if (rt->right == Py_None){
			Py_INCREF(rt);
			rt->right = orphan;
			orphy->parent = root;
			propagateHeight(tree, rt);
		} else { findMeAParent(tree, rt->right, orphan); }
	}
	
	

}

static void propagateHeight(AVLTree_O* tree, AVLNode_O* node){
	
	if ((PyObject*)node == Py_None) return;
	/*
	if (balance(tree, node)) {
		propagateHeight(tree, node);
		//if (node->parent != Py_None) { propagateHeight(tree, (AVLNode_O*) node->parent); }
		//return;	
	}
	*/
	int _height = getHeight(node);
	
	if (_height != node->height){
		node->height = _height;
		balance(tree, node);		
		//TODO remember py recursion stuff
		propagateHeight(tree, (AVLNode_O*) node->parent);	
	}	

}

static int balance(AVLTree_O* tree, AVLNode_O* node){

	if (getBalance(node) == -2){
		if (getBalance((AVLNode_O*)node->right) == -1) {
			rotateLeft(tree, (PyObject*) node);
		} else if (getBalance((AVLNode_O*)node->right) == 1) { rotateDoubleLeft(tree, (PyObject*) node); }
		else { printf("Something is horribly wrong\n"); }
		return 1;
	} else if (getBalance(node) == 2){
		if (getBalance((AVLNode_O*) node->left) == 1) {
			rotateRight(tree, (PyObject*) node);
		} else if (getBalance((AVLNode_O*)node->left) == -1) { rotateDoubleRight(tree, (PyObject*) node); }
		else { printf("Something is horribly wrong\n"); }
		return 1;
	}
	return 0;
}

static int getHeight(AVLNode_O* node){
	if ((PyObject*)node == Py_None) return 0;
	int l, r;
	l = r = 0;
	if (node->right != Py_None) r = ((AVLNode_O*)node->right)->height;
	if (node->left != Py_None) l = ((AVLNode_O*)node->left)->height;

	return ((l > r) ? l : r) + 1;

}

static int getBalance(AVLNode_O* node){
	if ((PyObject*)node == Py_None) return 0;
	int l, r;
	l = r = 0;
	if (node->right != Py_None) r = ((AVLNode_O*)node->right)->height;
	if (node->left != Py_None) l = ((AVLNode_O*)node->left)->height;

	return l - r;
}

/* single left rotation
1
 \        3
  3 -->  / \
   \    1   5
	5
*/
void rotateLeft(AVLTree_O* tree, PyObject* root){

	AVLNode_O* rt = (AVLNode_O*) root;
	AVLNode_O* rtp = (rt->parent != Py_None) ? (AVLNode_O*) rt->parent : NULL;
	AVLNode_O* r = (AVLNode_O*) rt->right;
	

	if (!rtp) { 
		tree->root = (PyObject*)r;
		r->parent = Py_BuildValue("");
	} else {
		r->parent = (PyObject*)rtp;
		if (root == rtp->left) { rtp->left = (PyObject*)r; }
		else { rtp->right = (PyObject*)r; }
	} 
	rt->parent = (PyObject*)r;
	rt->right = r->left;
	r->left = (PyObject*)rt;

	if (rt->right != Py_None) ((AVLNode_O*)rt->right)->parent = (PyObject*) rt;

	rt->height = getHeight(rt);

}

/* Double left rotation
1
 \        2
  3 -->  / \
 /      1   3
2
*/
void rotateDoubleLeft(AVLTree_O* tree, PyObject* root){

	AVLNode_O* rt = (AVLNode_O*) root;
	AVLNode_O* rtp = (rt->parent != Py_None) ? (AVLNode_O*) rt->parent : NULL;
	AVLNode_O* r = (AVLNode_O*)rt->right;
	AVLNode_O* rl = (AVLNode_O*)r->left;
	
	if (!rtp) { 
		tree->root = (PyObject*)rl; 
		rl->parent = Py_BuildValue("");
	} else {
		rl->parent = rt->parent;
		if (root == rtp->left) { rtp->left = (PyObject*)rl; }
		else { rtp->right = (PyObject*)rl; }
	}
	r->left = rl->right;
	rt->right = rl->left;
	r->parent = (PyObject*) rl;
	rt->parent = (PyObject*) rl;
	rl->left = (PyObject*) rt;
	rl->right = (PyObject*) r;

	if (rt->right != Py_None) ((AVLNode_O*)rt->right)->parent = (PyObject*) rt;
	if (r->left != Py_None) ((AVLNode_O*)r->left)->parent = (PyObject*) r;	
	
	rt->height = getHeight(rt);
	r->height = getHeight(r);
	
}

// right rotation, same as mirrored left rotation
void rotateRight(AVLTree_O* tree, PyObject* root){

	AVLNode_O* rt = (AVLNode_O*) root;
	AVLNode_O* rtp = (rt->parent != Py_None) ? (AVLNode_O*) rt->parent : NULL;
	AVLNode_O* l = (AVLNode_O*) rt->left;
	
	if (!rtp) { 
		tree->root = (PyObject*) l; 
		l->parent = Py_BuildValue("");
	} else {
		l->parent = rt->parent;
		if (root == rtp->left) { rtp->left = (PyObject*)l; }
		else { rtp->right = (PyObject*) l; }
	}
	rt->parent = (PyObject*) l;
	rt->left = l->right;
	l->right = (PyObject*)rt;

	if (rt->left != Py_None) ((AVLNode_O*)rt->left)->parent = (PyObject*) rt;
	
	rt->height = getHeight(rt);

}

// doubel right rotation. Same as doubleLeft only mirrored
void rotateDoubleRight(AVLTree_O* tree, PyObject* root){

	AVLNode_O* rt = (AVLNode_O*) root;
	AVLNode_O* rtp = (rt->parent != Py_None) ? (AVLNode_O*) rt->parent : NULL;
	AVLNode_O* l = (AVLNode_O*)rt->left;
	AVLNode_O* lr = (AVLNode_O*)l->right;
	
	if (!rtp) { 
		tree->root = (PyObject*) lr;
		lr->parent = Py_BuildValue("");		
	} else {
		lr->parent = rt->parent;
		if (root == rtp->left) { rtp->left = (PyObject*) lr; }
		else { rtp->right = (PyObject*) lr; }
	}
	l->right = lr->left;
	rt->left = lr->right;
	l->parent = (PyObject*) lr;
	rt->parent = (PyObject*) lr;
	lr->right = (PyObject*) rt;
	lr->left = (PyObject*) l;

	if (rt->left != Py_None) ((AVLNode_O*)rt->left)->parent = (PyObject*) rt;
	if (l->right != Py_None) ((AVLNode_O*)l->right)->parent = (PyObject*) l;	

	rt->height = getHeight(rt);
	l->height = getHeight(l);

}


static PyObject* avl_search(AVLTree_O* self, PyObject* value){

	AVLNode_O* n = (AVLNode_O*) self->root;

	while (1){
		if ((PyObject*) n == Py_None) return Py_None;
		if (PyObject_RichCompareBool(n->key, value, Py_GT)){
			n = (AVLNode_O*) n->left;
		} else if ((PyObject_RichCompareBool(n->key, value, Py_LT))){
			n = (AVLNode_O*) n->right;
		} else {
			return (PyObject*) n;
		}

	}	

}

// deletes first instance of key found
static PyObject* avl_delete(AVLTree_O* self, PyObject* value){

	AVLNode_O* n = (AVLNode_O*) avl_search(self, value);
	if ((PyObject*)n == Py_None) return Py_False;
	
	// if n has no children
	if (n->left == Py_None && n->right == Py_None){
		if (n->parent != Py_None){
			if (isLeftChildOfParent(n)){
				((AVLNode_O*)n->parent)->left = Py_None;
			} else { ((AVLNode_O*)n->parent)->right = Py_None; }
		}
		
	// if n has one child
	} else if ((n->left == Py_None || n->right == Py_None) && (n->left != n->right)){
		AVLNode_O* child = (AVLNode_O*)((n->left != Py_None) ? n->left : n->right);
		parentSwap(self, n, child);
	} else {
		// if node to be deleted has two children
		AVLNode_O* replacement = (AVLNode_O*)n->right;
		// find left most node in subtree
		while (replacement->left != Py_None){
			replacement = (AVLNode_O*)replacement->left;
		}
		// if left most node has right child
		if (replacement->right != Py_None){
			((AVLNode_O*)replacement->parent)->left = replacement->right;
			((AVLNode_O*)replacement->right)->parent = replacement->parent;
		}
		// replace 
		replacement->left = n->left;
		replacement->right = n->right;
		parentSwap(self, n, replacement);
	}

	Py_XDECREF(n);
		

	return Py_True;
}


///////////////////////
// Operation helpers //
///////////////////////

static int isLeftChildOfParent(AVLNode_O* node){
	
	if ((PyObject*) node == ((AVLNode_O*)node->parent)->left) return 1;

	return 0;

}

static void parentSwap(AVLTree_O* tree, AVLNode_O* parent, AVLNode_O* child){
	
	if (parent->parent == Py_None) {
		tree->root = (PyObject*)child;
		return;
	}
	child->parent = parent->parent;
	if (isLeftChildOfParent(parent)){
		((AVLNode_O*)parent->parent)->left = (PyObject*)child;
		return;
	}
	((AVLNode_O*)parent->parent)->right = (PyObject*)child;
	return;

}

////////////////////////////
// python exposed helpers //
////////////////////////////

static void appendElements(PyObject* list, PyObject* node){
	
	if (node == Py_None) return;
	else {
		AVLNode_O* n = (AVLNode_O*) node;
		appendElements(list, n->left);
		PyList_Append(list, Py_BuildValue("OO", n->key, n->value));
		appendElements(list, n->right);
	}
	
}

// recursively prints key/value in ascending order
static PyObject* avl_print_traverse(PyObject* node){
	AVLNode_O* n = (AVLNode_O*)node;

	if (node == Py_None) { return Py_BuildValue(""); }
	avl_print_traverse(n->left);
	printf("%s: %s\n", PyUnicode_AsUTF8(PyObject_Repr(n->key)), PyUnicode_AsUTF8(PyObject_Repr(n->value)));
	avl_print_traverse(n->right);
	return Py_BuildValue("");
}

// finds height of node recursively
static int getHeightRecursive(AVLNode_O* node){
	if ((PyObject*)node == Py_None) return 0;
	
	int l, r;
	l = r = 0;
	if (node->right != Py_None) r = getHeightRecursive((AVLNode_O*)node->right);
	if (node->left != Py_None) l = getHeightRecursive((AVLNode_O*)node->left); 

	return ((l > r) ? l : r) + 1;
}

// returns balance from recursive height
static int getBalanceRecursive(AVLNode_O* node){
	if ((PyObject*)node == Py_None) return 0;
	
	int l, r;
	l = r = 0;
	if (node->right != Py_None) r = getHeightRecursive((AVLNode_O*)node->right);
	if (node->left != Py_None) l = getHeightRecursive((AVLNode_O*)node->left); 

	return l - r;
}

// recursively calculates and puts balance of all nodes in list
static void balanceTestRecursive(AVLNode_O* node, int* nodeBalanceList, int* counter){
	if ((PyObject*)node == Py_None) return;
	balanceTestRecursive((AVLNode_O*)node->left, nodeBalanceList, counter);
	nodeBalanceList[*counter] = getBalanceRecursive(node);
	(*counter)++;
	balanceTestRecursive((AVLNode_O*)node->right, nodeBalanceList, counter);
}




/* 
	Diagnostics
*/

// prints tree in ascending order
static PyObject* avl_print_asc(AVLTree_O* self){
	avl_print_traverse(self-> root);
	return Py_BuildValue("");
}

// Checks that the tree is balanced from seperatly calculated height
static PyObject* balanceTest(AVLTree_O* self){
	int list[self->size];
	int counter = 0;
	balanceTestRecursive((AVLNode_O*)self->root, list, &counter);
	for (int i=0; i<self->size;i++){
		if (list[i] > 2 || list[i]< -2){
			return Py_False;
		}
	}
	return Py_True;

}

/// Python exposed methods

static PyObject* avl_push(AVLTree_O* self, PyObject* args){
	
	PyObject* node = PyObject_CallObject((PyObject *) &AVLNode_T, args);
	Py_INCREF(node);	


	self->size++;
	findMeAParent(self, self->root, node);
	
	return Py_BuildValue("");
}


static PyObject* avl_to_list(AVLTree_O* self){
	
	PyObject* list = Py_BuildValue("[]");
	appendElements(list, self->root);
	return list;

}

static PyObject* searchWraper(AVLTree_O* self, PyObject* args){
	printf("swag\n");	
	PyObject* key = NULL;
	PyArg_ParseTuple(args, "|O", &key);

	printf("yolo\n");
	PyObject* ret = avl_search(self, key);
	if (ret == Py_None){ return ret; }
	return ((AVLNode_O*)ret)->value;

}

static PyObject* deleteWraper(AVLTree_O* self, PyObject* args){
	
	PyObject* key = NULL;
	PyArg_ParseTuple(args, "|O", &key);
	
	return avl_delete(self, key);

}


static PyMethodDef avl_methods[] = {
    {"push", (PyCFunction) avl_push, METH_VARARGS,
     "Push a new node to the tree"
    },
	{"print_asc", (PyCFunction) avl_print_asc, METH_NOARGS,
	 "Prints tree contents in ascending order."},
	{"list", (PyCFunction) avl_to_list, METH_NOARGS,
	 "Returns the tree as a list."},
	{"balance_test", (PyCFunction) balanceTest, METH_NOARGS,
	 "Returns True if balance of all nodes are in range [-2, 2]. Only for testing purposes."},
	{"search", (PyCFunction) searchWraper, METH_VARARGS,
	 "Searches for a key. Returns value first match or None, if no match is found."},
	{"delete", (PyCFunction) deleteWraper, METH_VARARGS,
	 "Deletes first matching instance. Returns True if a node is deleted, False if no match was found"},
    {NULL}  
};


static PyTypeObject AVLTree_T = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "astruct.AVLTree",
    .tp_doc = "An AVLtree",
    .tp_basicsize = sizeof(AVLTree_O),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc) avl_init,
    .tp_dealloc = (destructor) avl_dealloc,
    .tp_methods = avl_methods,
};


#endif

