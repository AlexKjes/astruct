#ifndef AVLNODEHEADER
#define AVLNODEHEADER


#include <Python.h>

typedef	struct AVLNode AVLNode;
struct AVLNode{
	PyObject* key;
	PyObject* value;

	short int height;

	struct AVLNode* parent;
	struct AVLNode* children[2];

}; 



static void avln_dealloc(AVLNode* self);
static AVLNode* avln_init(PyObject* key, PyObject* value);


//int getBalanceFactor_C(PyObject* self);
//int getSubTreeHeight_C(PyObject* self);

//PyObject* getBalanceFactor(PyObject* self);
//PyObject* getSubTreeHeight(PyObject* self);




#endif
