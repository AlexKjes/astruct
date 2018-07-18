#ifndef AVLNODEHEADER
#define AVLNODEHEADER


#include <Python.h>



typedef struct {
	PyObject_HEAD
	PyObject* key;
	PyObject* value;

	short int height;

	PyObject* parent;
	//PyObject* left;
	//PyObject* right;
	PyObject* children[2];

} AVLNode_O; 



static void avln_dealloc(AVLNode_O *self);
static PyObject* avln_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static int avln_init(AVLNode_O* self, PyObject *args);


//int getBalanceFactor_C(PyObject* self);
//int getSubTreeHeight_C(PyObject* self);

//PyObject* getBalanceFactor(PyObject* self);
//PyObject* getSubTreeHeight(PyObject* self);




#endif
