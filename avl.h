#ifndef AVLTREEHEADER
#define AVLTREEHEADER


#include <Python.h>
#include "avln.h"
#include "binary_cursor.h"



typedef struct {
	PyObject_HEAD
	
	AVLNode* root;
	int size;

	BinaryCursor* cursor;	


} AVLTree_O; 



static void avl_dealloc(AVLTree_O* self);
static int avl_init(AVLTree_O* self, PyObject *args, PyObject *kwds);
static void findMeAParent(AVLTree_O* tree, AVLNode* root, AVLNode* orphan);
static void propagateHeight(AVLTree_O* tree, AVLNode* node);
static int getBalance(AVLNode* node);
static int getHeight(AVLNode* node);
static int balance(AVLTree_O* tree, AVLNode* root);



// Rotations //
void rotateLeft(AVLTree_O* tree, AVLNode* root);
void rotateDoubleLeft(AVLTree_O* tree, AVLNode* root);
void rotateRight(AVLTree_O* tree, AVLNode* root);
void rotateDoubleRight(AVLTree_O* tree, AVLNode* root);

//PyObject* getBalanceFactor(PyObject* self, PyObject* args, PyObject* kwds);
//PyObject* getSubTreeHeight(PyObject* self, PyObject* args, PyObject* kwds);

// tree operation helpers
static void parentSwap(AVLTree_O* tree, AVLNode* parent, AVLNode* child);
static int isLeftChildOfParent(AVLNode* node);


// diagnostic
static PyObject* avl_print_traverse(AVLNode* node);
static PyObject* balanceTest(AVLTree_O* self);
static PyObject* heightTest(AVLTree_O* self);

// Python exposed helper functions
static int getHeightRecursive(AVLNode* node);
static int getBalanceRecursive(AVLNode* node);
static void appendElements(PyObject* list, AVLNode* node);
static void avl_tree_map(AVLNode* node, void (*f)(AVLNode*));
static PyObject* notImplemented(void);

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
