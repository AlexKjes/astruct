#ifndef AVLTREEHEADER
#define AVLTREEHEADER


#include <Python.h>
#include "avln.h"
#include "binary_cursor.h"



typedef struct {
	PyObject_HEAD
	
	PyObject* root;
	int size;

	BinaryCursor* cursor;	


} AVLTree_O; 



static void avl_dealloc(AVLTree_O *self);
static int avl_init(AVLTree_O* self, PyObject *args, PyObject *kwds);
static void findMeAParent(AVLTree_O* tree, PyObject* root, PyObject* orphan);
static void propagateHeight(AVLTree_O* tree, AVLNode_O* node);
static int getBalance(AVLNode_O* node);
static int getHeight(AVLNode_O* node);
static int balance(AVLTree_O* tree, AVLNode_O* root);
static void avl_tree_map(PyObject* node, void (*f)(AVLNode_O*));


// Rotations //
void rotateLeft(AVLTree_O* tree, PyObject* root);
void rotateDoubleLeft(AVLTree_O* tree, PyObject* root);
void rotateRight(AVLTree_O* tree, PyObject* root);
void rotateDoubleRight(AVLTree_O* tree, PyObject* root);

//PyObject* getBalanceFactor(PyObject* self, PyObject* args, PyObject* kwds);
//PyObject* getSubTreeHeight(PyObject* self, PyObject* args, PyObject* kwds);

// tree operation helpers
static void parentSwap(AVLTree_O* tree, AVLNode_O* parent, AVLNode_O* child);
static int isLeftChildOfParent(AVLNode_O* node);


// diagnostic
static PyObject* avl_print_traverse(PyObject* node);
static PyObject* balanceTest(AVLTree_O* self);

// Python exposed helper functions
static int getHeightRecursive(AVLNode_O* node);
static int getBalanceRecursive(AVLNode_O* node);
static void balanceTestRecursive(AVLNode_O* node, int* nodeBalanceList, int* counter);
static void appendElements(PyObject* list, PyObject* node);

// Python exposed functions
static PyObject* avl_push(AVLTree_O* self, PyObject* args);
static PyObject* avl_print_asc(AVLTree_O* self);
static PyObject* avl_to_list(AVLTree_O* self);
static PyObject* searchWraper(AVLTree_O* self, PyObject* args);
static PyObject* deleteWraper(AVLTree_O* self, PyObject* args);
static PyObject* avl_range(AVLTree_O* self, PyObject* args, PyObject* kwargs);
static PyObject* avl_n_max(AVLTree_O* self, PyObject* args);
static PyObject* avl_n_min(AVLTree_O* self, PyObject* args);

// sequnece functions
static Py_ssize_t tree_size(AVLTree_O* self);

// iter functions
static PyObject* avl_iter(AVLTree_O* self);
static PyObject* avl_iter_next(AVLTree_O* self);


#endif
