#ifndef AVLTREE
#define AVLTREE

#include "avl.h"
#include "binary_cursor.c"
#include "avln.c"

static PySequenceMethods avl_sequence_methods = {
    (lenfunc) tree_size,
};


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
	{"range", (PyCFunction) avl_range, METH_VARARGS | METH_KEYWORDS,
	 "Returns a subtree matching given arguments. Or None if no subtree match exists"},
	{"n_max", (PyCFunction) avl_n_max, METH_VARARGS,
	 "Returns a subtree of n largest values"},
	{"n_min", (PyCFunction) avl_n_min, METH_VARARGS,
	 "Returns a subtree of n smalest values"},
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
	.tp_as_sequence = &avl_sequence_methods,
	.tp_iter = (getiterfunc) avl_iter,
	.tp_iternext = (iternextfunc) avl_iter_next,
};




static int avl_init(AVLTree_O* self, PyObject *args, PyObject *kwds) {

	self->cursor = NULL;

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
		if (rt->children[0] == Py_None) {
			Py_INCREF(rt);
			rt->children[0] = orphan;
			orphy->parent = root;
			//propagateHeight(tree, rt);
		} else { findMeAParent(tree, rt->children[0], orphan); }
	} else {
		if (rt->children[1] == Py_None){
			Py_INCREF(rt);
			rt->children[1] = orphan;
			orphy->parent = root;
			//propagateHeight(tree, rt);
		} else { findMeAParent(tree, rt->children[1], orphan); }
	}	

}

static void propagateHeight(AVLTree_O* tree, AVLNode_O* node){
	
	if ((PyObject*)node == Py_None) return;

	int _height = getHeight(node);
	
	if (_height != node->height){
		node->height = _height;
		balance(tree, node);		
		//TODO remember py recursion stuff
		propagateHeight(tree, (AVLNode_O*) node->parent);	
	}	

}

static int balance(AVLTree_O* tree, AVLNode_O* node){

	if (getBalance(node) < -1){
		if (getBalance((AVLNode_O*)node->children[1]) < 0) {
			rotateLeft(tree, (PyObject*) node);
		} else if (getBalance((AVLNode_O*)node->children[1]) > 0) { rotateDoubleLeft(tree, (PyObject*) node); }
		//else { printf("Something is horribly wrong\n"); }
		return 1;
	} else if (getBalance(node) > 1){
		if (getBalance((AVLNode_O*) node->children[0]) > 0) {
			rotateRight(tree, (PyObject*) node);
		} else if (getBalance((AVLNode_O*)node->children[0]) < 0) { rotateDoubleRight(tree, (PyObject*) node); }
		//else { printf("Something is horribly wrong\n"); }
		return 1;
	}
	return 0;
}

static int getHeight(AVLNode_O* node){
	if ((PyObject*)node == Py_None) return 0;
	int l, r;
	l = r = 0;
	if (node->children[1] != Py_None) r = ((AVLNode_O*)node->children[1])->height;
	if (node->children[0] != Py_None) l = ((AVLNode_O*)node->children[0])->height;

	return ((l > r) ? l : r) + 1;

}

static int getBalance(AVLNode_O* node){
	if ((PyObject*)node == Py_None) return 0;
	int l, r;
	l = r = 0;
	if (node->children[1] != Py_None) r = ((AVLNode_O*)node->children[1])->height;
	if (node->children[0] != Py_None) l = ((AVLNode_O*)node->children[0])->height;

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
	AVLNode_O* r = (AVLNode_O*) rt->children[1];
	

	if (!rtp) { 
		tree->root = (PyObject*)r;
		r->parent = Py_BuildValue("");
	} else {
		r->parent = (PyObject*)rtp;
		if (root == rtp->children[0]) { rtp->children[0] = (PyObject*)r; }
		else { rtp->children[1] = (PyObject*)r; }
	} 
	rt->parent = (PyObject*)r;
	rt->children[1] = r->children[0];
	r->children[0] = (PyObject*)rt;

	if (rt->children[1] != Py_None) ((AVLNode_O*)rt->children[1])->parent = (PyObject*) rt;

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
	AVLNode_O* r = (AVLNode_O*)rt->children[1];
	AVLNode_O* rl = (AVLNode_O*)r->children[0];
	
	if (!rtp) { 
		tree->root = (PyObject*)rl; 
		rl->parent = Py_BuildValue("");
	} else {
		rl->parent = rt->parent;
		if (root == rtp->children[0]) { rtp->children[0] = (PyObject*)rl; }
		else { rtp->children[1] = (PyObject*)rl; }
	}
	r->children[0] = rl->children[1];
	rt->children[1] = rl->children[0];
	r->parent = (PyObject*) rl;
	rt->parent = (PyObject*) rl;
	rl->children[0] = (PyObject*) rt;
	rl->children[1] = (PyObject*) r;

	if (rt->children[1] != Py_None) ((AVLNode_O*)rt->children[1])->parent = (PyObject*) rt;
	if (r->children[0] != Py_None) ((AVLNode_O*)r->children[0])->parent = (PyObject*) r;	
	
	rt->height = getHeight(rt);
	r->height = getHeight(r);
	
}

// right rotation, same as mirrored left rotation
void rotateRight(AVLTree_O* tree, PyObject* root){

	AVLNode_O* rt = (AVLNode_O*) root;
	AVLNode_O* rtp = (rt->parent != Py_None) ? (AVLNode_O*) rt->parent : NULL;
	AVLNode_O* l = (AVLNode_O*) rt->children[0];
	
	if (!rtp) { 
		tree->root = (PyObject*) l; 
		l->parent = Py_BuildValue("");
	} else {
		l->parent = rt->parent;
		if (root == rtp->children[0]) { rtp->children[0] = (PyObject*)l; }
		else { rtp->children[1] = (PyObject*) l; }
	}
	rt->parent = (PyObject*) l;
	rt->children[0] = l->children[1];
	l->children[1] = (PyObject*)rt;

	if (rt->children[0] != Py_None) ((AVLNode_O*)rt->children[0])->parent = (PyObject*) rt;
	
	rt->height = getHeight(rt);

}

// doubel right rotation. Same as doubleLeft only mirrored
void rotateDoubleRight(AVLTree_O* tree, PyObject* root){

	AVLNode_O* rt = (AVLNode_O*) root;
	AVLNode_O* rtp = (rt->parent != Py_None) ? (AVLNode_O*) rt->parent : NULL;
	AVLNode_O* l = (AVLNode_O*)rt->children[0];
	AVLNode_O* lr = (AVLNode_O*)l->children[1];
	
	if (!rtp) { 
		tree->root = (PyObject*) lr;
		lr->parent = Py_BuildValue("");		
	} else {
		lr->parent = rt->parent;
		if (root == rtp->children[0]) { rtp->children[0] = (PyObject*) lr; }
		else { rtp->children[1] = (PyObject*) lr; }
	}
	l->children[1] = lr->children[0];
	rt->children[0] = lr->children[1];
	l->parent = (PyObject*) lr;
	rt->parent = (PyObject*) lr;
	lr->children[1] = (PyObject*) rt;
	lr->children[0] = (PyObject*) l;

	if (rt->children[0] != Py_None) ((AVLNode_O*)rt->children[0])->parent = (PyObject*) rt;
	if (l->children[1] != Py_None) ((AVLNode_O*)l->children[1])->parent = (PyObject*) l;	

	rt->height = getHeight(rt);
	l->height = getHeight(l);

}


static PyObject* avl_search(AVLTree_O* self, PyObject* value){

	AVLNode_O* n = (AVLNode_O*) self->root;

	while (1){
		if ((PyObject*) n == Py_None) return Py_None;
		if (PyObject_RichCompareBool(n->key, value, Py_GT)){
			n = (AVLNode_O*) n->children[0];
		} else if ((PyObject_RichCompareBool(n->key, value, Py_LT))){
			n = (AVLNode_O*) n->children[1];
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
	if (n->children[0] == Py_None && n->children[1] == Py_None){
		if (n->parent != Py_None){
			if (isLeftChildOfParent(n)){
				((AVLNode_O*)n->parent)->children[0] = Py_None;
			} else { ((AVLNode_O*)n->parent)->children[1] = Py_None; }
		}
		
	// if n has one child
	} else if ((n->children[0] == Py_None || n->children[1] == Py_None) && (n->children[0] != n->children[1])){
		AVLNode_O* child = (AVLNode_O*)((n->children[0] != Py_None) ? n->children[0] : n->children[1]);
		parentSwap(self, n, child);
	} else {
		// if node to be deleted has two children
		AVLNode_O* replacement = (AVLNode_O*)n->children[1];
		// find left most node in subtree
		while (replacement->children[0] != Py_None){
			replacement = (AVLNode_O*)replacement->children[0];
		}
		// if left most node has right child
		if (replacement->children[1] != Py_None){
			((AVLNode_O*)replacement->parent)->children[0] = replacement->children[1];
			((AVLNode_O*)replacement->children[1])->parent = replacement->parent;
		}
		// replace 
		replacement->children[0] = n->children[0];
		replacement->children[1] = n->children[1];
		parentSwap(self, n, replacement);
	}

	self->size--;
	Py_XDECREF(n);
		

	return Py_True;
}


///////////////////////
// Operation helpers //
///////////////////////

static int isLeftChildOfParent(AVLNode_O* node){
	
	if ((PyObject*) node == ((AVLNode_O*)node->parent)->children[0]) return 1;

	return 0;

}

static void parentSwap(AVLTree_O* tree, AVLNode_O* parent, AVLNode_O* child){
	
	if (parent->parent == Py_None) {
		tree->root = (PyObject*)child;
		return;
	}
	child->parent = parent->parent;
	if (isLeftChildOfParent(parent)){
		((AVLNode_O*)parent->parent)->children[0] = (PyObject*)child;
		return;
	}
	((AVLNode_O*)parent->parent)->children[1] = (PyObject*)child;
	return;

}





////////////////////////////
// python exposed helpers //
////////////////////////////

//static PyObject* 


static void refinc(AVLNode_O* node){
	Py_INCREF(node);
}

// do thing to each node in tree
static void avl_tree_map(PyObject* node, void (*f)(AVLNode_O*)){
	
	if (node == Py_None) return;

	avl_tree_map(((AVLNode_O*)node)->children[0], f);
	f((AVLNode_O*)node);
	avl_tree_map(((AVLNode_O*)node)->children[1], f);

}


static void appendElements(PyObject* list, PyObject* node){
	
	if (node == Py_None) return;
	else {
		AVLNode_O* n = (AVLNode_O*) node;
		appendElements(list, n->children[0]);
		PyList_Append(list, Py_BuildValue("OO", n->key, n->value));
		appendElements(list, n->children[1]);
	}
	
}

// recursively prints key/value in ascending order
static PyObject* avl_print_traverse(PyObject* node){
	AVLNode_O* n = (AVLNode_O*)node;

	if (node == Py_None) { return Py_BuildValue(""); }
	avl_print_traverse(n->children[0]);
	printf("%s: %s\n", PyUnicode_AsUTF8(PyObject_Repr(n->key)), PyUnicode_AsUTF8(PyObject_Repr(n->value)));
	avl_print_traverse(n->children[1]);
	return Py_BuildValue("");
}

// finds height of node recursively
static int getHeightRecursive(AVLNode_O* node){
	if ((PyObject*)node == Py_None) return 0;
	
	int l, r;
	l = r = 0;
	if (node->children[1] != Py_None) r = getHeightRecursive((AVLNode_O*)node->children[1]);
	if (node->children[0] != Py_None) l = getHeightRecursive((AVLNode_O*)node->children[0]); 

	return ((l > r) ? l : r) + 1;
}

// returns balance from recursive height
static int getBalanceRecursive(AVLNode_O* node){
	if ((PyObject*)node == Py_None) return 0;
	
	int l, r;
	l = r = 0;
	if (node->children[1] != Py_None) r = getHeightRecursive((AVLNode_O*)node->children[1]);
	if (node->children[0] != Py_None) l = getHeightRecursive((AVLNode_O*)node->children[0]); 

	return l - r;
}

// recursively calculates and puts balance of all nodes in list
static void balanceTestRecursive(AVLNode_O* node, int* nodeBalanceList, int* counter){
	if ((PyObject*)node == Py_None) return;
	balanceTestRecursive((AVLNode_O*)node->children[0], nodeBalanceList, counter);
	nodeBalanceList[*counter] = getBalanceRecursive(node);
	(*counter)++;
	balanceTestRecursive((AVLNode_O*)node->children[1], nodeBalanceList, counter);
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
	
	
	PyObject* krr = NULL;
	PyObject* val = NULL;
	int is_seq;
	PyArg_ParseTuple(args, "O|O", &krr, &val);
	is_seq = PySequence_Check(krr);
	// multi node insert
		
	if (is_seq){
		int seq_size = PySequence_Size(krr);
		PyObject* nodes[seq_size];
		
		for (int i=0;i<seq_size;i++){
			
			PyObject* arg = PySequence_Tuple(PySequence_GetItem(krr, i));

			nodes[i] = PyObject_CallObject((PyObject *) &AVLNode_T, arg);
			Py_INCREF(nodes[i]);	

			findMeAParent(self, self->root, nodes[i]);		
		}
		
		for (int i=0;i<seq_size;i++){		
			propagateHeight(self, (AVLNode_O*)((AVLNode_O*)nodes[i])->parent);
		}
		
		self->size += seq_size;
		
	// single node insert
	} else {
		PyObject* node = PyObject_CallObject((PyObject *) &AVLNode_T, args);
		Py_INCREF(node);	

		self->size++;
		findMeAParent(self, self->root, node);
		propagateHeight(self, (AVLNode_O*)((AVLNode_O*)node)->parent);
	}

	return Py_BuildValue("");
}


static PyObject* avl_to_list(AVLTree_O* self){
	
	PyObject* list = Py_BuildValue("[]");
	appendElements(list, self->root);
	return list;

}

static PyObject* searchWraper(AVLTree_O* self, PyObject* args){
	PyObject* key = NULL;
	PyArg_ParseTuple(args, "O", &key);

	PyObject* ret = avl_search(self, key);
	if (ret == Py_None){ return ret; }
	return ((AVLNode_O*)ret)->value;

}

static PyObject* deleteWraper(AVLTree_O* self, PyObject* args){
	
	PyObject* key = NULL;
	PyArg_ParseTuple(args, "O", &key);
	
	return avl_delete(self, key);

}

static PyObject* avl_range(AVLTree_O* self, PyObject* args, PyObject* kwargs){
	if (self->root == Py_None) return Py_BuildValue("");

	PyObject* max = Py_None;
	PyObject* min = Py_None;
	char* maxOp = "<=";
	char* minOp = ">=";
	int mxOp = Py_GE;
	int mnOp = Py_LE;

	PyObject* list = Py_BuildValue("[]");
	BinaryCursor* cursor = bc_init((AVLNode_O*)self->root, 0);

	char* charList[] = {"min", "max", "minOp", "mmaxOp", NULL};
	
	PyArg_ParseTupleAndKeywords(args, kwargs, "|OO$ss:", charList, &min, &max, &maxOp, &minOp);

	PyObject* tree = PyObject_CallObject((PyObject *) &AVLTree_T, NULL);
	

	if (strcmp("<", maxOp) == 0) mxOp = Py_GT;
	if (strcmp(">", minOp) == 0) mnOp = Py_LT;
	
	while (bc_moveToNextNode_minMax(cursor, max, min, mxOp, mnOp)){
		AVLNode_O* n = bc_getCurrentNode(cursor);
		PyObject_CallMethodObjArgs(list, Py_BuildValue("s", "append"), Py_BuildValue("(OO)", n->key, n->value), NULL);
	}
	avl_push((AVLTree_O*)tree, Py_BuildValue("(O)", list));
	return tree;
}

// returns n largest nodes in tree
static PyObject* avl_n_max(AVLTree_O* self, PyObject* args){
	int n;

	PyArg_ParseTuple(args, "i", &n);
	PyObject* tree = PyObject_CallObject((PyObject *) &AVLTree_T, NULL);
	PyObject* list = Py_BuildValue("[]");	
	BinaryCursor* bc = bc_init((AVLNode_O*)self->root, 1);

	for (int i=0;i<n;i++){
		if (!bc_moveToNextNode(bc)) break;
		AVLNode_O* node = bc_getCurrentNode(bc);
		PyObject_CallMethodObjArgs(list, Py_BuildValue("s", "append"), Py_BuildValue("(OO)", node->key, node->value), NULL);
	}

	bc_dealloc(bc);
	avl_push((AVLTree_O*)tree, Py_BuildValue("(O)", list));
	return tree;

}

// returns n smalest nodes in tree
static PyObject* avl_n_min(AVLTree_O* self, PyObject* args){
	int n;

	PyArg_ParseTuple(args, "i", &n);
	PyObject* tree = PyObject_CallObject((PyObject *) &AVLTree_T, NULL);
	PyObject* list = Py_BuildValue("[]");	
	BinaryCursor* bc = bc_init((AVLNode_O*)self->root, 0);

	for (int i=0;i<n;i++){
		if (!bc_moveToNextNode(bc)) break;
		AVLNode_O* node = bc_getCurrentNode(bc);
		PyObject_CallMethodObjArgs(list, Py_BuildValue("s", "append"), Py_BuildValue("(OO)", node->key, node->value), NULL);
	}
	
	bc_dealloc(bc);
	avl_push((AVLTree_O*)tree, Py_BuildValue("(O)", list));
	return tree;

}

// sequece functions

static Py_ssize_t tree_size(AVLTree_O* self){
	
	return self->size;

}

static int value_in_tree(AVLTree_O* self, PyObject* value){
	if (avl_search(self, value) == Py_None) return 0;
	return 1;
}


// iter functions

static PyObject* avl_iter(AVLTree_O* self){
	Py_INCREF(self);
	self->cursor = bc_init((AVLNode_O*) self->root, 0);
	return (PyObject*)self;
}


static PyObject* avl_iter_next(AVLTree_O* self){
	if (bc_moveToNextNode(self->cursor)){
		AVLNode_O* n = bc_getCurrentNode(self->cursor);
		
		PyObject* ret = Py_BuildValue("(OO)", n->key, n->value);
		return ret;
	}
	Py_DECREF(self);
	bc_dealloc(self->cursor);
	return NULL;

}

#endif

