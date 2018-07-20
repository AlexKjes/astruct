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
	{"height_test", (PyCFunction) heightTest, METH_NOARGS,
	 "Returns True if true height = cached height"},
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
	self->root = NULL;
	self->size = 0;

	return 0;

};


static void avl_dealloc(AVLTree_O* self){

	avl_tree_map(self->root, avln_dealloc);
    Py_TYPE(self)->tp_free((PyObject *) self);

}

// recursively appending node to tree	
static void findMeAParent(AVLTree_O* tree, AVLNode* root, AVLNode* orphan){
	
	AVLNode* rt = root;
	AVLNode* orphy = orphan;	

	if (!root) {
		tree->root = orphan;
		return;
	} 

	if (PyObject_RichCompareBool(rt->key, orphy->key, Py_GE)){
		if (!rt->children[0]) {
			rt->children[0] = orphan;
			orphy->parent = root;
		} else { findMeAParent(tree, rt->children[0], orphan); }
	} else {
		if (!rt->children[1]){
			rt->children[1] = orphan;
			orphy->parent = root;
		} else { findMeAParent(tree, rt->children[1], orphan); }
	}	

}

static void propagateHeight(AVLTree_O* tree, AVLNode* node){
	
	if (!node) return;

	int _height = getHeight(node);
	int hc = _height != node->height;
	int b = getBalance(node);
	if (hc || b > 1 || b < -1){
		//TODO remember py recursion stuff
		node->height = _height;		
		balance(tree, node);
		propagateHeight(tree, node->parent);
		
	}	

}

static int balance(AVLTree_O* tree, AVLNode* node){
	int b = getBalance(node);
	if (b < -1){
		if (getBalance(node->children[1]) < 0) {
			rotateLeft(tree, node); 
		} else { rotateDoubleLeft(tree, node); }
		return 1;
	} else if (b > 1){
		if (getBalance(node->children[0]) > 0) {
			rotateRight(tree, node); 
		} else { rotateDoubleRight(tree, node); }
		return 1;	
	}
	return 0;
}

static int getHeight(AVLNode* node){
	if (!node) return 0;
	int l, r;
	l = r = 0;
	if (node->children[1]) r = node->children[1]->height;
	if (node->children[0]) l = node->children[0]->height;

	return ((l > r) ? l : r) + 1;

}

static int getBalance(AVLNode* node){
	if (!node) return 0;
	int l, r;
	l = r = 0;
	if (node->children[1]) r = node->children[1]->height;
	if (node->children[0]) l = node->children[0]->height;

	return l - r;
}

/* single left rotation
rt
 \        r
  r -->  / \
   \    rt  rr
	rr
*/
void rotateLeft(AVLTree_O* tree, AVLNode* root){

	AVLNode* rt = root;
	AVLNode* rtp = (rt->parent) ? (AVLNode*) rt->parent : NULL;
	AVLNode* r = rt->children[1];
	

	if (!rtp) { 
		tree->root = r;
		r->parent = NULL;
	} else {
		r->parent = rtp;
		if (root == rtp->children[0]) { rtp->children[0] = r; }
		else { rtp->children[1] = r; }
	} 
	rt->parent = r;
	rt->children[1] = r->children[0];
	r->children[0] = rt;

	if (rt->children[1]) rt->children[1]->parent = rt;

	rt->height = getHeight(rt);

}

/* Double left rotation
1
 \        2
  3 -->  / \
 /      1   3
2
*/
void rotateDoubleLeft(AVLTree_O* tree, AVLNode* root){

	AVLNode* rt = root;
	AVLNode* rtp = (rt->parent) ? rt->parent : NULL;
	AVLNode* r = rt->children[1];
	AVLNode* rl = r->children[0];
	
	if (!rtp) { 
		tree->root = rl; 
		rl->parent = NULL;
	} else {
		rl->parent = rt->parent;
		if (root == rtp->children[0]) { rtp->children[0] = rl; }
		else { rtp->children[1] = rl; }
	}
	r->children[0] = rl->children[1];
	rt->children[1] = rl->children[0];
	r->parent = rl;
	rt->parent = rl;
	rl->children[0] = rt;
	rl->children[1] = r;

	if (rt->children[1]) rt->children[1]->parent = rt;
	if (r->children[0]) r->children[0]->parent = r;	
	
	rt->height = getHeight(rt);
	r->height = getHeight(r);
	
}

// right rotation, same as mirrored left rotation
void rotateRight(AVLTree_O* tree, AVLNode* root){

	AVLNode* rt = root;
	AVLNode* rtp = (rt->parent) ? rt->parent : NULL;
	AVLNode* l = rt->children[0];
	
	if (!rtp) { 
		tree->root = l; 
		l->parent = NULL;
	} else {
		l->parent = rt->parent;
		if (root == rtp->children[0]) { rtp->children[0] = l; }
		else { rtp->children[1] = l; }
	}
	rt->parent = l;
	rt->children[0] = l->children[1];
	l->children[1] = rt;

	if (rt->children[0]) rt->children[0]->parent = rt;
	
	rt->height = getHeight(rt);

}

// doubel right rotation. Same as doubleLeft only mirrored
void rotateDoubleRight(AVLTree_O* tree, AVLNode* root){

	AVLNode* rt = root;
	AVLNode* rtp = (rt->parent) ? rt->parent : NULL;
	AVLNode* l = rt->children[0];
	AVLNode* lr = l->children[1];
	
	if (!rtp) { 
		tree->root = lr;
		lr->parent = NULL;		
	} else {
		lr->parent = rt->parent;
		if (root == rtp->children[0]) { rtp->children[0] = lr; }
		else { rtp->children[1] = lr; }
	}
	l->children[1] = lr->children[0];
	rt->children[0] = lr->children[1];
	l->parent = lr;
	rt->parent = lr;
	lr->children[1] = rt;
	lr->children[0] = l;

	if (rt->children[0]) rt->children[0]->parent = rt;
	if (l->children[1]) l->children[1]->parent = l;	

	rt->height = getHeight(rt);
	l->height = getHeight(l);

}


static AVLNode* avl_search(AVLTree_O* self, PyObject* key){

	AVLNode* n = self->root;

	while (1){
		if (!n) return NULL;
		if (PyObject_RichCompareBool(n->key, key, Py_GT)){
			n = n->children[0];
		} else if (PyObject_RichCompareBool(n->key, key, Py_LT)){
			n = n->children[1];
		} else if (PyObject_RichCompareBool(n->key, key, Py_EQ)){
			return n;
		} else return NULL;

	}	

}

// deletes first instance of key found
static AVLNode* avl_delete(AVLTree_O* self, PyObject* value){
	
	AVLNode* propagateFrom = NULL;
	AVLNode* n = avl_search(self, value);
	AVLNode* replacement = NULL;
	if (!n) return NULL;
	// has no children
	if (!n->children[0] && !n->children[1]){
		if (n->parent){
			if (isLeftChildOfParent(n)){
				n->parent->children[0] = NULL;
			} else { n->parent->children[1] = NULL; }
			propagateFrom = n->parent;
		} 
		
	// if n has one child
	} else if ((!n->children[0] && n->children[1]) || (n->children[0] && !n->children[1])){
		replacement = (n->children[0]) ? n->children[0] : n->children[1];
		replacement->parent = n->parent;
		parentSwap(self, n, replacement);
		propagateFrom = replacement->parent;
	// node has two children
	} else {
		int lr = (getBalance(n) > 0) ? 1 : 0;
		replacement = n->children[1 - lr];
		// replacement has no children
		if (!replacement->children[lr]){
			propagateFrom = replacement;
			replacement->parent = n->parent;
			replacement->children[lr] = n->children[lr];
			n->children[lr]->parent = replacement;
			parentSwap(self, n, replacement);
			
		} else {
			// gong left
			while(replacement->children[lr]){
				replacement = replacement->children[lr];
			}
			propagateFrom = replacement->parent;
			// if left most node in subtree has a right child
			if (replacement->children[1-lr]){
				AVLNode* rc = replacement->children[1-lr];
				rc->parent = replacement->parent;
				replacement->parent->children[lr] = rc;
			} else {
				replacement->parent->children[lr] = NULL;
			}
				replacement->parent = n->parent;
				parentSwap(self, n, replacement);
				replacement->children[0] = n->children[0];
				replacement->children[1] = n->children[1];
				n->children[0]->parent = n->children[1]->parent = replacement;
				replacement->height = n->height;
		}
	}
	// if tree root was deleted
	if (!n->parent)
		self->root = replacement;	

	n->parent = NULL;
	n->children[0] = NULL;
	n->children[1] = NULL;

	self->size--;
	avln_dealloc(n);

	return propagateFrom;
}


///////////////////////
// Operation helpers //
///////////////////////

static int isLeftChildOfParent(AVLNode* node){
	
	if (node == node->parent->children[0]) return 1;
	return 0;

}

static void parentSwap(AVLTree_O* tree, AVLNode* parent, AVLNode* child){
	
	if (!parent->parent) {
		tree->root = child;
		return;
	}
	child->parent = parent->parent;
	if (isLeftChildOfParent(parent)){
		parent->parent->children[0] = child;
		return;
	}
	parent->parent->children[1] = child;
	return;

}





////////////////////////////
// python exposed helpers //
////////////////////////////


// do thing to each node in tree
static void avl_tree_map(AVLNode* node, void (*f)(AVLNode*)){
	
	if (!node) return;

	avl_tree_map(node->children[0], f);
	f(node);
	avl_tree_map(node->children[1], f);

}


static void appendElements(PyObject* list, AVLNode* node){
	
	if (!node) return;
	else {
		AVLNode* n = node;
		appendElements(list, n->children[0]);
		PyList_Append(list, Py_BuildValue("OO", n->key, n->value));
		appendElements(list, n->children[1]);
	}
	
}

// recursively prints key/value in ascending order
static PyObject* avl_print_traverse(AVLNode* node){
	AVLNode* n = node;

	if (!node) { return Py_BuildValue(""); }
	avl_print_traverse(n->children[0]);
	printf("%s: %s\n", PyUnicode_AsUTF8(PyObject_Repr(n->key)), PyUnicode_AsUTF8(PyObject_Repr(n->value)));
	avl_print_traverse(n->children[1]);
	return Py_BuildValue("");
}

// finds height of node recursively
static int getHeightRecursive(AVLNode* node){
	if (!node) return 0;
	
	int l, r;
	l = r = 0;
	if (node->children[1]) r = getHeightRecursive(node->children[1]);
	if (node->children[0]) l = getHeightRecursive(node->children[0]); 

	return ((l > r) ? l : r) + 1;
}

// returns balance from recursive height
static int getBalanceRecursive(AVLNode* node){
	if (!node) return 0;
	
	int l, r;
	r = getHeightRecursive(node->children[1]);
	l = getHeightRecursive(node->children[0]); 

	return l - r;
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
static PyObject* heightTest(AVLTree_O* self){
	BinaryCursor* bc = bc_init(self->root, 0);
	int err = 0;
	while (bc_moveToNextNode(bc)){
		AVLNode* n = bc_getCurrentNode(bc);
		int rh = getHeightRecursive(n);
		int h = n->height;
		if (rh != h){
			printf("height: %d, should be: %d\n", h, rh);
			err = 1;
			//bc_dealloc(bc);			
			//Py_INCREF(Py_False);			
			//return Py_False;	
		}		
	}
	bc_dealloc(bc);	
	if (err) {				
		Py_INCREF(Py_False);			
		return Py_False;	
	}	
	Py_INCREF(Py_True);
	return Py_True;

}

// Checks that the tree is balanced from seperatly calculated height
static PyObject* balanceTest(AVLTree_O* self){
	BinaryCursor* bc = bc_init(self->root, 0);

	while (bc_moveToNextNode(bc)){
		AVLNode* n = bc_getCurrentNode(bc);
		int b = getBalanceRecursive(n);
		if (b > 2 || b < -2){
			printf("%d, %d, %d\n", b, n->height, getHeightRecursive(n));
			bc_dealloc(bc);
			Py_INCREF(Py_False);			
			return Py_False;
		}
	}
	bc_dealloc(bc);
	Py_INCREF(Py_True);
	return Py_True;

}

static PyObject* notImplemented(void){
	return Py_NotImplemented;
}


//////////////////////////////
/// Python exposed methods ///
//////////////////////////////

static PyObject* avl_push(AVLTree_O* self, PyObject* args){
	
	
	PyObject* krr = NULL;
	PyObject* val = NULL;
	int is_seq;
	PyArg_ParseTuple(args, "O|O", &krr, &val);
	is_seq = PySequence_Check(krr);
	// multi node insert
		
	if (is_seq){
		int seq_size = PySequence_Size(krr);
		AVLNode* nodes[seq_size];
		
		for (int i=0;i<seq_size;i++){
			PyObject* arg = PySequence_GetItem(krr, i);
			nodes[i] = avln_init(PySequence_GetItem(arg, 0), PySequence_GetItem(arg, 1));
			findMeAParent(self, self->root, nodes[i]);	
			Py_XDECREF(arg);	
		}
		
		for (int i=0;i<seq_size;i++){		
			propagateHeight(self, nodes[i]->parent);
		}
		
		self->size += seq_size;
		
	// single node insert
	} else {
		AVLNode* node = avln_init(krr, val);
		self->size++;
		findMeAParent(self, self->root, node);
		propagateHeight(self, node->parent);
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

	AVLNode* ret = avl_search(self, key);
	if (!ret){ return Py_None; }
	return ret->value;

}

static PyObject* deleteWraper(AVLTree_O* self, PyObject* args){
	
	PyObject* key = NULL;
	PyArg_ParseTuple(args, "O", &key);

	if (PySequence_Check(key)){
		
		int len = PySequence_Size(key);
		AVLNode* list[len];
		for (int i=0;i<len;i++){
			PyObject* tmp = PySequence_GetItem(key, i);
			list[i] = avl_delete(self, tmp); 
			if (list[i])
				propagateHeight(self, list[i]);
			Py_XDECREF(tmp);
		}
		/*
		for (int i=0;i<len;i++){
			if (list[i] != Py_None){
				AVLNode* n = (AVLNode*)list[len-1-i];
				//if (n->children[0] != Py_None && n->children[1] != Py_None)
					propagateHeight(self, n);
			}
		}
		*/
		Py_INCREF(Py_None);
		return Py_None;
		

	} else {
		AVLNode* ret = avl_delete(self, key);
		if (ret){
			propagateHeight(self, ret);
			Py_INCREF(Py_True);
			return Py_True;
		}
	}
	Py_INCREF(Py_False);
	return Py_False;
	
}

static PyObject* avl_range(AVLTree_O* self, PyObject* args, PyObject* kwargs){
	if (!self->root) return Py_BuildValue("");

	PyObject* max = Py_None;
	PyObject* min = Py_None;
	char* maxOp = "<=";
	char* minOp = ">=";
	int mxOp = Py_GE;
	int mnOp = Py_LE;

	PyObject* list = Py_BuildValue("[]");
	BinaryCursor* cursor = bc_init((AVLNode*)self->root, 0);

	char* charList[] = {"min", "max", "minOp", "mmaxOp", NULL};
	
	PyArg_ParseTupleAndKeywords(args, kwargs, "|OO$ss:", charList, &min, &max, &maxOp, &minOp);

	PyObject* tree = PyObject_CallObject((PyObject *) &AVLTree_T, NULL);
	

	if (strcmp("<", maxOp) == 0) mxOp = Py_GT;
	if (strcmp(">", minOp) == 0) mnOp = Py_LT;
	
	while (bc_moveToNextNode_minMax(cursor, max, min, mxOp, mnOp)){
		AVLNode* n = bc_getCurrentNode(cursor);
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
	BinaryCursor* bc = bc_init((AVLNode*)self->root, 1);

	for (int i=0;i<n;i++){
		if (!bc_moveToNextNode(bc)) break;
		AVLNode* node = bc_getCurrentNode(bc);
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
	BinaryCursor* bc = bc_init((AVLNode*)self->root, 0);

	for (int i=0;i<n;i++){
		if (!bc_moveToNextNode(bc)) break;
		AVLNode* node = bc_getCurrentNode(bc);
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


// iter functions

static PyObject* avl_iter(AVLTree_O* self){
	self->cursor = bc_init((AVLNode*) self->root, 0);
	Py_INCREF(self);
	return (PyObject*)self;
}


static PyObject* avl_iter_next(AVLTree_O* self){
	if (bc_moveToNextNode(self->cursor)){
		AVLNode* n = bc_getCurrentNode(self->cursor);
		
		PyObject* ret = Py_BuildValue("(OO)", n->key, n->value);
		return ret;
	}
	bc_dealloc(self->cursor);
	return NULL;

}

#endif

